#pragma once
#include <vector>
#include "geometry.h"
#include "glm.hpp"
#include "util.h"
#include "camera.h"

void add_lambertian_quad_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color color = glm::dvec3(0.5, 0.5, 0.5), double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);

void add_lambertian_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(0.5, 0.5, 0.5));
void add_metal_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0);
void add_dielectric_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, double refraction_index, bool caustics = false);

void add_lambertian_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(0.5, 0.5, 0.5), double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);
void add_metal_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);
void add_dielectric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, double refraction_index, double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0, bool caustics = false);

void add_lambertian_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color = glm::dvec3(0.5, 0.5, 0.5), double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);
void add_metal_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);
void add_dielectric_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, double refraction_index = 1.0, double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0, bool caustics = false);

void add_quad_light_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color = glm::dvec3(4.0, 4.0, 4.0), double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);
void add_sphere_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(4.0, 4.0, 4.0));
void add_asymmetric_cube_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color = glm::dvec3(4.0, 4.0, 4.0), double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);
void add_cube_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(4.0, 4.0, 4.0), double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);

void add_spherical_constant_density_medium_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color = glm::dvec3(0.5, 0.5, 0.5), double density = 1.0);
void add_cubical_constant_density_medium_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color = glm::dvec3(0.5, 0.5, 0.5), double density = 1.0, double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);
void add_asymmetric_cubical_constant_density_medium_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color = glm::dvec3(0.5, 0.5, 0.5), double density = 1.0, double x_rotation = 0.0, double y_rotation = 0.0, double z_rotation = 0.0);