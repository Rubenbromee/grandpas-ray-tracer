#pragma once
#include "geometry.h"
#include "util.h"

point3 calculate_triangle_center(const triangle& triangle);
void set_face_normal(const ray& ray, const glm::dvec3& outward_normal, hit_record& rec);
void update_hit_record(hit_record& temp_rec, const scene_object& obj, hit_record& rec);

double calculate_triangle_area(const triangle& triangle);
double calculate_cube_area(const scene_object& cube);
double calculate_quad_area(const scene_object& quad);

bool sphere_contains_point(const scene_object& sphere, const point3& point);
bool quad_contains_point(const scene_object& quad, const point3& point);
bool cube_contains_point(const scene_object& cube, const point3& point);

point3 get_random_point_on_sphere(point3 origin, const scene_object& sphere, const hit_record& rec);
point3 get_random_point_on_triangle(const triangle& random_triangle);
point3 get_random_point_on_quad(point3 origin, const scene_object& quad);
point3 get_random_point_on_cube(point3 origin, const scene_object& cube, const std::vector<scene_object>& scene_objects);

std::ostream& print_triangle(std::ostream& os, triangle triangle);
std::ostream& print_cube(std::ostream& os, scene_object cube, point3 cube_center, double cube_size);
std::ostream& print_asymmetric_cube(std::ostream& os, scene_object cube, point3 cube_center);