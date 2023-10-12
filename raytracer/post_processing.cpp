#include <vector>
#include "util.h"
#include "glm.hpp"
#include "camera.h"

// Kernel size affects area that is averaged, sigma affects "sharpness" of gaussian curve, low sigma: sharp curve, high sigma: rounder curve
std::vector<std::vector<color>> gaussian_filter(const std::vector<std::vector<color>>& pixel_colors, int kernel_size, double sigma, const camera& camera) {
	int zero_padding = (kernel_size - 1) / 2; // If kernel size is 3, then kernel is 3 x 3

	// Create zero-padded matrix to use for convolution
	std::vector<std::vector<color>> zero_padded_pixel_colors(camera.image_height + zero_padding, std::vector<color>(camera.image_width + zero_padding, color(0.0, 0.0, 0.0)));

	// Copy values to zero-padded matrix
	for (int i = 0 + zero_padding; i < camera.image_height - zero_padding; i++) {
		for (int j = 0 + zero_padding; j < camera.image_width - zero_padding; j++) {
			const color& pixel_color = pixel_colors[i - zero_padding][j - zero_padding]; 
			zero_padded_pixel_colors[i][j] = pixel_color; 
		}
	}

	double sum = 0.0;

	// Create gaussian filter kernel
	std::vector<std::vector<double>> gaussian_kernel(kernel_size, std::vector<double>(kernel_size, 0.0));
	for (int i = 0; i < kernel_size; i++) {
		for (int j = 0; j < kernel_size; j++) {
			double gaussian_value = (1.0 / 2.0 * pi * (sigma * sigma)) * glm::exp(-static_cast<double>((i * i) + (j * j)) / 2.0 * (sigma * sigma));
			gaussian_kernel[i][j] = gaussian_value;
			sum += gaussian_value;
		}
	}

	// Average kernel
	for (int i = 0; i < kernel_size; i++) {
		for (int j = 0; j < kernel_size; j++) {
			gaussian_kernel[i][j] /= sum;
		}
	}

	std::vector<std::vector<color>> result_matrix(camera.image_height, std::vector<color>(camera.image_width, color(0.0, 0.0, 0.0)));

	// Apply filter
	for (int i = 0 + zero_padding; i < camera.image_height - zero_padding; i++) {
		for (int j = 0 + zero_padding; j < camera.image_width - zero_padding; j++) {

			color filtered_value = color(0.0, 0.0, 0.0);

			for (int x = 0; x < kernel_size; x++) {
				for (int y = 0; y < kernel_size; y++) {
					filtered_value += zero_padded_pixel_colors[i + x - zero_padding][j + y - zero_padding] * gaussian_kernel[x][y];
				}
			}

			result_matrix[i][j] = filtered_value;

		}
	}

	return result_matrix;
}