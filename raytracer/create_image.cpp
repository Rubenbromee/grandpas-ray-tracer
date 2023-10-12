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
	// Scene-specific camera configurations are done in scene_util.cpp
	camera cam;

	cam.image_width = 1200;
	cam.samples_per_pixel = 100;
	cam.max_depth = 10;

	render(cam);
	return;
}