#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "fileloader.hpp"




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


void FileLoader::componentsToVec3s(const std::vector<tinyobj::shape_t> &components, const tinyobj::attrib_t& attrib, std::vector<float>& vecs) {
    //convert a vector of back-to-back vertex components to a vector of vec3 objects

    for (size_t s = 0; s < components.size(); s++) {
      size_t index_offset = 0;
      for (size_t f = 0; f < components[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = size_t(components[s].mesh.num_face_vertices[f]);
        for (size_t v = 0; v < fv; v++) {
          tinyobj::index_t idx = components[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
          tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
          tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];
          // vecs.push_back( glm::vec3(vx, vy, vz ));
          vecs.push_back(vx);
          vecs.push_back(vy);
          vecs.push_back(vz);

        }
        index_offset += fv;


      }

    }
}

const std::vector<tinyobj::shape_t>& FileLoader::getShapes(std::vector<float>& vecs) {
  if (!reader.Error().empty()) {
    std::cerr << "TinyObjReader: " << reader.Error();
    exit(1);
  }
  const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
  const tinyobj::attrib_t& attrib = reader.GetAttrib();

  componentsToVec3s(shapes, attrib, vecs);
  // std::cout << "Gera isso: " << std::endl;
  // int count = 0; 

  //   for (int i =0; i < vecs.size(); i = i + 3) {
  //   count ++;
  //   std::cout << "(" << count << ") - " << vecs[i] << ", " << vecs[i + 1] << ", " << vecs[i + 2] << std::endl;
  //   }


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