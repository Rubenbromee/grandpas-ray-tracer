#pragma once
#include <cmath>
#include <limits>
#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <functional>
#include "glm.hpp"
#include "gtc/random.hpp"
#include "gtc/constants.hpp"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = glm::pi<double>();
using point3 = glm::dvec3;
using color = glm::dvec3;
static std::mt19937 generator;

struct interval {
	double min = +infinity;
	double max = -infinity;
};

struct onb {
	glm::dvec3 u;
	glm::dvec3 v;
	glm::dvec3 w;
};

const static interval empty;
const static interval universe = { -infinity, +infinity };

double degrees_to_radians(double degrees);
bool near_zero(glm::dvec3 vector);
double random_double(double min = 0.0, double max = 1.0);
int random_int(int min, int max);
void re_seed_random_generator();
point3 random_point_in_unit_disk();
glm::dvec3 random_cosine_direction();
bool contains(interval i, double x);
bool surrounds(interval i, double x);

glm::dvec3 local_coord(onb onb, double a, double b, double c);
glm::dvec3 local_coord(onb onb, glm::dvec3 a);
onb build_onb_from_w(const glm::dvec3 normal);

std::ostream& print_vector(std::ostream& os, glm::dvec3 vector);