#pragma once
#include "geometry.h"
#include "util.h"

void rotate_triangle_around_axis(triangle& triangle, double angle, point3 center, glm::dvec3 axis);

void rotate_triangle_x(triangle& triangle, double angle, point3 center);
void rotate_triangle_y(triangle& triangle, double angle, point3 center);
void rotate_triangle_z(triangle& triangle, double angle, point3 center);

void rotate_cube_x(scene_object& cube, double angle);
void rotate_cube_y(scene_object& cube, double angle);
void rotate_cube_z(scene_object& cube, double angle);

void rotate_quad_x(scene_object& quad, double angle);
void rotate_quad_y(scene_object& quad, double angle);
void rotate_quad_z(scene_object& quad, double angle);

void rotate_polygon(scene_object& polygon, double x_rotation, double y_rotation, double z_rotation);