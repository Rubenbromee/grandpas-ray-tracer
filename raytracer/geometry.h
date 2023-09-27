#pragma once
#include <map>
#include <vector>
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
	double shininess;
	double refraction_index;
	double time;
	bool outward_face;
};

// Triangles have counter-clockwise/right-hand rule, for normals
struct triangle {
	glm::dvec3 vertices[3];
	glm::dvec3 normal;
};

// Enum for an intersectable object
enum object_enum {
	SPHERE,
	CUBE,
	QUAD,
};

// Union type for all scene objects :)
struct scene_object {
	// Enum to determine object type
	object_enum object_type;

	// Light fields, diffuse light
	// Any geometry can be a light
	color diffuse_light_color;

	// Material properties, defaults to grey lambertian geometry
	material_enum material;
	color material_color;
	double metal_fuzz;
	double shininess;
	double refraction_index;

	// Quad fields, polygon surface
	size_t nr_quad_triangles = 2;
	triangle quad_triangles[2];

	// Sphere fields, implicit surface
	glm::dvec3 center;
	double radius;

	// Cube fields, polygon surface
	size_t nr_cube_triangles = 12;
	triangle cube_triangles[12];
};

scene_object create_quad(point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0, double shininess = 1.0);
scene_object create_sphere(point3 center, double radius, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0, double shininess = 1.0);
scene_object create_cube(point3 center, double size, material_enum material = LAMBERTIAN, color color = glm::dvec3(0.5, 0.5, 0.5), double metal_fuzz = 1.0, double refraction_index = 1.0, double shininess = 1.0); // Cubes are symmetrical and have no rotation

void set_face_normal(const ray& ray, const glm::dvec3& outward_normal, hit_record& rec);
bool sphere_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& sphere);
bool triangle_intersection(const ray& ray, interval ray_time, hit_record& rec, const triangle& triangle);
bool quad_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& quad);
bool cube_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& cube);

void update_hit_record(hit_record& temp_rec, const scene_object& obj, hit_record& rec);
bool find_intersection(const ray& ray, interval initial_ray_time_interval, hit_record& rec, const std::vector<scene_object>& scene_objects);

std::ostream& print_triangle(std::ostream& os, triangle triangle);
std::ostream& print_cube(std::ostream& os, scene_object cube, point3 cube_center, double cube_size);