#pragma once
#include "util.h"
#include "geometry.h"

struct camera {
	double aspect_ratio = 1.0; // Ratio of image width over height
	int image_width = 100; // Rendered image width in pixel count
	int samples_per_pixel = 10; // Count of random samples for each pixel
	int max_depth = 10; // Maximum number of ray bounces
	double vertical_field_of_view = 90.0; // Vertical view angle (field of view)
	point3 look_from = point3(0.0, 0.0, 0.0); // Point camera is looking from
	point3 look_at = point3(0.0, 0.0, -1.0); // Point camera is looking at
	glm::dvec3 camera_up = glm::dvec3(0.0, 1.0, 0.0); // Camera-relative "up" direction
	double defocus_angle = 0.0; // Variation angle of rays through each pixel, 0.0 turns off depth-of-field
	double focus_distance = 10.0; // Distance from camera lookfrom point to plane of perfect focus

	int image_height; // Rendered image height
	point3 center; // Camera center
	point3 pixel_00_loc; // Location of pixel 0, 0
	glm::dvec3 pixel_delta_u; // Offset to pixel to the right
	glm::dvec3 pixel_delta_v; // Offset to pixel below
	glm::dvec3 u, v, w; // Camera frame basis vectors
	glm::dvec3 defocus_disc_u; // Defocus disk horizontal radius
	glm::dvec3 defocus_disc_v; // Defocus disk vertical radius
};

std::ostream& print_camera_configuration(std::ostream& os, const camera& camera);
void initialize(camera& camera);
point3 pixel_sample_square(const camera& camera);
point3 defocus_disk_sample(const camera& camera);
const ray& get_multisample_ray(int i, int j, const camera& camera);
color ray_color(const ray& ray, int depth, const std::vector<scene_object>& scene_objects, const color& background_color);
void render(camera& camera);