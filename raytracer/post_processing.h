#pragma once
#include<vector>
#include "util.h"
#include "camera.h"
std::vector<std::vector<color>> gaussian_filter(const std::vector<std::vector<color>>& pixel_colors, int kernel_size, double sigma, const camera& camera);