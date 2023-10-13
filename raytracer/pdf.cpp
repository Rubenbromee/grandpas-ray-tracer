#include <random>
#include <vector>
#include "pdf.h"
#include "geometry.h"
#include "glm.hpp"
#include "util.h"
#include "ray.h"
#include "geometry_util.h"

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