#pragma once
#include "util.h"
#include "geometry.h"

enum pdf_enum {
	COSINE,
	INTERSECTABLE
};

double cosine_pdf(const glm::dvec3& normal, const glm::dvec3& random_direction);
void calculate_intersectable_pdf(const scene_object& light, const hit_record& rec, glm::dvec3& scattered_ray_direction, double& pdf, const std::vector<scene_object> scene_objects, int& nr_contributing_rays);
point3 get_random_point_on_quad(point3 origin, const scene_object& quad);
point3 get_random_point_on_cube(point3 origin, const scene_object& cube, const std::vector<scene_object> scene_objects);
point3 get_random_point_on_sphere(point3 origin, const scene_object& sphere, const hit_record& rec);