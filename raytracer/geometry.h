#pragma once
#include <map>
#include <vector>
#include <memory>
#include "ray.h"
#include "glm.hpp"
#include "material.h"
#include "util.h"

// Hit record to track rays
struct hit_record {
	point3 point;
	glm::dvec3 normal;
	material_enum material;
	color material_color;
	double metal_fuzz;
	double refraction_index;
	double time;
	bool outward_face;
};

// Triangles have counter-clockwise/right-hand rule, for normals
struct triangle {
	glm::dvec3 vertices[3];
	glm::dvec3 normal;
	point3 center;
};

// Enum for an intersectable object
enum object_enum {
	SPHERE,
	CUBE,
	QUAD,
	ASYMMETRIC_CUBE, 
	CONSTANT_DENSITY_MEDIUM
};

// Union type for all scene objects
struct scene_object {
	// Enum to determine object type
	object_enum object_type;

	// Material properties, defaults to grey lambertian geometry
	material_enum material;
	color material_color;
	double metal_fuzz;
	double refraction_index;

	// Sphere fields, implicit surface
	glm::dvec3 sphere_center;
	double sphere_radius;
	double sphere_area;

	// Quad fields, polygon surface
	int nr_quad_triangles = 2;
	triangle quad_triangles[2];
	point3 quad_center;
	double quad_area;

	// Cube fields, polygon surface
	int nr_cube_triangles = 12;
	triangle cube_triangles[12];
	point3 cube_center;
	double cube_area;

	// Constant density medium fields, probabilistic density
	double density;
	std::shared_ptr<scene_object> boundrary_volume;
};

// Geometry creation functions
scene_object create_sphere(point3 center, double radius, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0);
scene_object create_quad(point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0);
scene_object create_quad(point3 center, double width, double height, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0);
scene_object create_cube(point3 center, double size, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0);
scene_object create_asymmetric_cube(point3 center, double width, double height, double depth, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0);
scene_object create_constant_density_medium(std::shared_ptr<scene_object> boundrary_volume, double density = 0.01, color color = glm::dvec3(0.5, 0.5, 0.5));

// Geometry intersection functions
bool sphere_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& sphere);
bool triangle_intersection(const ray& ray, interval ray_time, hit_record& rec, const triangle& triangle);
bool quad_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& quad);
bool cube_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& cube);
bool constant_density_medium_intersection(const ray& ray_in, interval ray_time, hit_record& rec, const scene_object constant_density_medium);

// Scene intersection function
bool find_intersection(const ray& ray, interval initial_ray_time_interval, hit_record& rec, const std::vector<scene_object>& scene_objects);