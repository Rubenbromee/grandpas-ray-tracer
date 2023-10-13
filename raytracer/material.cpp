#include "material.h"
#include "ray.h"
#include "glm.hpp"
#include "util.h"
#include "geometry.h"
#include "camera.h"
#include "pdf.h"

// Uniform lambertian scattering
bool lambertian_scatter(const ray& ray_in, const hit_record& rec, color& attenuation, ray& scattered_ray, double& pdf) {
	onb onb = build_onb_from_w(rec.normal);
	glm::dvec3 scattered_ray_direction = local_coord(onb, random_cosine_direction());

	scattered_ray = create_ray(rec.point, scattered_ray_direction);

	attenuation = rec.material_color;

	pdf = glm::dot(onb.w, scattered_ray_direction) / pi;

	return true;
}

// Probability density function for lambertian scattering
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
	// Calculate the reflection coefficient (Fresnel reflectance) using Schlick's approximation
	double r0 = (1.0 - refraction_index) / (1.0 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1.0 - r0) * std::pow((1 - cosine), 5);
}

bool dielectric_refraction(const ray& ray_in, const hit_record& rec, color& attenuation, ray& ray_out, double refraction_index) {
	attenuation = color(1.0, 1.0, 1.0);
	double refraction_ratio = rec.outward_face ? (1.0 / refraction_index) : refraction_index; // Adjust refraction index depending on if ray is travelling from inside or outside the object
	glm::dvec3 unit_direction = glm::normalize(ray_in.direction);

	// Calculate the cosine of the angle between the ray direction and the surface normal
	double cos_theta = std::min(glm::dot(-unit_direction, rec.normal), 1.0);

	// Calculate the sine of the angle using trigonometric identity
	double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

	// Check if total internal reflection occurs (cannot refract)
	bool cannot_refract = refraction_ratio * sin_theta > 1.0;

	glm::dvec3 direction;

	if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
		// If total internal reflection occurs or a random value exceeds reflectance, reflect the ray
		direction = reflect(unit_direction, rec.normal);
	}
	else {
		// Otherwise, refract the ray using Snell's law
		direction = refract(unit_direction, rec.normal, refraction_ratio);
	}
	
	ray_out = create_ray(rec.point, direction);

	return true;
}

// When a constant density medium is intersected, give the point the color of the geometry and randomly scatter the ray at the geometrical intersection point
bool constant_density_medium_scatter(const hit_record& rec, color& attenuation, ray& ray_out) {
	ray_out = create_ray(rec.point, glm::normalize(random_hemispherical_direction(rec.normal)));
	attenuation = rec.material_color;
	return true;
}