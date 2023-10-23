#include "geometry_util.h"

// Triangle centers are needed for rotation
point3 calculate_triangle_center(const triangle& triangle) {
	// Calculate the average of the vertices' coordinates
	point3 center = (triangle.vertices[0] + triangle.vertices[1] + triangle.vertices[2]) / 3.0;

	return center;
}

// To set face normal for geometries
void set_face_normal(const ray& ray, const glm::dvec3& outward_normal, hit_record& rec) {
	rec.outward_face = glm::dot(ray.direction, outward_normal) < 0;
	rec.normal = rec.outward_face ? outward_normal : -outward_normal;
}

// Utility function to update hit record
void update_hit_record(hit_record& temp_rec, const scene_object& obj, hit_record& rec) {
	temp_rec.material = obj.material;
	temp_rec.material_color = obj.material_color;
	temp_rec.metal_fuzz = obj.metal_fuzz;
	temp_rec.refraction_index = obj.refraction_index;
	rec = temp_rec;
}

// Utility functions for surface area calculations for polygons

// Calculate the area of the triangle using Heron's formula
double calculate_triangle_area(const triangle& triangle) {
	glm::dvec3 a = triangle.vertices[0];
	glm::dvec3 b = triangle.vertices[1];
	glm::dvec3 c = triangle.vertices[2];
	double side_1 = glm::length(b - a);
	double side_2 = glm::length(c - b);
	double side_3 = glm::length(a - c);
	double s = (side_1 + side_2 + side_3) / 2.0;
	double triangle_area = glm::sqrt(s * (s - side_1) * (s - side_2) * (s - side_3));
	return triangle_area;
}

// Use triangle area calculation on all triangles in the cube
double calculate_cube_area(const scene_object& cube) {
	double total_area = 0.0;

	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		const triangle& triangle = cube.cube_triangles[i];
		double triangle_area = calculate_triangle_area(triangle);
		total_area += triangle_area;
	}

	return total_area;
}

// Use triangle area calculation on all triangles in the quad
double calculate_quad_area(const scene_object& quad) {
	double total_area = 0.0;

	for (int i = 0; i < quad.nr_quad_triangles; i++) {
		const triangle& triangle = quad.quad_triangles[i];
		double triangle_area = calculate_triangle_area(triangle);
		total_area += triangle_area;
	}

	return total_area;
}

// Utility functions for getting random points on different geometries, used for calculating pdf:s

// Used for sampling spherical geometries
point3 get_random_point_on_sphere(point3 origin, const scene_object& sphere, const hit_record& rec) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);

	// Generate random spherical coordinates
	double theta = 2.0 * pi * uniform_distribution(gen); // Random angle in radians
	double phi = acos(2.0 * uniform_distribution(gen) - 1.0); // Random inclination angle in radians

	// Convert spherical coordinates to Cartesian coordinates
	double x = sphere.sphere_center.x + sphere.sphere_radius * sin(phi) * cos(theta);
	double y = sphere.sphere_center.y + sphere.sphere_radius * sin(phi) * sin(theta);
	double z = sphere.sphere_center.z + sphere.sphere_radius * cos(phi);

	point3 random_point = point3(x, y, z);

	glm::dvec3 vector_to_random_point = random_point - origin;

	// If the point is on the far side of the sphere seen from the origin, move it to the near side
	if (glm::dot(vector_to_random_point, rec.normal) > 0.0) {
		random_point = random_point + (2.0 * sphere.sphere_radius) * (-rec.normal);
	}

	return random_point;
}

// Get random parametrized point on a triangle
point3 get_random_point_on_triangle(const triangle& random_triangle) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);

	double u = uniform_distribution(gen);
	double v = uniform_distribution(gen);

	if (u + v > 1.0) {
		u = 1.0 - u;
		v = 1.0 - v;
	}

	point3 random_point_on_triangle = u * random_triangle.vertices[0] + v * random_triangle.vertices[1] + (1.0 - u - v) * random_triangle.vertices[2];

	return random_point_on_triangle;
}

// Used for sampling quad geometries
point3 get_random_point_on_quad(point3 origin, const scene_object& quad) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> index_distribution(0, 1);

	int random_triangle_index = index_distribution(gen);

	// Choose a random triangle
	const triangle& random_triangle = quad.quad_triangles[random_triangle_index];

	point3 random_point_on_quad = get_random_point_on_triangle(random_triangle);

	return random_point_on_quad;
}

// Used for sampling cubical geometries
point3 get_random_point_on_cube(point3 origin, const scene_object& cube, const std::vector<scene_object>& scene_objects, bool ignore_reflection, glm::dvec3 triangle_normal) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> index_distribution(0, 11);

	int random_triangle_index = index_distribution(gen);

	// Choose a random triangle
	const triangle& random_triangle = cube.cube_triangles[random_triangle_index];
	triangle_normal = random_triangle.normal;

	point3 random_point_on_cube = get_random_point_on_triangle(random_triangle);

	if (ignore_reflection) {
		return random_point_on_cube;
	}

	glm::dvec3 vector_to_random_point = random_point_on_cube - origin;

	// If this dot product is positive it means we've hit the far side of the cube, seen from the origin, and want to reflect the point to the near side
	// Do so by finding the intersection in the negative normal direction
	if (glm::dot(vector_to_random_point, random_triangle.normal) > 0.0) {
		hit_record rec;
		find_intersection(create_ray(random_point_on_cube, -random_triangle.normal), interval{ 0.001, infinity }, rec, scene_objects);
		random_point_on_cube = rec.point;
	}

	return random_point_on_cube;
}

// Print utilities for different geometries

std::ostream& print_triangle(std::ostream& os, triangle triangle) {
	os << "Normal: " << "(" << triangle.normal.x << ", " << triangle.normal.y << ", " << triangle.normal.z << ")" << std::endl;
	os << "Vertices: ";
	for (int i = 0; i < 3; i++) {
		os << "(" << triangle.vertices[i].x << ", " << triangle.vertices[i].y << ", " << triangle.vertices[i].z << ") ";
	}
	os << std::endl;
	return os;
}

std::ostream& print_cube(std::ostream& os, scene_object cube, point3 cube_center, double cube_size) {
	os << "Created cube with center at: " << "(" << cube_center.x << ", " << cube_center.y << ", " << cube_center.z << ")" << std::endl;
	os << "Size: " << cube_size << std::endl;
	os << "Triangles: " << std::endl;
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		print_triangle(os, cube.cube_triangles[i]);
	}
	os << std::endl;
	return os;
}

std::ostream& print_asymmetric_cube(std::ostream& os, scene_object cube, point3 cube_center) {
	os << "Created cube with center at: " << "(" << cube_center.x << ", " << cube_center.y << ", " << cube_center.z << ")" << std::endl;
	os << "Triangles: " << std::endl;
	for (int i = 0; i < cube.nr_cube_triangles; i++) {
		print_triangle(os, cube.cube_triangles[i]);
	}
	os << std::endl;
	return os;
}