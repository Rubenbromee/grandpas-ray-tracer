#include "geometry_rotation.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

// Rotation functions for polygons

// Rotates a triangle around an arbitrary axis
void rotate_triangle_around_axis(triangle& triangle, double angle, point3 center, glm::dvec3 axis) {
	angle = glm::radians(angle); // Convert angle to radians

	// Create the rotation matrix for the x-axis
	glm::dmat4 rotation_matrix = glm::rotate(glm::dmat4(1.0), angle, axis);

	// Translate each vertex so that the triangle center would be in the world origin, rotate, then translate back
	for (int i = 0; i < 3; i++) {
		glm::dvec3 relative_vertex = triangle.vertices[i] - center;
		glm::dvec4 vertex = glm::dvec4(relative_vertex, 1.0);
		vertex = rotation_matrix * vertex;
		triangle.vertices[i] = center + glm::dvec3(vertex);
	}

	// Same procedure with normal as with vertices
	glm::dvec4 normal = glm::dvec4(triangle.normal, 1.0);
	normal = rotation_matrix * normal;
	triangle.normal = glm::dvec3(normal);
}

// Rotates a triangle counter-clockwise along the positive x-axis
void rotate_triangle_x(triangle& triangle, double angle, point3 center) {
	rotate_triangle_around_axis(triangle, angle, center, glm::dvec3(1.0, 0.0, 0.0));
}

// Rotates a triangle counter-clockwise along the positive y-axis
void rotate_triangle_y(triangle& triangle, double angle, point3 center) {
	rotate_triangle_around_axis(triangle, angle, center, glm::dvec3(0.0, 1.0, 0.0));
}

// Rotates a triangle counter-clockwise along the positive z-axis
void rotate_triangle_z(triangle& triangle, double angle, point3 center) {
	rotate_triangle_around_axis(triangle, angle, center, glm::dvec3(0.0, 0.0, 1.0));
}

// For all polygons, just rotate each triangle

void rotate_cube_x(scene_object& cube, double angle) {
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		rotate_triangle_x(cube.cube_triangles[i], angle, cube.cube_center);
	}
}

void rotate_cube_y(scene_object& cube, double angle) {
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		rotate_triangle_y(cube.cube_triangles[i], angle, cube.cube_center);
	}
}

void rotate_cube_z(scene_object& cube, double angle) {
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		rotate_triangle_z(cube.cube_triangles[i], angle, cube.cube_center);
	}
}

void rotate_quad_x(scene_object& quad, double angle) {
	for (int i = 0; i < quad.nr_quad_triangles; i++) {
		rotate_triangle_x(quad.quad_triangles[i], angle, point3(0.0, 0.0, 0.0));
	}
}

void rotate_quad_y(scene_object& quad, double angle) {
	for (int i = 0; i < quad.nr_quad_triangles; i++) {
		rotate_triangle_y(quad.quad_triangles[i], angle, point3(0.0, 0.0, 0.0));
	}
}

void rotate_quad_z(scene_object& quad, double angle) {
	for (int i = 0; i < quad.nr_quad_triangles; i++) {
		rotate_triangle_z(quad.quad_triangles[i], angle, point3(0.0, 0.0, 0.0));
	}
}

// Utility function to rotate a polygon if it has any rotation
void rotate_polygon(scene_object& polygon, double x_rotation, double y_rotation, double z_rotation) {
	switch (polygon.object_type) {
	case QUAD:
		if (x_rotation != 0.0) {
			rotate_quad_x(polygon, x_rotation);
		}
		if (y_rotation != 0.0) {
			rotate_quad_y(polygon, y_rotation);
		}
		if (z_rotation != 0.0) {
			rotate_quad_z(polygon, z_rotation);
		}
		break;
	case CUBE:
		if (x_rotation != 0.0) {
			rotate_cube_x(polygon, x_rotation);
		}
		if (y_rotation != 0.0) {
			rotate_cube_y(polygon, y_rotation);
		}
		if (z_rotation != 0.0) {
			rotate_cube_z(polygon, z_rotation);
		}
		break;
	case ASYMMETRIC_CUBE:
		if (x_rotation != 0.0) {
			rotate_cube_x(polygon, x_rotation);
		}
		if (y_rotation != 0.0) {
			rotate_cube_y(polygon, y_rotation);
		}
		if (z_rotation != 0.0) {
			rotate_cube_z(polygon, z_rotation);
		}
		break;
	}
}