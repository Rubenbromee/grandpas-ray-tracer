#pragma once
#include "util.h"
#include "geometry.h"
double cosine_pdf(onb onb, const glm::dvec3& direction);
double intersectable_pdf(onb onb, point3 origin, glm::dvec3 search_direction, const std::vector<scene_object>& scene_objects);