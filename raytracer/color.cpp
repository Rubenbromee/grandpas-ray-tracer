#include <iostream>
#include "util.h"

double linear_to_gamma(double linear_component) {
	return glm::sqrt(linear_component);
}

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
	double r = pixel_color.x;
	double g = pixel_color.y;
	double b = pixel_color.z;

	// Divide color by number of samples
	double scale = 1.0f / static_cast<double>(samples_per_pixel);
	r *= scale;
	g *= scale;
	b *= scale;

	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	out << static_cast<int>(256 * glm::clamp(r, 0.000, 0.999)) << ' '
		<< static_cast<int>(256 * glm::clamp(g, 0.000, 0.999)) << ' '
		<< static_cast<int>(256 * glm::clamp(b, 0.000, 0.999)) << '\n';
}