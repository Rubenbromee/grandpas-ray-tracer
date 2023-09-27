#include <iostream>
#include "geometry.h"
#include "glm.hpp"
#include "util.h"
#include "material.h"
#include "scene_util.h"

point3 calculate_triangle_center(const triangle& triangle) {
	// Calculate the average of the vertices' coordinates
	point3 center = (triangle.vertices[0] + triangle.vertices[1] + triangle.vertices[2]) / 3.0;

	return center;
}

// A quad is just made up of two triangles
scene_object create_quad(point3 top_left, point3 top_right, point3 bottom_left, point3 bottom_right, material_enum material, color color, double metal_fuzz, double refraction_index, double shininess) {
	scene_object quad;
	quad.object_type = QUAD;

	quad.material = material;
	quad.material_color = color;
	quad.metal_fuzz = metal_fuzz;
	quad.refraction_index = refraction_index;
	quad.shininess = shininess;

	// Define the vertices of the quad
	triangle triangles[2];

	// First triangle (top-left, top-right, bottom-right)
	triangles[0].vertices[0] = top_right;
	triangles[0].vertices[1] = top_left;
	triangles[0].vertices[2] = bottom_left;

	// Second triangle (top-left, bottom-right, bottom-left)
	triangles[1].vertices[0] = top_right;
	triangles[1].vertices[1] = bottom_left;
	triangles[1].vertices[2] = bottom_right;

	// Calculate the normals for the triangles with the given vertex order
	triangles[0].normal = glm::normalize(glm::cross(triangles[0].vertices[2] - triangles[0].vertices[1], triangles[0].vertices[0] - triangles[0].vertices[1]));
	triangles[1].normal = glm::normalize(glm::cross(triangles[1].vertices[0] - triangles[1].vertices[2], triangles[1].vertices[1] - triangles[1].vertices[2]));

	// Assign the triangles to the quad
	for (size_t i = 0; i < 2; i++) {
		quad.quad_triangles[i] = triangles[i];
		quad.quad_triangles[i].center = calculate_triangle_center(quad.quad_triangles[i]);
	}

	return quad;
}

scene_object create_quad(point3 center, double width, double height, material_enum material, color color, double metal_fuzz, double refraction_index, double shininess) {
	scene_object quad;
	quad.object_type = QUAD;

	quad.material = material;
	quad.material_color = color;
	quad.metal_fuzz = metal_fuzz;
	quad.refraction_index = refraction_index;
	quad.shininess = shininess;

	// Calculate half-width and half-height
	double half_width = width * 0.5;
	double half_height = height * 0.5;

	// Define the vertices of the quad based on center, width, and height
	point3 top_left = center + point3(-half_width, half_height, 0.0);
	point3 top_right = center + point3(half_width, half_height, 0.0);
	point3 bottom_left = center + point3(-half_width, -half_height, 0.0);
	point3 bottom_right = center + point3(half_width, -half_height, 0.0);

	triangle triangles[2];

	// First triangle (top-left, top-right, bottom-right)
	triangles[0].vertices[0] = top_right;
	triangles[0].vertices[1] = top_left;
	triangles[0].vertices[2] = bottom_left;

	// Second triangle (top-left, bottom-right, bottom-left)
	triangles[1].vertices[0] = top_right;
	triangles[1].vertices[1] = bottom_left;
	triangles[1].vertices[2] = bottom_right;

	// Calculate the normals for the triangles with the given vertex order
	triangles[0].normal = glm::normalize(glm::cross(triangles[0].vertices[2] - triangles[0].vertices[1], triangles[0].vertices[0] - triangles[0].vertices[1]));
	triangles[1].normal = glm::normalize(glm::cross(triangles[1].vertices[0] - triangles[1].vertices[2], triangles[1].vertices[1] - triangles[1].vertices[2]));

	// Assign the triangles to the quad
	for (size_t i = 0; i < 2; i++) {
		quad.quad_triangles[i] = triangles[i];
		quad.quad_triangles[i].center = calculate_triangle_center(quad.quad_triangles[i]);
	}

	return quad;
}

