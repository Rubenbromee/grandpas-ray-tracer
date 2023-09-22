#pragma once
#include "glm.hpp"
#include "util.h"

struct ray {
	point3 origin;
	glm::dvec3 direction;
};

ray create_ray(const point3& origin_, const glm::dvec3& direction_);
const point3& ray_at(ray ray, double time);
glm::dvec3 random_hemispherical_direction(const glm::dvec3& normal);
glm::dvec3 reflect(const glm::dvec3& vector, const glm::dvec3& normal);
glm::dvec3 refract(const glm::dvec3& unit_vector, const glm::dvec3& normal, double etai_over_etat);