#pragma once
#include "tiny_obj_loader.h"
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>



int loadFile ();
const std::vector<tinyobj::shape_t>& getShapes(std::vector<float>& vecs);
const tinyobj::attrib_t& GetAttrib();
const std::vector<tinyobj::material_t>& GetMaterials();
void print_original();


