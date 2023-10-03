#include "scene_util.h"
#include "util.h"
#include "glm.hpp"
#include "camera.h"

// Utility functions to add different scene objects to scenes
// Tight coupling between scene objects and materials comes back to haunt me here

void add_lambertian_quad_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object lambertian_quad = create_quad(top_left, top_right, bottom_left, bottom_right, LAMBERTIAN, color);

	if (x_rotation != 0.0) {
		rotate_quad_x(lambertian_quad, x_rotation);
	}
	if (y_rotation != 0.0) {
		rotate_quad_y(lambertian_quad, y_rotation);
	}
	if (z_rotation != 0.0) {
		rotate_quad_z(lambertian_quad, z_rotation);
	}

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

void add_dielectric_sphere_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, double refraction_index) {
	scene_object outer_sphere = create_sphere(center, radius, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_sphere = create_sphere(center, -(0.95 * radius), DIELECTRIC, {}, {}, refraction_index);

	scene_objects.push_back(outer_sphere);
	scene_objects.push_back(inner_sphere);
}

void add_lambertian_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_cube(center, size, LAMBERTIAN, color);

	if (x_rotation != 0.0) {
		rotate_cube_x(cube, x_rotation);
	}
	if (y_rotation != 0.0) {
		rotate_cube_y(cube, y_rotation);
	}
	if (z_rotation != 0.0) {
		rotate_cube_z(cube, z_rotation);
	}

	scene_objects.push_back(cube);
}

void add_metal_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, color color, double metal_fuzz, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_cube(center, size, METAL, color, metal_fuzz);

	if (x_rotation != 0.0) {
		rotate_cube_x(cube, x_rotation);
	}
	if (y_rotation != 0.0) {
		rotate_cube_y(cube, y_rotation);
	}
	if (z_rotation != 0.0) {
		rotate_cube_z(cube, z_rotation);
	}

	scene_objects.push_back(cube);
}

void add_dielectric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double size, double refraction_index, double x_rotation, double y_rotation, double z_rotation) {
	scene_object outer_cube = create_cube(center, size, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_cube = create_cube(center, -(0.95 * size), DIELECTRIC, {}, {}, refraction_index);

	if (x_rotation != 0.0) {
		rotate_cube_x(outer_cube, x_rotation);
		rotate_cube_x(inner_cube, x_rotation);
	}
	if (y_rotation != 0.0) {
		rotate_cube_y(outer_cube, y_rotation);
		rotate_cube_y(inner_cube, y_rotation);
	}
	if (z_rotation != 0.0) {
		rotate_cube_z(outer_cube, z_rotation);
		rotate_cube_z(inner_cube, z_rotation);
	}

	scene_objects.push_back(outer_cube);
	scene_objects.push_back(inner_cube);
}

void add_lambertian_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_asymmetric_cube(center, width, height, depth, LAMBERTIAN, color);

	if (x_rotation != 0.0) {
		rotate_cube_x(cube, x_rotation);
	}
	if (y_rotation != 0.0) {
		rotate_cube_y(cube, y_rotation);
	}
	if (z_rotation != 0.0) {
		rotate_cube_z(cube, z_rotation);
	}

	scene_objects.push_back(cube);
}

void add_metal_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, color color, double metal_fuzz, double x_rotation, double y_rotation, double z_rotation) {
	scene_object cube = create_asymmetric_cube(center, width, height, depth, METAL, color, metal_fuzz, {});

	if (x_rotation != 0.0) {
		rotate_cube_x(cube, x_rotation);
	}
	if (y_rotation != 0.0) {
		rotate_cube_y(cube, y_rotation);
	}
	if (z_rotation != 0.0) {
		rotate_cube_z(cube, z_rotation);
	}

	scene_objects.push_back(cube);
}

void add_dielectric_asymmetric_cube_to_scene(std::vector<scene_object>& scene_objects, point3 center, double width, double height, double depth, double refraction_index, double x_rotation, double y_rotation, double z_rotation) {
	scene_object outer_cube = create_asymmetric_cube(center, width, height, depth, DIELECTRIC, {}, {}, refraction_index);
	scene_object inner_cube = create_asymmetric_cube(center, (-0.95 * width), (-0.95 * height), (-0.95 * depth), DIELECTRIC, {}, {}, refraction_index);

	if (x_rotation != 0.0) {
		rotate_cube_x(outer_cube, x_rotation);
		rotate_cube_x(inner_cube, x_rotation);
	}
	if (y_rotation != 0.0) {
		rotate_cube_y(outer_cube, y_rotation);
		rotate_cube_y(inner_cube, y_rotation);
	}
	if (z_rotation != 0.0) {
		rotate_cube_z(outer_cube, z_rotation);
		rotate_cube_z(inner_cube, z_rotation);
	}

	scene_objects.push_back(outer_cube);
	scene_objects.push_back(inner_cube);
}

void add_quad_light_to_scene(std::vector<scene_object>& scene_objects, point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, color color) {
	scene_object quad_light = create_quad(top_left, top_right, bottom_left, bottom_right, LIGHT, color);
	scene_objects.push_back(quad_light);
}

void add_sphere_light_to_scene(std::vector<scene_object>& scene_objects, point3 center, double radius, color color) {
	scene_object sphere_light = create_sphere(center, radius, LIGHT, color);
	scene_objects.push_back(sphere_light);
}

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
	add_quad_light_to_scene(scene_objects, point3(-15.0, 49.9, -85.0), point3(15.0, 49.9, -85.0), point3(-15.0, 49.9, -115.0), point3(15.0, 49.9, -115.0), color(15.0, 15.0, 15.0)); // Light

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
	add_lambertian_cube_to_scene(scene_objects, point3(-1.0, 0.0, -1.0), 0.5, color(1.0, 0.0, 0.0), {}, {}, 45.0);
	add_lambertian_cube_to_scene(scene_objects, point3(0.0, 0.0, -1.0), 0.5, color(0.0, 1.0, 0.0), {}, 45.0, {});
	add_lambertian_cube_to_scene(scene_objects, point3(1.0, 0.0, -1.0), 0.5, color(0.0, 0.0, 1.0), 45.0, 45.0, 45.0);
}

// Populate scene with geometries, change which scene is rendered here
std::vector<scene_object> create_scene(camera& camera, color& background_color) {
	std::vector<scene_object> scene_objects = std::vector<scene_object>();
	create_scene_7(scene_objects, camera, background_color);
	return scene_objects;
}