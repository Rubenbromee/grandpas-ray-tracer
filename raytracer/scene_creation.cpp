#include <memory>
#include "scene_population.h"
#include "geometry_rotation.h"
#include "util.h"
#include "glm.hpp"
#include "camera.h"
#include "scene_population.h"

// This file is for creating different scenes, there exists utility functions to populate the scene with geometries
// You can also modify camera properties, see the existing functions to get a grip of the pattern
// Samples per pixel and max depth can also be modified through the camera object

// Utility functions to create different scenes

// Grey lambertian cube, dielectric sphere, yellow metal sphere and a blue lambertian sphere
// Blue lambertian sphere will be seen through dielectric sphere
void create_scene_1(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 16.0 / 9.0;
	camera.look_from = point3(0.0, 0.0, 0.0);
	camera.look_at = point3(0.0, 0.0, -1.0);
	camera.vertical_field_of_view = 120.0;
	background_color = color(0.70, 0.80, 1.00); // "Sky" background

	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_dielectric_sphere_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.5, 1.5);
	add_lambertian_sphere_to_scene(scene_objects, point3(-0.5, 0.0, -3.0), 0.5, color(0.1, 0.2, 0.5));
	add_metal_sphere_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.5, color(0.8, 0.6, 0.2), 0.0);
	add_lambertian_cube_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.5);
}

// Brown-red lambertian sphere, dielectric cube, metal cube and a blue lambertian sphere
// Default camera look from/at: Blue lambertian sphere will be seen through dielectric cube
void create_scene_2(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 16.0 / 9.0;
	camera.look_from = point3(0.0, 0.0, 0.0);
	camera.look_at = point3(0.0, 0.0, -1.0);
	camera.vertical_field_of_view = 120.0;
	background_color = color(0.70, 0.80, 1.00); // "Sky" background
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_lambertian_sphere_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.5, color(0.5, 0.2, 0.1));
	add_dielectric_cube_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.5, 1.5);
	add_lambertian_sphere_to_scene(scene_objects, point3(-0.5, 0.0, -3.0), 0.5, color(0.1, 0.2, 0.5));
	add_metal_cube_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.5, color(0.8, 0.6, 0.2), 0.0);
}

// A blue lambertian sphere and a red metal cube each inside a dielectric cube
// Default camera look from/at: Blue sphere to the left, red cube to the right
void create_scene_3(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 16.0 / 9.0;
	camera.look_from = point3(0.0, 0.0, 0.0);
	camera.look_at = point3(0.0, 0.0, -1.0);
	camera.vertical_field_of_view = 120.0;
	background_color = color(0.70, 0.80, 1.00); // "Sky" background
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_lambertian_sphere_to_scene(scene_objects, point3(-1.0, 0.4, -1.5), 0.35, color(0.1, 0.2, 0.5));
	add_dielectric_cube_to_scene(scene_objects, point3(-1.0, 0.4, -1.5), 1.0, 1.5);
	add_metal_cube_to_scene(scene_objects, point3(1.0, 0.4, -1.5), 0.5, color(0.8, 0.1, 0.2), 0.0);
	add_dielectric_cube_to_scene(scene_objects, point3(1.0, 0.4, -1.5), 1.0, 1.5);
}

// Test scene for camera with adjustable field of view
// Default camera look from/at: Blue sphere to the left, red sphere to the right
void create_scene_4(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 16.0 / 9.0;
	camera.look_from = point3(0.0, 0.0, 0.0);
	camera.look_at = point3(0.0, 0.0, -1.0);
	camera.vertical_field_of_view = 120.0;
	background_color = color(0.70, 0.80, 1.00); // "Sky" background
	double r = std::cos(pi / 4);
	add_lambertian_sphere_to_scene(scene_objects, point3(-r, 0.0, -1.0), r, color(0.0, 0.0, 1.0));
	add_lambertian_sphere_to_scene(scene_objects, point3(r, 0.0, -1.0), r, color(1.0, 0.0, 0.0));
}

