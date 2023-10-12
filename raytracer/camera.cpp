#include <iostream>
#include <fstream>
#include <future>
#include <vector>
#include <algorithm>
#include <exception>
#include "camera.h"
#include "color.h"
#include "util.h"
#include "geometry.h"
#include "material.h"
#include "scene_util.h"
#include "glm.hpp"
#include "pdf.h"

std::ostream& print_camera_configuration(std::ostream& os, const camera& camera) {
	os << "Aspect ratio: " << camera.aspect_ratio << std::endl;
	os << "Image width: " << camera.image_width << std::endl;
	os << "Samples per pixel: " << camera.samples_per_pixel << std::endl;
	os << "Max depth: " << camera.max_depth << std::endl;
	os << "Vertical field of view: " << camera.vertical_field_of_view << std::endl;

	os << "Camera look from: ";
	print_vector(os, camera.look_from);
	os << std::endl;

	os << "Camera look at: ";
	print_vector(os, camera.look_at);
	os << std::endl;

	os << "Camera up: ";
	print_vector(os, camera.camera_up);
	os << std::endl;

	os << "Defocus angle: " << camera.defocus_angle << std::endl;
	os << "Focus distance: " << camera.focus_distance << std::endl;

	return os;
}

void initialize(camera& camera) {
	int image_height = static_cast<int>(camera.image_width / camera.aspect_ratio);
	camera.image_height = (image_height < 1) ? 1 : image_height; // Make sure image height is atleast 1

	camera.center = camera.look_from;

	// Viewport dimensions
	// double focal_length = glm::length(camera.look_from - camera.look_at);
	double theta = degrees_to_radians(camera.vertical_field_of_view);
	double height = std::tan(theta / 2);
	double viewport_height = 2.0 * height * camera.focus_distance;
	double viewport_width = viewport_height * (static_cast<double>(camera.image_width) / image_height);

	camera.w = glm::normalize(camera.look_from - camera.look_at);
	camera.u = glm::normalize(glm::cross(camera.camera_up, camera.w));
	camera.v = glm::cross(camera.w, camera.u);

	// Image coordinates since programmatic traversal of an image, row by row, will be easier with image coordinates as directional vectors.
	glm::dvec3 viewport_u = viewport_width * camera.u;
	glm::dvec3 viewport_v = viewport_height * -camera.v;

	// Horizontal and vertical delta vectors from pixel to pixel
	camera.pixel_delta_u = viewport_u / static_cast<double>(camera.image_width);
	camera.pixel_delta_v = viewport_v / static_cast<double>(image_height);

	point3 viewport_upper_left = camera.center - (camera.focus_distance * camera.w) - viewport_u / 2.0 - viewport_v / 2.0;
	camera.pixel_00_loc = viewport_upper_left + 0.5 * (camera.pixel_delta_u + camera.pixel_delta_v);

	double defocus_radius = camera.focus_distance * std::tan(degrees_to_radians(camera.defocus_angle / 2.0));
	camera.defocus_disc_u = camera.u * defocus_radius;
	camera.defocus_disc_v = camera.v * defocus_radius;

	print_camera_configuration(std::cout, camera);
}

// Sample a random point around pixel center in the viewport within delta_u and delta_v bounds
point3 pixel_sample_square(const camera& camera) {
	double px = -0.5 + random_double(0.0, 1.0);
	double py = -0.5 + random_double(0.0, 1.0);

	return (px * camera.pixel_delta_u) + (py * camera.pixel_delta_v);
}

// Adjust ray-origin depending on depth-of-field setting
point3 defocus_disk_sample(const camera& camera) {
	point3 point = random_point_in_unit_disk();
	return camera.center + (point.x * camera.defocus_disc_u) + (point.y * camera.defocus_disc_v);
}

// Get randomly sampled camera ray for pixel at location i,j
const ray& get_multisample_ray(int i, int j, const camera& camera) {
	point3 pixel_center = camera.pixel_00_loc /*Start position*/ + (static_cast<double>(j) * camera.pixel_delta_u) /*Iterate columns*/ + (static_cast<double>(i) * camera.pixel_delta_v); /*Iterate rows*/
	point3 pixel_sample = pixel_center + pixel_sample_square(camera);

	point3 ray_origin = (camera.defocus_angle <= 0) ? camera.center : defocus_disk_sample(camera); // Adjust ray origin depending on if the camera should have depth of field or not
	glm::dvec3 ray_direction = pixel_sample - ray_origin;

	return create_ray(ray_origin, ray_direction);
}