// A sphere is an implicit surface
scene_object create_sphere(point3 center, double radius, material_enum material, color color, double metal_fuzz, double refraction_index, double shininess) {
	scene_object sphere;

	// Geometry properties
	sphere.object_type = SPHERE;
	sphere.center = center;
	sphere.radius = radius;

	// Material properties
	sphere.material = material;
	sphere.material_color = color;
	sphere.metal_fuzz = metal_fuzz;
	sphere.refraction_index = refraction_index;
	sphere.shininess = shininess;

	return sphere;
}



// A cube is defined by 12 triangles
// Front face towards camera
scene_object create_cube(point3 center, double size, material_enum material, color color, double metal_fuzz, double refraction_index, double shininess) {
	scene_object cube;

	// Geometric properties
	cube.object_type = CUBE;

	double half = size * 0.5;

	// Define cube vertices
	const glm::dvec3 vertices[] = {
		// Front face
		center + glm::dvec3(-half, -half, half),   // Vertex 0 (Front bottom left)
		center + glm::dvec3(half, -half, half),    // Vertex 1 (Front bottom right)
		center + glm::dvec3(-half, half, half),    // Vertex 2 (Front top left)
		center + glm::dvec3(half, half, half),     // Vertex 3 (Front top right)

		// Back face
		center + glm::dvec3(-half, -half, -half), // Vertex 4 (Back bottom left)
		center + glm::dvec3(half, -half, -half),  // Vertex 5 (Back bottom right)
		center + glm::dvec3(-half, half, -half),  // Vertex 6 (Back top left)
		center + glm::dvec3(half, half, -half)    // Vertex 7 (Back top right)
	};

	// Define indices for triangles
	const int indices[] = {
		0, 1, 2,  2, 1, 3,  // Front face
		4, 6, 5,  5, 6, 7,  // Back face
		2, 3, 6,  6, 3, 7,  // Top face
		0, 4, 1,  1, 4, 5,  // Bottom face
		0, 2, 4,  4, 2, 6,  // Left face
		1, 5, 3,  3, 5, 7   // Right face
	};

	// Assign vertices and normal for each triangle in the cube
	for (size_t i = 0; i < cube.nr_cube_triangles; i++) {
		const glm::dvec3& v0 = vertices[indices[i * 3]];
		const glm::dvec3& v1 = vertices[indices[i * 3 + 1]];
		const glm::dvec3& v2 = vertices[indices[i * 3 + 2]];

		glm::dvec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
		cube.cube_triangles[i].vertices[0] = v0;
		cube.cube_triangles[i].vertices[1] = v1;
		cube.cube_triangles[i].vertices[2] = v2;
		cube.cube_triangles[i].normal = normal;
		cube.cube_triangles[i].center = calculate_triangle_center(cube.cube_triangles[i]);
	}

	// Material properties
	cube.material = material;
	cube.material_color = color;
	cube.metal_fuzz = metal_fuzz;
	cube.refraction_index = refraction_index;
	cube.shininess = shininess;

	return cube;
}

