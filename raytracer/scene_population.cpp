#include <memory>
#include "geometry_rotation.h"
#include "util.h"
#include "glm.hpp"
#include "camera.h"

// Utility functions to add different scene objects to scenes
// Tight coupling between scene objects and materials comes back to haunt me here

void add_lambertian_quad_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object lambertian_quad = create_quad(top_left, top_right, bottom_left, bottom_right, LAMBERTIAN, color);
	rotate_polygon(lambertian_quad, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(lambertian_quad);
}

void add_lambertian_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color) {
	scene_object sphere = create_sphere(center, radius, LAMBERTIAN, color);
	scene_objects.push_back(sphere);
}

void add_metal_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color, double metal_fuzz) {
	scene_object sphere = create_sphere(center, radius, METAL, color, metal_fuzz);
	scene_objects.push_back(sphere);
}

void add_dielectric_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, double refraction_index, bool caustics) {
	scene_object outer_sphere = create_sphere(center, radius, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_sphere = create_sphere(center, -(0.95 * radius), DIELECTRIC, {}, {}, refraction_index);

	scene_objects.push_back(outer_sphere);

	if (!caustics) {
		scene_objects.push_back(inner_sphere);
	}
}

void add_lambertian_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_cube(center, size, LAMBERTIAN, color);
	rotate_polygon(cube, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(cube);
}

void add_metal_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color, double metal_fuzz, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_cube(center, size, METAL, color, metal_fuzz);
	rotate_polygon(cube, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(cube);
}

void add_dielectric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, double refraction_index, double x_rotation, double y_rotation, double z_rotation, bool caustics) {
	scene_object outer_cube = create_cube(center, size, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_cube = create_cube(center, -(0.95 * size), DIELECTRIC, {}, {}, refraction_index);

	rotate_polygon(outer_cube, x_rotation, y_rotation, z_rotation);
	rotate_polygon(inner_cube, x_rotation, y_rotation, z_rotation);

	scene_objects.push_back(outer_cube);

	if (!caustics) {
		scene_objects.push_back(inner_cube);
	}
}

void add_lambertian_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_asymmetric_cube(center, width, height, depth, LAMBERTIAN, color);
	rotate_polygon(cube, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(cube);
}

void add_metal_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color, double metal_fuzz, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_asymmetric_cube(center, width, height, depth, METAL, color, metal_fuzz, {});
	rotate_polygon(cube, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(cube);
}

void add_dielectric_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, double refraction_index, double x_rotation, double y_rotation, double z_rotation, bool caustics) {
	scene_object outer_cube = create_asymmetric_cube(center, width, height, depth, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_cube = create_asymmetric_cube(center, (-0.95 * width), (-0.95 * height), (-0.95 * depth), DIELECTRIC, {}, {}, refraction_index);

	rotate_polygon(outer_cube, x_rotation, y_rotation, z_rotation);
	rotate_polygon(inner_cube, x_rotation, y_rotation, z_rotation);

	scene_objects.push_back(outer_cube);

	if (!caustics) {
		scene_objects.push_back(inner_cube);
	}
}

void add_quad_light_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object quad_light = create_quad(top_left, top_right, bottom_left, bottom_right, LIGHT, color);
	rotate_polygon(quad_light, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(quad_light);
}

void add_sphere_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color) {
	scene_object sphere_light = create_sphere(center, radius, LIGHT, color);
	scene_objects.push_back(sphere_light);
}

void add_cube_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube_light = create_cube(center, size, LIGHT, color);
	rotate_polygon(cube_light, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(cube_light);
}

void add_asymmetric_cube_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object asymmetric_cube_light = create_asymmetric_cube(center, width, height, depth, LIGHT, color);
	rotate_polygon(asymmetric_cube_light, x_rotation, y_rotation, z_rotation);
	scene_objects.push_back(asymmetric_cube_light);
}

void add_spherical_constant_density_medium_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color, double density) {
	scene_object sphere = create_sphere(center, radius, CONSTANT_DENSITY_MEDIUM_MATERIAL, color);
	std::shared_ptr<scene_object> sphere_ = std::make_shared<scene_object>(sphere);

	scene_object constant_density_medium = create_constant_density_medium(sphere_, density, color);

	scene_objects.push_back(constant_density_medium);
}

void add_quad_constant_density_medium_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color color, double density, double x_rotation, double y_rotation, double z_rotation) {
	scene_object quad = create_quad(top_left, top_right, bottom_left, bottom_right, CONSTANT_DENSITY_MEDIUM_MATERIAL, color);

	rotate_polygon(quad, x_rotation, y_rotation, z_rotation);

	std::shared_ptr<scene_object> quad_ = std::make_shared<scene_object>(quad);
	scene_object constant_density_medium = create_constant_density_medium(quad_, density, color);

	scene_objects.push_back(constant_density_medium);
}

void add_cubical_constant_density_medium_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color, double density, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_cube(center, size, CONSTANT_DENSITY_MEDIUM_MATERIAL, color);

	// rotate_polygon(cube, x_rotation, y_rotation, z_rotation);

	std::shared_ptr<scene_object> cube_ = std::make_shared<scene_object>(cube);
	scene_object constant_density_medium = create_constant_density_medium(cube_, density, color);

	scene_objects.push_back(constant_density_medium);
}

void add_asymmetric_cubical_constant_density_medium_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color, double density, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_asymmetric_cube(center, width, height, depth, CONSTANT_DENSITY_MEDIUM_MATERIAL, color);

	rotate_polygon(cube, x_rotation, y_rotation, z_rotation);

	std::shared_ptr<scene_object> cube_ = std::make_shared<scene_object>(cube);
	scene_object constant_density_medium = create_constant_density_medium(cube_, density, color);

	scene_objects.push_back(constant_density_medium);
}