#include "scene_util.h"
#include "util.h"
#include "glm.hpp"

// Utility functions to add different scene objects to scenes
// Tight coupling between scene objects and materials comes back to haunt me here

void add_lambertian_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color) {
	scene_object sphere = create_sphere(center, radius, LAMBERTIAN, color);
	scene_objects.push_back(sphere);
}

void add_metal_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color, double metal_fuzz) {
	scene_object sphere = create_sphere(center, radius, METAL, color, metal_fuzz);
	scene_objects.push_back(sphere);
}

void add_dielectric_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, double refraction_index) {
	scene_object outer_sphere = create_sphere(center, radius, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_sphere = create_sphere(center, -(0.8 * radius), DIELECTRIC, {}, {}, refraction_index);

	scene_objects.push_back(outer_sphere);
	scene_objects.push_back(inner_sphere);
}

void add_lambertian_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color) {
	scene_object cube = create_cube(center, size, LAMBERTIAN, color);
	scene_objects.push_back(cube);
}

void add_metal_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color, double metal_fuzz) {
	scene_object cube = create_cube(center, size, METAL, color, metal_fuzz);
	scene_objects.push_back(cube);
}

void add_dielectric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, double refraction_index) {
	scene_object outer_cube = create_cube(center, size, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_cube = create_cube(center, -(0.8 * size), DIELECTRIC, {}, {}, refraction_index);

	scene_objects.push_back(outer_cube);
	scene_objects.push_back(inner_cube);
}

// Utility functions to create different scenes

// Grey lambertian cube, dielectric sphere, yellow metal sphere and a blue lambertian sphere
// Default camera look from/at: Blue lambertian sphere will be seen through dielectric sphere
void create_scene_1(std::vector<scene_object>& scene_objects) {
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_dielectric_sphere_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.5, 1.5);
	add_lambertian_sphere_to_scene(scene_objects, point3(-0.5, 0.0, -3.0), 0.5, color(0.1, 0.2, 0.5));
	add_metal_sphere_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.5, color(0.8, 0.6, 0.2), 0.0);
	add_lambertian_cube_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.5);
}

// Brown-red lambertian sphere, dielectric cube, metal cube and a blue lambertian sphere
// Default camera look from/at: Blue lambertian sphere will be seen through dielectric cube
void create_scene_2(std::vector<scene_object>& scene_objects) {
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_lambertian_sphere_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.5, color(0.5, 0.2, 0.1));
	add_dielectric_cube_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.5, 1.5);
	add_lambertian_sphere_to_scene(scene_objects, point3(-0.5, 0.0, -3.0), 0.5, color(0.1, 0.2, 0.5));
	add_metal_cube_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.5, color(0.8, 0.6, 0.2), 0.0);
}

// A blue lambertian sphere and a red metal cube each inside a dielectric cube
// Default camera look from/at: Blue sphere to the left, red cube to the right
void create_scene_3(std::vector<scene_object>& scene_objects) {
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_lambertian_sphere_to_scene(scene_objects, point3(-1.0, 0.4, -1.5), 0.35, color(0.1, 0.2, 0.5));
	add_dielectric_cube_to_scene(scene_objects, point3(-1.0, 0.4, -1.5), 1.0, 1.5);
	add_metal_cube_to_scene(scene_objects, point3(1.0, 0.4, -1.5), 0.5, color(0.8, 0.1, 0.2), 0.0);
	add_dielectric_cube_to_scene(scene_objects, point3(1.0, 0.4, -1.5), 1.0, 1.5);
}

// Test scene for camera with adjustable field of view
// Default camera look from/at: Blue sphere to the left, red sphere to the right
void create_scene_4(std::vector<scene_object>& scene_objects) {
	double r = std::cos(pi / 4);
	add_lambertian_sphere_to_scene(scene_objects, point3(-r, 0.0, -1.0), r, color(0.0, 0.0, 1.0));
	add_lambertian_sphere_to_scene(scene_objects, point3(r, 0.0, -1.0), r, color(1.0, 0.0, 0.0));
}

// Simple test scene with dielectric, lambertian and metal sphere
// Default camera look from/at: Dielectric sphere to the left, lambertian sphere in the center and metal sphere to the right
void create_scene_5(std::vector<scene_object>& scene_objects) {
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_dielectric_sphere_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.5, 1.5);
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.5, color(0.1, 0.2, 0.5));
	add_metal_sphere_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.5, color(0.8, 0.6, 0.2), 0.0);
}

// Populate scene with geometries
std::vector<scene_object> create_scene_objects() {
	std::vector<scene_object> scene_objects = std::vector<scene_object>();
	create_scene_3(scene_objects);
	return scene_objects;
}