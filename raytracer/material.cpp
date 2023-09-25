#include "material.h"
#include "ray.h"
#include "glm.hpp"
#include "util.h"
#include "geometry.h"

bool lambertian_scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered_ray, const color& light_color) {
	glm::dvec3 scattered_ray_direction = rec.normal + random_hemispherical_direction(rec.normal);

	// To avoid zero vector as scatter direction if random direction vector is opposite normal
	if (near_zero(scattered_ray_direction)) {
		scattered_ray_direction = rec.normal;
	}

	scattered_ray = create_ray(rec.point, scattered_ray_direction);

	if (light_color != color(0.0, 0.0, 0.0)) {
		attenuation = (rec.material_color + light_color) / 2.0;
	}
	else {
		attenuation = rec.material_color;
	}
	
	return true;
}

void calculate_specular_spot(const camera& camera, const hit_record& rec, double shininess, const color& light_color, color& attenuation, const color& in_color) {
	// Calculate possible specular spot
	glm::dvec3 viewer_direction = glm::normalize(camera.look_from - rec.point);
	glm::dvec3 veiwer_reflection_direction = reflect(-viewer_direction, rec.normal);

	// Calculate the threshold angle based on the shininess
	double angle_threshold = glm::radians(2.0 / shininess); // Adjust this factor as needed

	// Calculate the angle (in radians) between viewer direction and reflection direction
	double angle_radians = std::acos(glm::dot(viewer_direction, veiwer_reflection_direction));

	// Blend between specular color and material's base color based on the angle
	if (angle_radians > angle_threshold) {
		if (light_color != color(0.0, 0.0, 0.0)) {
			attenuation = (in_color + light_color) / 2.0; // If the point is illuminated by any number of light sources, blend their colors into the color contribution
		}
		else {
			attenuation = in_color; // Use material's base color
		}
	}
	else {
		double specular_intensity = glm::pow(std::max(0.0, glm::dot(veiwer_reflection_direction, viewer_direction)), shininess); // Specularity according to Blinn-Phong model
		color specular_color = specular_intensity * light_color;
		attenuation = specular_color; // Use specular color
	}
}

bool metallic_reflection(const ray& ray_in, const hit_record& rec, color& attenuation, ray& reflected_ray, double metallic_fuzz, const camera& camera, double shininess, const color& light_color) {
	// Calculate trajectory for reflected ray
	glm::dvec3 reflected_ray_direction = reflect(glm::normalize(ray_in.direction), rec.normal);
	reflected_ray = create_ray(rec.point, reflected_ray_direction + (metallic_fuzz * random_hemispherical_direction(rec.normal)));
	
	calculate_specular_spot(camera, rec, shininess, light_color, attenuation, rec.material_color);

	return (glm::dot(reflected_ray.direction, rec.normal) > 0.0); // Absorb rays that reflect below the surface of the object
}

// Schlick's approximation for reflectance
double reflectance(double cosine, double refraction_index) {
	double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1.0 - r0) * std::pow((1 - cosine), 5);
}

bool dielectric_refraction(const ray& ray_in, const hit_record& rec, color& attenuation, ray& ray_out, double refraction_index, const camera& camera, double shininess, const color& light_color) {
	attenuation = color(1.0, 1.0, 1.0);
	double refraction_ratio = rec.outward_face ? (1.0 / refraction_index) : refraction_index; 
	glm::dvec3 unit_direction = glm::normalize(ray_in.direction);

	double cos_theta = std::min(glm::dot(-unit_direction, rec.normal), 1.0);
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	bool cannot_refract = refraction_ratio * sin_theta > 1.0;

	glm::dvec3 direction;

	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
		calculate_specular_spot(camera, rec, shininess, light_color, attenuation, color(1.0, 1.0, 1.0));
		direction = reflect(unit_direction, rec.normal);
	}
	else {
		direction = refract(unit_direction, rec.normal, refraction_ratio);
	}

	double a = std::abs(glm::dot(direction, rec.normal));
	
	ray_out = create_ray(rec.point, direction);

	return true;
}