#pragma once
#include "util.h"
#include "geometry.h"

double cosine_pdf(const glm::dvec3& normal, const glm::dvec3& random_direction);
double intersectable_pdf(point3 origin, glm::dvec3 sample_object_direction, const scene_object& sample_object, const hit_record& rec, const point3 random_point_on_sample_object);
void calculate_intersectable_pdf(const scene_object& sample_object, const hit_record& rec, glm::dvec3& scattered_ray_direction, double& pdf, const std::vector<scene_object>& scene_objects);