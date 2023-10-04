#include <random>
#include <vector>
#include "pdf.h"
#include "geometry.h"
#include "glm.hpp"
#include "util.h"
#include "ray.h"

// Samples towards a light in the scene
double intersectable_pdf(point3 origin, glm::dvec3 light_direction, const scene_object& light, const hit_record& rec) {
	if (light.material != LIGHT) {
		return infinity;
	}
	double area = 1.0;

	// For the non-quad geometries I'm assuming that half of the light area is visible to the geometry intersection point
	switch (light.object_type) {
	case SPHERE:
		area = light.sphere_area / 2.0;
		break;
	case CUBE:
		area = light.cube_area / 2.0;
		break;
	case QUAD:
		area = light.quad_area;
		break;
	case ASYMMETRIC_CUBE:
		area = light.cube_area / 2.0;
		break;
	}
	double distance_squared = rec.time * rec.time * glm::dot(light_direction, light_direction);
	double cosine = glm::abs(glm::dot(light_direction, rec.normal) / glm::length(light_direction));
	return distance_squared / (cosine * area);
}

void calculate_intersectable_pdf(const scene_object& light, const hit_record& rec, glm::dvec3& scattered_ray_direction, double& pdf, const std::vector<scene_object> scene_objects, int& nr_contributing_rays) {
	point3 random_point_on_light;

	switch (light.object_type) {
		case SPHERE:
			random_point_on_light = get_random_point_on_sphere(rec.point, light, rec);
		break;
		case CUBE:
			random_point_on_light = get_random_point_on_cube(rec.point, light, scene_objects);
		break;
		case ASYMMETRIC_CUBE:
			random_point_on_light = get_random_point_on_cube(rec.point, light, scene_objects);
		break;
		case QUAD:
			random_point_on_light = get_random_point_on_quad(rec.point, light);
		break;
	}

	// Check if ray towards light hits anything except a light source, don't contribute from this light source
	// Ok, so this is shadow rays
	glm::dvec3 direction_towards_light = glm::normalize(random_point_on_light - rec.point);
	hit_record temp_rec;
	bool hit_anything;
	scene_object intersected_object = find_intersection_return_scene_object(create_ray(rec.point, direction_towards_light), interval{ 0.001, infinity }, temp_rec, scene_objects, hit_anything);
	if (hit_anything && intersected_object.material != LIGHT) {
		return;
	}

	glm::dvec3 direction_to_light = glm::normalize(random_point_on_light - rec.point);
	scattered_ray_direction += direction_to_light;
	pdf += intersectable_pdf(rec.point, direction_to_light, light, rec);
	nr_contributing_rays++;
}

// Uniformly samples in a hemisphere around the geometry intersection point
double cosine_pdf(const glm::dvec3& normal, const glm::dvec3& random_direction) {
	double cosine = glm::dot(glm::normalize(random_direction), normal);
	return (cosine <= 0.0) ? 0.0 : cosine / pi;
}

// Used for sampling cube light sources
point3 get_random_point_on_cube(point3 origin, const scene_object& cube, const std::vector<scene_object> scene_objects) {
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

	// If this dot product is negative it means we've hit the far side of the cube and want to reflect the point to the near side
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
	double x = sphere.center.x + sphere.radius * sin(phi) * cos(theta);
	double y = sphere.center.y + sphere.radius * sin(phi) * sin(theta);
	double z = sphere.center.z + sphere.radius * cos(phi);

	point3 random_point = point3(x, y, z);

	glm::dvec3 vector_to_random_point = random_point - origin;

	// If the point is on the far side of the sphere, move it to the near side
	if (glm::dot(vector_to_random_point, rec.normal) > 0.0) {
		random_point = random_point + (2.0 * sphere.radius) * (-rec.normal);
	}

	return random_point;
}