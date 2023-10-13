#include <iostream>
#include <fstream>
#include "create_image.h"
#include "color.h"
#include "ray.h"
#include "geometry.h"
#include "util.h"
#include "camera.h"

void create_image() {
	// Common camera features for all scene configurations
	// Scene-specific camera configurations are done in scene_creation.cpp
	// All camera configurations can also be overwritten in scene_creation.cpp if you want to change samples per pixel etc.
	camera cam;

	cam.image_width = 600;
	cam.samples_per_pixel = 100;
	cam.max_depth = 10;

	render(cam);
	return;
}