// Simple test scene with dielectric, lambertian and metal sphere
// Default camera look from/at: Dielectric sphere to the left, lambertian sphere in the center and metal sphere to the right
void create_scene_5(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 16.0 / 9.0;
	camera.look_from = point3(0.0, 0.0, 0.0);
	camera.look_at = point3(0.0, 0.0, -1.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.70, 0.80, 1.00); // "Sky" background
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_dielectric_sphere_to_scene(scene_objects, point3(-1.0, 0.0, -1.2), 0.5, 1.5);
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, 0.0, -1.2), 0.5, color(0.1, 0.2, 0.5));
	add_metal_sphere_to_scene(scene_objects, point3(1.0, 0.0, -1.2), 0.5, color(0.8, 0.6, 0.2), 0.0);
}

// Defocus blur scene
void create_scene_6(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 16.0 / 9.0;
	// View from upper-back-left corner
	camera.look_from = point3(-3.5, 3.5, 2.5); 
	camera.look_at = point3(0, 0, -1);
	camera.camera_up = glm::dvec3(0, 1, 0);
	camera.vertical_field_of_view = 20.0; // Zoom in
	// Tight defocus angle and short defocus distance
	camera.defocus_angle = 10.0;
	camera.focus_distance = glm::distance(camera.look_from, camera.look_at);
	background_color = color(0.70, 0.80, 1.00); // "Sky" background

	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_dielectric_sphere_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.5, 1.5);
	add_lambertian_sphere_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.5, color(0.1, 0.2, 0.5));
	add_metal_sphere_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.5, color(0.8, 0.6, 0.2), 0.0);
}

// Default cornell box with two cubes
void create_scene_7(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(8.0, 8.0, 8.0)); // Light

	// Objects
	add_lambertian_asymmetric_cube_to_scene(scene_objects, point3(-20.0, -15.0, -115.0), 30.0, 70.0, 30.0, color(0.5, 0.5, 0.5), {}, 30.0, {});
	add_lambertian_cube_to_scene(scene_objects, point3(20.0, -35.0, -80.0), 30.0, color(0.5, 0.5, 0.5), {}, -30.0, {});
}

// Cornell box with more objects and a blue wall behind the camera
void create_scene_8(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -51.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -50.0), point3(50.0, -50.0, -50.0), point3(-50.0, -50.0, -50.0), color(0.05, 0.05, 0.65)); // Wall behind camera
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(30.0, 30.0, 30.0)); // Light

	// Objects
	add_lambertian_asymmetric_cube_to_scene(scene_objects, point3(-20.0, -15.0, -115.0), 30.0, 70.0, 30.0, color(0.5, 0.5, 0.5), {}, 10.0, {});
	add_lambertian_cube_to_scene(scene_objects, point3(20.0, -35.0, -80.0), 30.0, color(0.5, 0.5, 0.5), {}, -10.0, {});
	add_dielectric_sphere_to_scene(scene_objects, point3(20.0, 0.0, -85.0), 20.0, 1.5);
	add_dielectric_asymmetric_cube_to_scene(scene_objects, point3(-20.0, 15.0, -80.0), 15.0, 30.0, 15.0, 1.5);
	add_lambertian_asymmetric_cube_to_scene(scene_objects, point3(20.0, 0.0, -115.0), 10.0, 25.0, 10.0, color(173.0 / 255.0, 216.0 / 255.0, 230.0 / 255.0), 45.0, 45.0, 45.0);
	add_metal_sphere_to_scene(scene_objects, point3(-20, -25.0, -80.0), 15.0, color(255.0 / 255.0, 165.0 / 255.0, 0.0), 0.0);
}