scene_object create_asymmetric_cube(point3 center, double width, double height, double depth, material_enum material, color color, double metal_fuzz, double refraction_index, double shininess) {
	scene_object asymmetric_cube;

	// Geometric properties
	asymmetric_cube.object_type = CUBE;

	double half_width = width * 0.5;
	double half_height = height * 0.5;
	double half_depth = depth * 0.5;

	// Define cube vertices
	const glm::dvec3 vertices[] = {
		// Front face
		center + glm::dvec3(-half_width, -half_height, half_depth),   // Vertex 0 (Front bottom left)
		center + glm::dvec3(half_width, -half_height, half_depth),    // Vertex 1 (Front bottom right)
		center + glm::dvec3(-half_width, half_height, half_depth),    // Vertex 2 (Front top left)
		center + glm::dvec3(half_width, half_height, half_depth),     // Vertex 3 (Front top right)

		// Back face
		center + glm::dvec3(-half_width, -half_height, -half_depth), // Vertex 4 (Back bottom left)
		center + glm::dvec3(half_width, -half_height, -half_depth),  // Vertex 5 (Back bottom right)
		center + glm::dvec3(-half_width, half_height, -half_depth),  // Vertex 6 (Back top left)
		center + glm::dvec3(half_width, half_height, -half_depth)    // Vertex 7 (Back top right)
	};

	// Define indices for triangles
	const int indices[] = {
		0, 1, 2,  2, 1, 3,  // Front face
		4, 6, 5,  5, 6, 7,  // Back face
		2, 3, 6,  6, 3, 7,  // Top face
		0, 4, 1,  1, 4, 5,  // Bottom face
		0, 2, 4,  4, 2, 6,  // Left face
		1, 5, 3,  3, 5, 7   // Right face
	};

	// Assign vertices and normal for each triangle in the cube
	for (size_t i = 0; i < asymmetric_cube.nr_cube_triangles; i++) {
		const glm::dvec3& v0 = vertices[indices[i * 3]];
		const glm::dvec3& v1 = vertices[indices[i * 3 + 1]];
		const glm::dvec3& v2 = vertices[indices[i * 3 + 2]];

		glm::dvec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
		asymmetric_cube.cube_triangles[i].vertices[0] = v0;
		asymmetric_cube.cube_triangles[i].vertices[1] = v1;
		asymmetric_cube.cube_triangles[i].vertices[2] = v2;
		asymmetric_cube.cube_triangles[i].normal = normal;
		asymmetric_cube.cube_triangles[i].center = calculate_triangle_center(asymmetric_cube.cube_triangles[i]);
	}

	// Material properties
	asymmetric_cube.material = material;
	asymmetric_cube.material_color = color;
	asymmetric_cube.metal_fuzz = metal_fuzz;
	asymmetric_cube.refraction_index = refraction_index;
	asymmetric_cube.shininess = shininess;

	return asymmetric_cube;
}

// To set face normal for spheres
void set_face_normal(const ray& ray, const glm::dvec3& outward_normal, hit_record& rec) {
	rec.outward_face = glm::dot(ray.direction, outward_normal) < 0;
	rec.normal = rec.outward_face ? outward_normal : -outward_normal;
}

// Sphere intersection is calculated as intersection with an implicit surface
bool sphere_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& sphere) {
	const glm::dvec3& ray_direction = ray.direction;
	const glm::dvec3& ray_origin = ray.origin;

	const glm::dvec3& oc = ray_origin - sphere.center;
	double a = glm::dot(ray_direction, ray_direction);
	double half_b = glm::dot(oc, ray_direction);
	double c = glm::dot(oc, oc) - sphere.radius * sphere.radius;

	// The terms under the root in the quadratic formula for sphere intersection
	// Negative values gives no real solution i.e. no intersection
	double discriminant = half_b * half_b - a * c;

	if (discriminant < 0) {
		return false;
	}

	double sqrtd = glm::sqrt(discriminant);

	double root = (-half_b - sqrtd) / a;

	if (!surrounds(ray_time, root)) {
		root = (-half_b + sqrtd) / a;
		if (!surrounds(ray_time, root)) {
			return false;
		}
	}

	// Update hit record with data from latest intersected point
	rec.time = root;
	rec.point = ray_at(ray, rec.time);

	glm::dvec3 outward_normal = (rec.point - sphere.center) / sphere.radius;
	set_face_normal(ray, outward_normal, rec);

	return true;
}

