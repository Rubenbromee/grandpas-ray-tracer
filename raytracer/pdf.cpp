#include <random>
#include <vector>
#include "pdf.h"
#include "geometry.h"
#include "glm.hpp"
#include "util.h"
#include "ray.h"

// Calculates the pdf of an intersectable based on the distance to it and its surface area
double intersectable_pdf(point3 origin, glm::dvec3 sample_object_direction, const scene_object& sample_object, const hit_record& rec, const point3 random_point_on_sample_object) {
	double area = 1.0; // To prevent possible 0 division

	// For the non-quad geometries I'm assuming that half of the light area is visible to the geometry intersection point
	// This will hopefully be a good enough estimation
	switch (sample_object.object_type) {
	case SPHERE:
		area = sample_object.sphere_area / 2.0;
		break;
	case QUAD:
		area = sample_object.quad_area;
		break;
	case CUBE:
		area = sample_object.cube_area / 2.0;
		break;
	case ASYMMETRIC_CUBE:
		area = sample_object.cube_area / 2.0;
		break;
	}

	double distance_squared = glm::distance(rec.point, random_point_on_sample_object) * glm::distance(rec.point, random_point_on_sample_object);
	double cosine = glm::abs(glm::dot(sample_object_direction, rec.normal) / glm::length(sample_object_direction));

	return distance_squared / (cosine * area);
}

// Point the indirect ray towards a random point on a sample object (light, dielectric object), calculate pdf based on distance to object
void calculate_intersectable_pdf(const scene_object& sample_object, const hit_record& rec, glm::dvec3& scattered_ray_direction, double& pdf, const std::vector<scene_object>& scene_objects) {
	point3 random_point_on_sample_object;

	switch (sample_object.object_type) {
		case SPHERE:
			random_point_on_sample_object = get_random_point_on_sphere(rec.point, sample_object, rec);
		break;
		case QUAD:
			random_point_on_sample_object = get_random_point_on_quad(rec.point, sample_object);
			break;
		case CUBE:
			random_point_on_sample_object = get_random_point_on_cube(rec.point, sample_object, scene_objects);
		break;
		case ASYMMETRIC_CUBE:
			random_point_on_sample_object = get_random_point_on_cube(rec.point, sample_object, scene_objects);
		break;
	}

	glm::dvec3 direction_to_sample_object = glm::normalize(random_point_on_sample_object - rec.point);
	scattered_ray_direction = direction_to_sample_object;
	pdf = intersectable_pdf(rec.point, direction_to_sample_object, sample_object, rec, random_point_on_sample_object);
}

// Uniformly samples in a hemisphere around the geometry intersection point
double cosine_pdf(const glm::dvec3& normal, const glm::dvec3& random_direction) {
	double cosine = glm::dot(glm::normalize(random_direction), normal);
	return (cosine <= 0.0) ? 0.0 : cosine / pi;
}

// Used for sampling cube light sources
point3 get_random_point_on_cube(point3 origin, const scene_object& cube, const std::vector<scene_object>& scene_objects) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);
	std::uniform_int_distribution<int> index_distribution(0, 11);

	int random_triangle_index = index_distribution(gen);

	// Choose a random triangle
	const triangle& random_triangle = cube.cube_triangles[random_triangle_index];

	// Choose a random point on that triangle
	double u = uniform_distribution(gen);
	double v = uniform_distribution(gen);

	if (u + v > 1.0) {
		u = 1.0 - u;
		v = 1.0 - v;
	}

	point3 random_point_on_cube = u * random_triangle.vertices[0] + v * random_triangle.vertices[1] + (1.0 - u - v) * random_triangle.vertices[2];

	glm::dvec3 vector_to_random_point = random_point_on_cube - origin;

	// If this dot product is positive it means we've hit the far side of the cube, seen from the origin, and want to reflect the point to the near side
	// Do so by finding the intersection in the negative normal direction
	if (glm::dot(vector_to_random_point, random_triangle.normal) > 0.0) {
		hit_record rec;
		find_intersection(create_ray(random_point_on_cube, -random_triangle.normal), interval{ 0.001, infinity }, rec, scene_objects);
		random_point_on_cube = rec.point;
	}

	return random_point_on_cube;
}

// Used for sampling quad light sources
point3 get_random_point_on_quad(point3 origin, const scene_object& quad) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);
	std::uniform_int_distribution<int> index_distribution(0, 1);

	int random_triangle_index = index_distribution(gen);

	// Choose a random triangle
	const triangle& random_triangle = quad.quad_triangles[random_triangle_index];

	// Choose a random point on that triangle
	double u = uniform_distribution(gen);
	double v = uniform_distribution(gen);

	if (u + v > 1.0) {
		u = 1.0 - u;
		v = 1.0 - v;
	}

	point3 random_point_on_quad = u * random_triangle.vertices[0] + v * random_triangle.vertices[1] + (1.0 - u - v) * random_triangle.vertices[2];

	return random_point_on_quad;
}

// Used for sampling spherical light sources
point3 get_random_point_on_sphere(point3 origin, const scene_object& sphere, const hit_record& rec) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);

	// Generate random spherical coordinates
	double theta = 2.0 * pi * uniform_distribution(gen); // Random angle in radians
	double phi = acos(2.0 * uniform_distribution(gen) - 1.0); // Random inclination angle in radians

	// Convert spherical coordinates to Cartesian coordinates
	double x = sphere.sphere_center.x + sphere.sphere_radius * sin(phi) * cos(theta);
	double y = sphere.sphere_center.y + sphere.sphere_radius * sin(phi) * sin(theta);
	double z = sphere.sphere_center.z + sphere.sphere_radius * cos(phi);

	point3 random_point = point3(x, y, z);

	glm::dvec3 vector_to_random_point = random_point - origin;

	// If the point is on the far side of the sphere seen from the origin, move it to the near side
	if (glm::dot(vector_to_random_point, rec.normal) > 0.0) {
		random_point = random_point + (2.0 * sphere.sphere_radius) * (-rec.normal);
	}

	return random_point;
}