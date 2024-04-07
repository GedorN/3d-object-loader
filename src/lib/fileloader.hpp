#pragma once
#include "tiny_obj_loader.h"
#include <iostream>
#include <vector>
#include <glm/vec3.hpp>


class FileLoader {
  private:
    tinyobj::ObjReader reader;
    void componentsToVec3s(const std::vector<tinyobj::shape_t> &components, const tinyobj::attrib_t& attrib, std::vector<float>& vecs);
  public:
    FileLoader(char* modelPath);
    const std::vector<tinyobj::shape_t>& getShapes(std::vector<float>& vecs);
    const tinyobj::attrib_t& GetAttrib();
    const std::vector<tinyobj::material_t>& GetMaterials();
    void print_original();
};
