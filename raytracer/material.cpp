#include "material.h"
#include "ray.h"
#include "glm.hpp"
#include "util.h"
#include "geometry.h"
#include "camera.h"
#include "pdf.h"

color emitted(const hit_record& rec) {
	if (!rec.outward_face) {
		return rec.material_color;
	}
	else {
		return color(0.0, 0.0, 0.0);
	}
}

bool lambertian_scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered_ray, double& pdf) {
	onb onb = build_onb_from_w(rec.normal);
	glm::dvec3 scattered_ray_direction = local_coord(onb, random_cosine_direction());

	scattered_ray = create_ray(rec.point, scattered_ray_direction);

	attenuation = rec.material_color;

	pdf = glm::dot(onb.w, scattered_ray_direction) / pi;

	return true;
}

double lambertian_scatter_pdf(const ray& ray_in, const hit_record& rec, const ray& scattered_ray) {
	double cosine = glm::dot(rec.normal, glm::normalize(scattered_ray.direction));
	return cosine < 0.0 ? 0.0 : cosine / pi;
}

bool metallic_reflection(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double metallic_fuzz) {
	// Calculate trajectory for reflected ray
	glm::dvec3 reflected_ray_direction = reflect(glm::normalize(ray_in.direction), rec.normal);
	reflected_ray = create_ray(rec.point, reflected_ray_direction + (metallic_fuzz * random_hemispherical_direction(rec.normal)));
	attenuation = rec.material_color;
	return (glm::dot(reflected_ray.direction, rec.normal) > 0.0); // Absorb rays that reflect below the surface of the object
}

// Schlick's approximation for reflectance
double reflectance(double cosine, double refraction_index) {
	double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1.0 - r0) * std::pow((1 - cosine), 5);
}

bool dielectric_refraction(const ray& ray_in, const hit_record& rec, color& attenuation, ray& ray_out, double refraction_index) {
	attenuation = color(1.0, 1.0, 1.0);
	double refraction_ratio = rec.outward_face ? (1.0 / refraction_index) : refraction_index; // Adjust refraction index depending on if ray is travelling from inside or outside the object
	glm::dvec3 unit_direction = glm::normalize(ray_in.direction);

	double cos_theta = std::min(glm::dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	bool cannot_refract = refraction_ratio * sin_theta > 1.0;

	glm::dvec3 direction;

	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
		direction = reflect(unit_direction, rec.normal);
	}
	else {
		direction = refract(unit_direction, rec.normal, refraction_ratio);
	}

	double a = std::abs(glm::dot(direction, rec.normal));
	
	ray_out = create_ray(rec.point, direction);

	return true;
}

// Scatters probabilistically depending on medium density
bool constant_density_medium_scatter(const scene_object& intersected_object, const ray& ray_in, hit_record& rec, ray& ray_out, double density, color& attenuation, const std::vector<scene_object>& scene_objects) {
	// Get a random hit distance depending on density, if intersection point + hit distance is outside of bounding box for the geometry the ray passes through
	// Otherwise, it uniformly scatters from intersection point + hit distance
	
	double hit_distance = (-1.0 / density) * glm::log(random_double(1e-8, 1.0));

	// std::cout << "Hit distance: " << hit_distance << std::endl;

	point3 evaluation_point = rec.point + (ray_in.direction * hit_distance);

	bool contains = false;
	point3 random_point_on_intersected_object;

	switch (intersected_object.object_type) {
		case SPHERE:
			contains = sphere_contains_point(intersected_object, evaluation_point);
			random_point_on_intersected_object = get_random_point_on_sphere(rec.point, intersected_object, rec);
		break;
		case QUAD:
			contains = quad_contains_point(intersected_object, evaluation_point);
			random_point_on_intersected_object = get_random_point_on_quad(rec.point, intersected_object);
		break;
		case CUBE:
			contains = cube_contains_point(intersected_object, evaluation_point);
			random_point_on_intersected_object = get_random_point_on_cube(rec.point, intersected_object, scene_objects);
		break;
		case ASYMMETRIC_CUBE:
			contains = cube_contains_point(intersected_object, evaluation_point);
			random_point_on_intersected_object = get_random_point_on_cube(rec.point, intersected_object, scene_objects);
		break;
	}

	// If density scattering should occur scatter uniformly from evaluation point and set color
	if (contains) {
		std::cout << "Evaluation point: ";
		print_vector(std::cout, evaluation_point);
		std::cout << std::endl;
		std::cout << "Sphere center: ";
		print_vector(std::cout, intersected_object.sphere_center);
		std::cout << std::endl;

		attenuation = rec.material_color;

		glm::dvec3 random_direction = random_cosine_direction();

		ray_out = create_ray(evaluation_point, random_direction);

		hit_record rec_second_intersection;
		find_intersection(ray_out, interval{ 0.001, infinity }, rec_second_intersection, scene_objects);

		// If second intersection from the inside, move passthrough ray to far side
		if (glm::dot(ray_out.direction, rec_second_intersection.normal) > 0.0) {
			rec.point = rec_second_intersection.point;
			ray_out = create_ray(rec_second_intersection.point, random_direction);
		}
		
	}
	else {
		hit_record rec_second_intersection;
		find_intersection(ray_in, interval{ 0.001, infinity }, rec_second_intersection, scene_objects);

		// If second intersection from the inside, move passthrough ray to far side
		if (glm::dot(ray_in.direction, rec_second_intersection.normal) > 0.0) {
			rec.point = rec_second_intersection.point;
			ray_out = create_ray(rec_second_intersection.point, ray_in.direction);
		}
	}

	return true;

}