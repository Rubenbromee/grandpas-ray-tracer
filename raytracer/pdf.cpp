#include <random>
#include "pdf.h"

double cosine_pdf(onb onb, const glm::dvec3& direction) {
	double cosine = glm::dot(glm::normalize(direction), onb.w);
	return (cosine <= 0.0) ? 0.0 : cosine / pi;
}

double intersectable_pdf(point3 origin, glm::dvec3 search_direction, const std::vector<scene_object>& scene_objects) {
	hit_record rec;
	bool hit_anything;
	scene_object hit_object = find_intersection_return_scene_object(create_ray(origin, search_direction), interval{ 0.001, infinity }, rec, scene_objects, hit_anything);
	if (!hit_anything) {
		return 0.0;
	}
	double area = 1.0;
	switch (hit_object.object_type) {
		case SPHERE:
			area = hit_object.sphere_area;
			break;
		case CUBE:
			area = hit_object.cube_area;
			break;
		case QUAD:
			area = hit_object.quad_area;
			break;
		case ASYMMETRIC_CUBE:
			area = hit_object.cube_area;
			break;
	}
	double distance_squared = rec.time * rec.time * glm::dot(search_direction, search_direction);
	double cosine = glm::abs(glm::dot(search_direction, rec.normal) / glm::length(search_direction));
	return distance_squared / (cosine * area);
}

point3 get_random_point_on_cube(point3 origin, const scene_object& cube, const std::vector<scene_object> scene_objects) {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_real_distribution<double> uniform_distribution(0.0, 1.0);
	std::uniform_int_distribution<int> index_distribution(0, 11);

	int random_triangle_index = index_distribution(gen);

	const triangle& random_triangle = cube.cube_triangles[random_triangle_index];

	double u = uniform_distribution(gen);
	double v = uniform_distribution(gen);

	if (u + v > 1.0) {
		u = 1.0 - u;
		v = 1.0 - v;
	}

	point3 random_point_on_triangle = u * random_triangle.vertices[0] + v * random_triangle.vertices[1] + (1.0 - u - v) * random_triangle.vertices[2];

	glm::dvec3 vector_to_random_point = random_point_on_triangle - origin;

	// If this dot product is negative it means we've hit the far side of the cube and want to reflect to the near side
	if (glm::dot(vector_to_random_point, random_triangle.normal) < 0.0) {
		hit_record rec;
		find_intersection(create_ray(random_point_on_triangle, -random_triangle.normal), interval{ 0.001, infinity }, rec, scene_objects);
		random_point_on_triangle = rec.point;
	}

	return random_point_on_triangle;
}

glm::dvec3 random_vector_towards_light_source(const point3& origin) {

}