#include <iostream>
#include <fstream>
#include "create_image.h"
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "util.h"
#include "camera.h"

void create_image() {
	// Create and intialize camera to get image width and height
	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 600;
	cam.samples_per_pixel = 100;
	cam.max_depth = 10;
	cam.look_from = point3(0.0, 0.0, -10.0);
	cam.look_at = point3(0.0, 0.0, -20.0);


	//cam.vertical_field_of_view = 20.0; // Zoom in

	//// View from upper-back-left corner
	//cam.look_from = point3(-2, 2, 1); 
	//cam.look_at = point3(0, 0, -1);
	//cam.camera_up = glm::dvec3(0, 1, 0);

	//// Tight defocus angle and short defocus distance
	//cam.defocus_angle = 10.0;
	//cam.focus_distance = 3.4;

	render(cam);

	return;
}