// Scene to test different rotations
void create_scene_9(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 16.0 / 9.0;
	// View from upper-back-left corner
	camera.look_from = point3(-3.5, 3.5, 2.5);
	camera.look_at = point3(0, 0, -1);
	camera.camera_up = glm::dvec3(0, 1, 0);
	camera.vertical_field_of_view = 20.0;
	background_color = color(0.70, 0.80, 1.00); // "Sky" background

	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, -100.5, -1.0), 100.0, color(0.8, 0.8, 0.0)); // "Ground"
	add_lambertian_asymmetric_cube_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.25, 0.5, 0.25, color(0.5, 0.5, 0.5), {}, {}, 45.0);
	add_lambertian_asymmetric_cube_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.25, 0.5, 0.25, color(0.5, 0.5, 0.5), -180.0, {}, {});
	add_lambertian_asymmetric_cube_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.25, 0.5, 0.25, color(0.5, 0.5, 0.5), 45.0, 45.0, 45.0);
}

// Cornell box with dielectric sphere on ground to showcase basic caustics
void create_scene_10(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(8.0, 8.0, 8.0)); // Light
	
	add_dielectric_sphere_to_scene(scene_objects, point3(0.0, -30.0, -100.0), 20.0, 1.5, true);
}

// Cornell box with multiple light geometries
void create_scene_11(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	
	// Lights
	add_sphere_light_to_scene(scene_objects, point3(0.0, 30.0, -100.0), 10.0, color(0.0, 10.0, 0.0));
	add_asymmetric_cube_light_to_scene(scene_objects, point3(-30.0, -15.0, -85.0), 5.0, 15.0, 5.0, color(10.0, 0.0, 0.0), 45.0, 45.0, 45.0);
	add_cube_light_to_scene(scene_objects, point3(30.0, -15.0, -85.0), 10.0, color(0.0, 0.0, 10.0));
	add_sphere_light_to_scene(scene_objects, point3(0.0, -25.0, -85.0), 7.5, color(10.0, 10.0, 10.0));
	add_lambertian_sphere_to_scene(scene_objects, point3(0.0, 0.0, -100.0), 15.0, color(0.5, 0.5, 0.5));
}

// Cornell box with lambertian asymmetric cube and dielectric sphere with caustics
void create_scene_12(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(15.0, 15.0, 15.0)); // Light

	add_lambertian_asymmetric_cube_to_scene(scene_objects, point3(-20.0, -15.0, -115.0), 30.0, 70.0, 30.0, color(0.5, 0.5, 0.5), {}, 30.0, {});
	add_dielectric_sphere_to_scene(scene_objects, point3(10.0, -29.0, -85.0), 20.0, 1.5, true);
}

// Cornell box with multiple colored caustics
void create_scene_13(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(8.0, 8.0, 8.0)); // Light

	add_sphere_light_to_scene(scene_objects, point3(-12.5, -2.5, -97.5), 5.0, color(10.0, 0.0, 0.0));
	add_sphere_light_to_scene(scene_objects, point3(0.0, -2.5, -82.5), 5.0, color(0.0, 10.0, 0.0));
	add_sphere_light_to_scene(scene_objects, point3(12.5, -2.5, -97.5), 5.0, color(0.0, 0.0, 10.0));

	add_dielectric_sphere_to_scene(scene_objects, point3(0.0, -29.0, -85.0), 20.0, 1.5, true);
}

// Cornell box with rotated dielectric asymmetric cube with caustics
void create_scene_14(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(15.0, 15.0, 15.0)); // Light

	add_dielectric_cube_to_scene(scene_objects, point3(-20.0, -20.0, -100.0), 20.0, 1.5, 45.0, 45.0, 45.0, true);
	add_dielectric_asymmetric_cube_to_scene(scene_objects, point3(20.0, -20.0, -100.0), 10.0, 25.0, 10.0, 1.5, 45.0, 45.0, 45.0, true);
}

