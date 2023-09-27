#pragma once
#include <vector>
#include "geometry.h"
#include "glm.hpp"
#include "util.h"
#include "camera.h"

void add_lambertian_quad_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color color = glm::dvec3(0.5, 0.5, 0.5));

void add_lambertian_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(0.5, 0.5, 0.5));
void add_metal_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0);
void add_dielectric_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, double refraction_index);

void add_lambertian_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(0.5, 0.5, 0.5));
void add_metal_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0);
void add_dielectric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, double refraction_index);

void add_quad_light_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color = glm::dvec3(4.0, 4.0, 4.0));
void add_sphere_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(4.0, 4.0, 4.0));

void create_scene_1(std::vector<scene_object>& scene_objects, camera& camera, color& background_color);
void create_scene_2(std::vector<scene_object>& scene_objects, camera& camera, color& background_color);
void create_scene_3(std::vector<scene_object>& scene_objects, camera& camera, color& background_color);
void create_scene_4(std::vector<scene_object>& scene_objects, camera& camera, color& background_color);
void create_scene_5(std::vector<scene_object>& scene_objects, camera& camera, color& background_color);
void create_scene_6(std::vector<scene_object>& scene_objects, camera& camera, color& background_color);
void create_scene_7(std::vector<scene_object>& scene_objects, camera& camera, color& background_color);

std::vector<scene_object> create_scene(camera& camera, color& background_color);