#pragma once
#include <vector>

float get_object_height(const std::vector<float>& original);

float get_object_width(const std::vector<float>& original);
float get_object_depth(const std::vector<float>& original);
float get_object_height_center_point(const std::vector<float>& original);

float get_object_width_center_point(const std::vector<float>& original);
float get_object_depth_center_point(const std::vector<float>& original);
std::vector<float> color_generator(const std::vector<float>& original);