// Triangle intersection calculation using M�ller�Trumbore algorithm
bool triangle_intersection(const ray& ray, interval ray_time, hit_record& rec, const triangle& triangle) {
	const glm::dvec3& a = triangle.vertices[0];
	const glm::dvec3& b = triangle.vertices[1];
	const glm::dvec3& c = triangle.vertices[2];
	const glm::dvec3& n = triangle.normal;

	// Calculate the edge vectors
	glm::dvec3 e1 = b - a;
	glm::dvec3 e2 = c - a;

	// Calculate the determinant
	glm::dvec3 h = glm::cross(ray.direction, e2);
	double det = glm::dot(e1, h);

	// Check if the ray and triangle are roughly parallel (no intersection or infinite intersections)
	if (glm::abs(det) < 1e-8) {
		return false;
	}

	double inv_det = 1.0 / det;

	// Calculate the vector from the ray origin to vertex a
	glm::dvec3 t = ray.origin - a;

	// Calculate u parameter
	double u = glm::dot(t, h) * inv_det;

	// Check if u is out of range [0, 1]
	if (u < 0.0 || u > 1.0) {
		return false;
	}

	// Calculate q and v parameter
	glm::dvec3 q = glm::cross(t, e1);
	double v = glm::dot(ray.direction, q) * inv_det;

	// Check if v is out of range [0, 1] or if u + v is greater than 1
	if (v < 0.0 || u + v > 1.0) {
		return false;
	}

	// Calculate time to find intersection point
	double time = glm::dot(e2, q) * inv_det;

	// Make sure ray is hitting from outside the cube
	double dot = glm::dot(ray.direction, n);

	// Check if t is positive (intersection in front of ray origin) and ray is hitting face from the outside
	// Also check that triangle is closest triangle
	if (time > 0.0 && dot < 0.0 && surrounds(ray_time, time)) {
		rec.point = ray.origin + time * ray.direction;
		rec.time = time;
		rec.normal = n;
		set_face_normal(ray, n, rec);
		return true;
	}

	return false;
}

// Iterate through all triangles in the quad and run triangle intersection test
bool quad_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& quad) {
	for (size_t i = 0; i < quad.nr_quad_triangles; i++) {
		const triangle& triangle = quad.quad_triangles[i];
		if (triangle_intersection(ray, ray_time, rec, triangle)) {
			return true;
		}
	}
	return false;
}

// Iterate through all triangles in the cube and run triangle intersection test
bool cube_intersection(const ray& ray, interval ray_time, hit_record& rec, const scene_object& cube) {
	bool hit_triangle = false;
	for (size_t i = 0; i < cube.nr_cube_triangles; i++) {
		const triangle& triangle = cube.cube_triangles[i];
		if (triangle_intersection(ray, ray_time, rec, triangle)) {
			// Update the max-time in the interval to not hit far triangle if multiple are intersected
			ray_time.max = rec.time;
			hit_triangle = true;
		}
	}
	return hit_triangle;
}

// Utility function to update hit record
void update_hit_record(hit_record& temp_rec, const scene_object& obj, hit_record& rec) {
	temp_rec.material = obj.material;
	temp_rec.material_color = obj.material_color;
	temp_rec.metal_fuzz = obj.metal_fuzz;
	temp_rec.refraction_index = obj.refraction_index;
	temp_rec.shininess = obj.shininess;

	rec = temp_rec;
}

// Iterate through all scene geometries and look for intersection with current ray
bool find_intersection(const ray& ray, interval initial_ray_time_interval, hit_record& rec, const std::vector<scene_object>& scene_objects) {
	size_t nr_scene_objects = scene_objects.size();
	hit_record temp_rec;
	bool hit_anyting = false;
	double closest_so_far = initial_ray_time_interval.max;
	interval local_ray_time_interval = { initial_ray_time_interval.min, closest_so_far };

	for (size_t i = 0; i < nr_scene_objects; i++) {
		const scene_object& obj = scene_objects[i];

		switch (obj.object_type) {
		case SPHERE:
			if (sphere_intersection(ray, local_ray_time_interval, temp_rec, obj)) {
				hit_anyting = true;
				local_ray_time_interval.max = temp_rec.time;
				update_hit_record(temp_rec, obj, rec);
			}
			break;
		case CUBE:
			if (cube_intersection(ray, local_ray_time_interval, temp_rec, obj)) {
				hit_anyting = true;
				local_ray_time_interval.max = temp_rec.time;
				update_hit_record(temp_rec, obj, rec);
			}
			break;
		case QUAD:
			if (quad_intersection(ray, local_ray_time_interval, temp_rec, obj)) {
				hit_anyting = true;
				local_ray_time_interval.max = temp_rec.time;
				update_hit_record(temp_rec, obj, rec);
			}
			break;
		}

	}

	return hit_anyting;
}

// Rotation functions for polygons