// Calculate color for current ray
color ray_color(const ray& ray_in, int depth, const std::vector<scene_object>& scene_objects, const color& background_color, const std::vector<scene_object>& sample_objects) {
	hit_record rec;
	interval initial_ray_time_interval = { 0.001, infinity };

	// If max depth is reached, stop bouncing the ray
	if (depth <= 0) {
		return color(0.0, 0.0, 0.0);
	}

	// If no intersection is found, return background color

	bool hit_anything = false;
	const scene_object& intersected_object = find_intersection_return_scene_object(ray_in, initial_ray_time_interval, rec, scene_objects, hit_anything);

	if (!hit_anything) {
		return background_color;
	}

	ray scattered_ray;
	color attenuation;
	double pdf;

	// If intersection, check material for emission/ray-traversal
	switch (rec.material) {
		case LAMBERTIAN:
			if (lambertian_scatter(ray_in, rec, attenuation, scattered_ray, pdf)) {
				ray scattered_ray;
				double pdf = 0.0;
				glm::dvec3 scattered_ray_direction = glm::dvec3(0.0, 0.0, 0.0);
			
				// 50/50 mixture of intersectable pdf and cosine pdf unless there are no sample objects, then just use cosine pdf
				if (sample_objects.size() > 0 && random_double() < 0.5) {
					// Choose random sample object (light, dielectric, etc.)
					int random_index = random_int(0, (sample_objects.size() - 1));
					const scene_object& sample_object = sample_objects[random_index];

					calculate_intersectable_pdf(sample_object, rec, scattered_ray_direction, pdf, scene_objects);

					scattered_ray = create_ray(rec.point, scattered_ray_direction);
				}
				else {
					onb onb = build_onb_from_w(rec.normal);
					glm::dvec3 random_direction = random_hemispherical_direction(rec.normal);
					scattered_ray = create_ray(rec.point, random_direction);
					pdf = cosine_pdf(rec.normal, random_direction);
				}

				//if (depth == 9) {
				//	ray_color_debug(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects);
				//}

				return emitted(rec) + attenuation * lambertian_scatter_pdf(ray_in, rec, scattered_ray) *
					ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects) / pdf;

			}
		break;
		case METAL:
			if (metallic_reflection(ray_in, rec, attenuation, scattered_ray, rec.metal_fuzz)) {
				return attenuation * ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects);
			}
		break;
		case DIELECTRIC:
			if (dielectric_refraction(ray_in, rec, attenuation, scattered_ray, rec.refraction_index)) {
				return attenuation * ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects);
			}
		break;
		case LIGHT:
			return rec.material_color;
		break;
		case CONSTANT_DENSITY_MEDIUM_MATERIAL:
			if (constant_density_medium_scatter(rec, attenuation, scattered_ray)) {
				return attenuation * ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects);
			}
		break;
	}
}

