#include "material.h"
#include "ray.h"
#include "glm.hpp"
#include "util.h"
#include "geometry.h"
#include "camera.h"

bool lambertian_scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered_ray) {
	glm::dvec3 scattered_ray_direction = rec.normal + random_hemispherical_direction(rec.normal);

	// To avoid zero vector as scatter direction if random direction vector is opposite normal
	if (near_zero(scattered_ray_direction)) {
		scattered_ray_direction = rec.normal;
	}

	scattered_ray = create_ray(rec.point, scattered_ray_direction);

	attenuation = rec.material_color;

	return true;
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