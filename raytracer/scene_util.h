#pragma once
#include <vector>
#include "geometry.h"
#include "glm.hpp"

void add_lambertian_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(0.5, 0.5, 0.5));
void add_metal_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0);
void add_dielectric_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, double refraction_index);

void add_lambertian_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(0.5, 0.5, 0.5));
void add_metal_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0);
void add_dielectric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, double refraction_index);

void create_scene_1(std::vector<scene_object>& scene_objects);
void create_scene_2(std::vector<scene_object>& scene_objects);
void create_scene_3(std::vector<scene_object>& scene_objects);
void create_scene_4(std::vector<scene_object>& scene_objects);

std::vector<scene_object> create_scene_objects();