// Intersecting constant density mediums
void create_scene_15(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(8.0, 8.0, 8.0)); // Light
	
	add_spherical_constant_density_medium_to_scene(scene_objects, point3(-15.0, 0.0, -100.0), 20.0, color(1.0, 0.0, 0.0), 0.03);
	add_spherical_constant_density_medium_to_scene(scene_objects, point3(0.0, 0.0, -100.0), 20.0, color(0.0, 1.0, 0.0), 0.03);
	add_spherical_constant_density_medium_to_scene(scene_objects, point3(15.0, 0.0, -100.0), 20.0, color(0.0, 0.0, 1.0), 0.03);
}

// Constant density medium and light inside a caustic object
void create_scene_16(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(15.0, 15.0, 15.0)); // Light

	add_sphere_light_to_scene(scene_objects, point3(0.0, -15.0, -100.0), 15.0, color(0.2, 0.9961, 1.0));
	add_spherical_constant_density_medium_to_scene(scene_objects, point3(0.0, -15.0, -100.0), 17.5, color(0.2, 0.9961, 1.0), 0.02);
	add_dielectric_sphere_to_scene(scene_objects, point3(0.0, -15.0, -100.0), 20.0, 1.5, true);
}

// Intersecting constant density medium quads, symmetric and asymmetric cubical constant density mediums
void create_scene_17(std::vector<scene_object>& scene_objects, camera& camera, color& background_color) {
	camera.aspect_ratio = 1.0;
	camera.look_from = point3(0.0, 0.0, -1.0);
	camera.look_at = point3(0.0, 0.0, -100.0);
	camera.vertical_field_of_view = 90.0;
	background_color = color(0.0, 0.0, 0.0);

	// Room
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(50.0, -50.0, -50.0), color(0.73, 0.73, 0.73)); // Floor
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), point3(-50.0, -50.0, -150.0), point3(50.0, -50.0, -150.0), color(0.73, 0.73, 0.73)); // Back wall
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(50.0, 50.0, -150.0), color(0.73, 0.73, 0.73)); // Ceiling
	add_lambertian_quad_to_scene(scene_objects, point3(-50.0, 50.0, -50.0), point3(-50.0, 50.0, -150.0), point3(-50.0, -50.0, -50.0), point3(-50.0, -50.0, -150.0), color(0.12, 0.45, 0.15)); // Right wall (green)
	add_lambertian_quad_to_scene(scene_objects, point3(50.0, 50.0, -150.0), point3(50.0, 50.0, -50.0), point3(50.0, -50.0, -150.0), point3(50.0, -50.0, -50.0), color(0.65, 0.05, 0.05)); // Left wall (red)
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(15.0, 15.0, 15.0)); // Light

	// add_quad_constant_density_medium_to_scene(scene_objects, point3(-10.0, 10.0, -100.0), point3(10.0, 10.0, -100.0), point3(-10.0, -10.0, -100.0), point3(10.0, -10.0, -100.0), color(1.0, 0.0, 0.0), 0.03);
	// add_quad_constant_density_medium_to_scene(scene_objects, point3(-10.0, 10.0, -100.0), point3(10.0, 10.0, -100.0), point3(-10.0, -10.0, -100.0), point3(10.0, -10.0, -100.0), color(0.0, 0.0, 1.0), 0.03, 45.0, -45.0);
	add_cubical_constant_density_medium_to_scene(scene_objects, point3(0.0, 25.0, -100.0), 15.0, color(1.0, 0.4980, 0.3137), 0.03);
	// add_lambertian_cube_to_scene(scene_objects, point3(0.0, 25.0, -100.0), 15.0, color(1.0, 0.4980, 0.3137));
	// add_asymmetric_cubical_constant_density_medium_to_scene(scene_objects, point3(0.0, -25.0, -100.0), 5.0, 15.0, 5.0, color(1.0, 0.4118, 0.7059), 0.03, {}, {}, 90.0);
}

// Populate scene with geometries, change which scene is rendered here
std::vector<scene_object> create_scene(camera& camera, color& background_color) {
	std::vector<scene_object> scene_objects = std::vector<scene_object>();
	create_scene_17(scene_objects, camera, background_color);
	return scene_objects;
}