#include <iostream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "ray.h"

ray create_ray(const point3& origin, const glm::dvec3& direction) {
	ray ray;
	ray.origin = origin;
	ray.direction = direction;
	return ray;
}

const point3& ray_at(ray ray, double time) {
	return ray.origin + time * ray.direction;
}

// Creates a random direcitonal vector based on the normal of a point
// Adjusts direction based on the normals hemisphere
glm::dvec3 random_hemispherical_direction(const glm::dvec3& normal) {

	// Generate random spherical coordinates in a full hemisphere
	double theta = random_double(0.0, pi); // Azimuthal angle (0 to pi)
	double phi = random_double(0.0, 2.0 * pi); // Polar angle (0 to 2*pi)

	// Convert spherical coordinates to Cartesian coordinates
	double sin_theta = std::sin(theta);
	double cos_theta = std::cos(theta);
	double sin_phi = std::sin(phi);
	double cos_phi = std::cos(phi);

	// Calculate the direction vector
	glm::dvec3 direction(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);

	// Ensure the direction vector is in the hemisphere defined by the normal of the intersection point, otherwise, flip direction vector
	if (glm::dot(direction, normal) < 0.0) {
		direction = -direction;
	}

	// Normalize the direction vector
	direction = glm::normalize(direction);

	return direction;
}

// Reflect a ray on a point based on the normal of the point
glm::dvec3 reflect(const glm::dvec3& vector, const glm::dvec3& normal) {
	return vector - 2 * glm::dot(vector, normal) * normal;
}

// Refract a ray using Snell's law
glm::dvec3 refract(const glm::dvec3& unit_vector, const glm::dvec3& normal, double etai_over_etat) {
	auto cos_theta = std::min(glm::dot(-unit_vector, normal), 1.0);
	glm::dvec3 ray_out_perpendicular = etai_over_etat * (unit_vector + cos_theta * normal);
	glm::dvec3 ray_out_parallel = -std::sqrt(std::abs(1.0 - glm::dot(ray_out_perpendicular, ray_out_perpendicular))) * normal;
	return ray_out_perpendicular + ray_out_parallel;
}