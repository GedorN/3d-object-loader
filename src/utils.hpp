#pragma once
#include <vector>
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp> 

float get_object_height(const std::vector<float>& original);

float get_object_width(const std::vector<float>& original);
float get_object_depth(const std::vector<float>& original);
float get_object_height_center_point(const std::vector<float>& original);

float get_object_width_center_point(const std::vector<float>& original);
float get_object_depth_center_point(const std::vector<float>& original);
std::vector<float> color_generator(const std::vector<float>& original);

void calculateTangentsAndBitangents(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<unsigned int>& indices, std::vector<float>& tangents, std::vector<float>& bitangents);