void rotate_triangle_x(triangle& triangle, double angle) {
	// Calculate sine and cosine of the angle
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);

	// Create the rotation matrix for the x-axis
	glm::mat3 rotation_matrix_x = {
		1.0,      0.0,       0.0,
		0.0, cos_angle, -sin_angle,
		0.0, sin_angle,  cos_angle
	};

	glm::dvec3 translation_vector = -triangle.center;

	for (int i = 0; i < 3; i++) {
		triangle.vertices[i] += translation_vector;
		triangle.vertices[i] = rotation_matrix_x * triangle.vertices[i];
		triangle.vertices[i] += -translation_vector;
	}

	triangle.normal = rotation_matrix_x * triangle.normal;
}

void rotate_triangle_y(triangle& triangle, double angle) {
	// Calculate sine and cosine of the angle
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);

	// Create the rotation matrix for the y-axis
	glm::mat3 rotation_matrix_y = {
		cos_angle,  0.0, sin_angle,
		0.0,        1.0,      0.0,
		-sin_angle, 0.0, cos_angle
	};

	glm::dvec3 translation_vector = -triangle.center;

	for (int i = 0; i < 3; i++) {
		triangle.vertices[i] += translation_vector;
		triangle.vertices[i] = rotation_matrix_y * triangle.vertices[i];
		triangle.vertices[i] += -translation_vector;
	}
	triangle.normal = rotation_matrix_y * triangle.normal;
}

void rotate_triangle_z(triangle& triangle, double angle) {
	// Calculate sine and cosine of the angle
	double cos_angle = cos(angle);
	double sin_angle = sin(angle);


	// Create the rotation matrix for the z-axis
	glm::mat3 rotation_matrix_z = {
		cos_angle, -sin_angle, 0.0,
		sin_angle, cos_angle,  0.0,
		0.0,       0.0,        1.0
	};

	glm::dvec3 translation_vector = -triangle.center;

	for (int i = 0; i < 3; i++) {
		triangle.vertices[i] += translation_vector;
		triangle.vertices[i] = rotation_matrix_z * triangle.vertices[i];
		triangle.vertices[i] += -translation_vector;
	}
	triangle.normal = rotation_matrix_z * triangle.normal;
}

void rotate_cube_x(scene_object& cube, double angle) {
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		rotate_triangle_x(cube.cube_triangles[i], angle);
	}
}

void rotate_cube_y(scene_object& cube, double angle) {
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		rotate_triangle_y(cube.cube_triangles[i], angle);
	}
}

void rotate_cube_z(scene_object& cube, double angle) {
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		rotate_triangle_z(cube.cube_triangles[i], angle);
	}
}

void rotate_quad_x(scene_object& quad, double angle) {
	for (int i = 0; i < quad.nr_quad_triangles; i++) {
		rotate_triangle_x(quad.quad_triangles[i], angle);
	}
}

void rotate_quad_y(scene_object& quad, double angle) {
	for (int i = 0; i < quad.nr_quad_triangles; i++) {
		rotate_triangle_y(quad.quad_triangles[i], angle);
	}
}

void rotate_quad_z(scene_object& quad, double angle) {
	for (int i = 0; i < quad.nr_quad_triangles; i++) {
		rotate_triangle_z(quad.quad_triangles[i], angle);
	}
}

// Print utilities

std::ostream& print_triangle(std::ostream& os, triangle triangle) {
	os << "Normal: " << "(" << triangle.normal.x << ", " << triangle.normal.y << ", " << triangle.normal.z << ")" << std::endl;
	os << "Vertices: ";
	for (size_t i = 0; i < 3; i++) {
		os << "(" << triangle.vertices[i].x << ", " << triangle.vertices[i].y << ", " << triangle.vertices[i].z << ") ";
	}
	os << std::endl;
	return os;
}

std::ostream& print_cube(std::ostream& os, scene_object cube, point3 cube_center, double cube_size) {
	os << "Created cube with center at: " << "(" << cube_center.x << ", " << cube_center.y << ", " << cube_center.z << ")" << std::endl;
	os << "Size: " << cube_size << std::endl;
	os << "Triangles: " << std::endl;
	for (size_t i = 0; i < cube.nr_cube_triangles; i++) {
		print_triangle(os, cube.cube_triangles[i]);
	}
	os << std::endl;
	return os;
}