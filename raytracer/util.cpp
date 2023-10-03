#include <iostream>
#include <random>
#include <thread>
#include "util.h"
#include "geometry.h"

double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

// Check if all elements in a vector is near zero
bool near_zero(glm::dvec3 vector) {
	double threshold = 1e-8;
	return glm::abs(vector.x) < threshold && glm::abs(vector.y) < threshold && glm::abs(vector.z) < threshold;
}

void re_seed_random_generator() {
	// Generate a seed based on thread-specific information and timestamps
	std::hash<std::thread::id> hasher;
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::size_t seed = hasher(std::this_thread::get_id()) ^ static_cast<std::size_t>(now.time_since_epoch().count());
	generator.seed(seed);
}

// Random double in interval [min,max), [0.0, 1.0) by default
double random_double(double min, double max) {
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);

	if (min == 0.0 && max == 1.0) {
		return distribution(generator);
	}
	else {
		return min + (max - min) * distribution(generator);
	}
}

point3 random_point_in_unit_disk() {
	while (true) {
		point3 point = point3(random_double(-1.0, 1.0), random_double(-1.0, 1.0), 0.0);
		if (glm::dot(point, point) < 1.0) {
			return point;
		}
	}
}

glm::dvec3 random_cosine_direction() {
	double r1 = random_double();
	double r2 = random_double();
	double z = glm::sqrt(1.0 - r2);
	double phi = 2 * pi * r1;
	double x = glm::cos(phi) * glm::sqrt(r2);
	double y = glm::sin(phi) * glm::sqrt(r2);
	return glm::dvec3(x, y, z);
}

bool contains(interval i, double x) {
	return i.min <= x && x <= i.max;
}

bool surrounds(interval i, double x) {
	return i.min < x && x < i.max;
}

glm::dvec3 local_coord(onb onb, double a, double b, double c) {
	return a * onb.u + b * onb.v + c * onb.w;
}

glm::dvec3 local_coord(onb onb, glm::dvec3 a) {
	return a.x * onb.u + a.y * onb.u + a.z * onb.w;
}

onb build_onb_from_w(const glm::dvec3 normal) {
	onb onb;
	onb.w = glm::normalize(normal);
	glm::dvec3 a = (glm::abs(onb.w.x) > 0.9) ? glm::dvec3(0.0, 1.0, 0.0) : glm::dvec3(1.0, 0.0, 0.0);
	onb.v = glm::normalize(glm::cross(onb.w, a));
	onb.u = glm::cross(onb.w, onb.v);
	return onb;
}


std::ostream& print_vector(std::ostream& os, glm::dvec3 vector) {
	os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
	return os;
}