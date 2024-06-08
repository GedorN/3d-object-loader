#define TINYOBJLOADER_IMPLEMENTATION
#include "fileloader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



FileLoader::FileLoader(char* modelPath) {
  std::string inputfile = modelPath;
  tinyobj::ObjReaderConfig reader_config;


  if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
  }

  if (!reader.Warning().empty()) {
    std::cout << "TinyObjReader: " << reader.Warning();
  }
}


void FileLoader::componentsToVec3s(const std::vector<tinyobj::shape_t> &components, const tinyobj::attrib_t& attrib, std::vector<float>& vecs, std::vector<float>& normals) {
    for (size_t s = 0; s < components.size(); s++) {
      size_t index_offset = 0;
      for (size_t f = 0; f < components[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = size_t(components[s].mesh.num_face_vertices[f]);
        for (size_t v = 0; v < fv; v++) {
          tinyobj::index_t idx = components[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3*(idx.vertex_index)+0];
          tinyobj::real_t vy = attrib.vertices[3*(idx.vertex_index)+1];
          tinyobj::real_t vz = attrib.vertices[3*(idx.vertex_index)+2];
          vecs.push_back(vx);
          vecs.push_back(vy);
          vecs.push_back(vz);
          if (idx.normal_index >= 0) {
            tinyobj::real_t nx = attrib.normals[3*(idx.normal_index)+0];
            tinyobj::real_t ny = attrib.normals[3*(idx.normal_index)+1];
            tinyobj::real_t nz = attrib.normals[3*(idx.normal_index)+2];
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);
          } else {
            // Caso não tenha normal associada, adicionar um valor padrão (0,0,0)
            normals.push_back(0.0f);
            normals.push_back(0.0f);
            normals.push_back(0.0f);
          }
        }
        index_offset += fv;
      }

    }
}

const std::vector<tinyobj::shape_t>& FileLoader::getShapes(std::vector<float>& vecs, std::vector<float>& normals) {
  if (!reader.Error().empty()) {
    std::cerr << "TinyObjReader: " << reader.Error();
    exit(1);
  }
  const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
  const tinyobj::attrib_t& attrib = reader.GetAttrib();

  componentsToVec3s(shapes, attrib, vecs, normals);

  return shapes;
}

const tinyobj::attrib_t& FileLoader::GetAttrib() {
  if (!reader.Error().empty()) {
    std::cerr << "TinyObjReader: " << reader.Error();
    exit(1);
  }
  const tinyobj::attrib_t& attrib = reader.GetAttrib();

  return attrib;
}

const std::vector<tinyobj::material_t>& FileLoader::GetMaterials() {
  if (!reader.Error().empty()) {
    std::cerr << "TinyObjReader: " << reader.Error();
    exit(1);
  }
  const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

  return materials;
}

unsigned int FileLoader::loadTextures(char* texture_faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
  
  int width, height, nrChannels;
  unsigned char *data;  
  for(unsigned int i = 0; i < 6; i++) {
    if (data) {
      data = stbi_load(texture_faces, &width, &height, &nrChannels, 0);
      glTexImage2D(
        GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
      );
      stbi_image_free(data);

    } else {
        std::cout << "Cubemap texture failed to load at path: " << texture_faces[i] << std::endl;
        stbi_image_free(data);
    }

  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

  return textureID;
}