// Calculate color for current ray, DEBUG
color ray_color_debug(const ray& ray_in, int depth, const std::vector<scene_object>& scene_objects, const color& background_color, const std::vector<scene_object>& sample_objects) {
	hit_record rec;
	interval initial_ray_time_interval = { 0.001, infinity };

	// If max depth is reached, stop bouncing the ray
	if (depth <= 0) {
		return color(0.0, 0.0, 0.0);
	}

	// If no intersection is found, return background color

	bool hit_anything = false;
	const scene_object& intersected_object = find_intersection_return_scene_object(ray_in, initial_ray_time_interval, rec, scene_objects, hit_anything);

	if (!hit_anything) {
		return background_color;
	}

	ray scattered_ray;
	color attenuation;
	double pdf;

	// If intersection, check material for emission/ray-traversal
	switch (rec.material) {
	case LAMBERTIAN:
		if (lambertian_scatter(ray_in, rec, attenuation, scattered_ray, pdf)) {
			ray scattered_ray;
			double pdf = 0.0;
			glm::dvec3 scattered_ray_direction = glm::dvec3(0.0, 0.0, 0.0);

			// 50/50 mixture of intersectable pdf and cosine pdf unless there are no sample objects, then just use cosine pdf
			if (sample_objects.size() > 0 && random_double() < 0.5) {
				// Choose random sample object (light, dielectric, etc.)
				int random_index = random_int(0, (sample_objects.size() - 1));
				const scene_object& sample_object = sample_objects[random_index];

				calculate_intersectable_pdf(sample_object, rec, scattered_ray_direction, pdf, scene_objects);

				scattered_ray = create_ray(rec.point, scattered_ray_direction);
			}
			else {
				onb onb = build_onb_from_w(rec.normal);
				glm::dvec3 random_direction = random_hemispherical_direction(rec.normal);
				scattered_ray = create_ray(rec.point, random_direction);
				pdf = cosine_pdf(rec.normal, random_direction);
			}

			return emitted(rec) + attenuation * lambertian_scatter_pdf(ray_in, rec, scattered_ray) *
				ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects) / pdf;

		}
		break;
	case METAL:
		if (metallic_reflection(ray_in, rec, attenuation, scattered_ray, rec.metal_fuzz)) {

			return attenuation * ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects);
		}
		break;
	case DIELECTRIC:
		if (dielectric_refraction(ray_in, rec, attenuation, scattered_ray, rec.refraction_index)) {

			return attenuation * ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects);
		}
		break;
	case LIGHT:
		return rec.material_color;
		break;
	case CONSTANT_DENSITY_MEDIUM_MATERIAL:
		if (constant_density_medium_scatter(rec, attenuation, scattered_ray)) {

			return attenuation * ray_color(scattered_ray, (depth - 1), scene_objects, background_color, sample_objects);
		}
		break;
	}
}


void render(camera& camera) {
	color background_color;
	std::vector<scene_object> scene_objects = create_scene(camera, background_color); // Set up scene objects, camera, background color

	// Get the sample objects in the scene by filtering them out of all scene objects
	std::vector<scene_object> sample_objects = create_scene(camera, background_color); 
	for (auto it = sample_objects.begin(); it != sample_objects.end();) {
		if (it->material != DIELECTRIC && it->material != LIGHT) {
			it = sample_objects.erase(it);
		}
		else {
			++it;
		}
	}

	initialize(camera); // Set up camera, create viewport from scene creation configurations

	std::ofstream output("output.ppm"); // Initialize output stream to ppm file

	output << "P3\n" << camera.image_width << ' ' << camera.image_height << "\n255\n"; // Define file format

	std::vector<std::future<void>> futures; // Create a vector of futures to store the multi-sample ray process for each pixel in

	// Matrix to store the image in, to avoid race conditions
	std::vector<std::vector<color>> pixel_colors(camera.image_height, std::vector<color>(camera.image_width, color(0.0, 0.0, 0.0)));

	for (int i = 0; i < camera.image_height; i++) {
		for (int j = 0; j < camera.image_width; j++) {
			// Aysnchronous processing of pixels
			// Add a thread that computes the multi-sampled pixel color
			futures.emplace_back(std::async(std::launch::async, [=, &camera, &scene_objects, &output, &pixel_colors]() {
				color pixel_color(0.0f, 0.0f, 0.0f); // Initial color for each pixel
				re_seed_random_generator(); // Re-seed each thread

				// Multi-sample a pixel
				for (size_t sample = 0; sample < camera.samples_per_pixel; sample++) {
					const ray& ray = get_multisample_ray(i, j, camera);
					pixel_colors[i][j] += ray_color(ray, camera.max_depth, scene_objects, background_color, sample_objects);
				}
			}));
		}
		std::cout << "\rScanlines remaining: " << ((camera.image_height - 1) - i) << ' ' << std::flush;
	}

	// Resolve all futures computing pixel color values
	std::cout << std::endl << "Resolving futures..." << std::endl;
	for (std::future<void>& future : futures) {
		future.wait();
	}

	// Write pixel values from color matrix to output stream
	std::cout << "Writing pixel colors from matrix to output stream..." << std::endl;
	for (int i = 0; i < camera.image_height; i++) {
		for (int j = 0; j < camera.image_width; j++) {
			const color& pixel_color = pixel_colors[i][j];
			{
				write_color(output, pixel_color, camera.samples_per_pixel);
			}
		}
	}

	output.close(); // Close output stream

	std::cout << "Done.\n";
}