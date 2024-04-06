#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
//#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "fileloader.hpp"




tinyobj::ObjReader reader;



void components_to_vec3s(const std::vector<tinyobj::shape_t> &components, const tinyobj::attrib_t& attrib, std::vector<float>& vecs) {
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
int loadFile () {
  std::string inputfile = "bunny.obj";
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

  return 1;
}

const std::vector<tinyobj::shape_t>& getShapes(std::vector<float>& vecs) {
  if (!reader.Error().empty()) {
    std::cerr << "TinyObjReader: " << reader.Error();
    exit(1);
  }
  const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
  const tinyobj::attrib_t& attrib = reader.GetAttrib();

  components_to_vec3s(shapes, attrib, vecs);
  std::cout << "Gera isso: " << std::endl;
  int count = 0; 

    for (int i =0; i < vecs.size(); i = i + 3) {
    count ++;
    std::cout << "(" << count << ") - " << vecs[i] << ", " << vecs[i + 1] << ", " << vecs[i + 2] << std::endl;
    }


  return shapes;
}

const tinyobj::attrib_t& GetAttrib() {
  if (!reader.Error().empty()) {
    std::cerr << "TinyObjReader: " << reader.Error();
    exit(1);
  }
  const tinyobj::attrib_t& attrib = reader.GetAttrib();

  return attrib;
}

const std::vector<tinyobj::material_t>& GetMaterials() {
  if (!reader.Error().empty()) {
    std::cerr << "TinyObjReader: " << reader.Error();
    exit(1);
  }
  const std::vector<tinyobj::material_t>& materials = reader.GetMaterials();

  return materials;
}

void print_original() {
  std::string inputfile = "teste_obj.obj";
    tinyobj::ObjReaderConfig reader_config;

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
      if (!reader.Error().empty()) {
          std::cerr << "TinyObjReader: " << reader.Error();
      }
      exit(1);
    }

    if (!reader.Warning().empty()) {
      std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    // const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    for (size_t s = 0; s < shapes.size(); s++) {
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
         // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
          tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
          tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

          // Check if `normal_index` is zero or positive. negative = no normal data
          if (idx.normal_index >= 0) {
            tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
            tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
            tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
          }

          std::cout << f +1 << ": " << "(" << vx << ", " << vy << ", " << vz << ")" << std::endl;
        }
        index_offset += fv;

      }
    }

}

// int main () {
//     std::string inputfile = "teste_obj.obj";
//     tinyobj::ObjReaderConfig reader_config;

//     tinyobj::ObjReader reader;

//     if (!reader.ParseFromFile(inputfile, reader_config)) {
//       if (!reader.Error().empty()) {
//           std::cerr << "TinyObjReader: " << reader.Error();
//       }
//       exit(1);
//     }

//     if (!reader.Warning().empty()) {
//       std::cout << "TinyObjReader: " << reader.Warning();
//     }

//     auto& attrib = reader.GetAttrib();
//     auto& shapes = reader.GetShapes();
//     // const std::vector<tinyobj::shape_t>& shapes = reader.GetShapes();
//     auto& materials = reader.GetMaterials();

//     for (size_t s = 0; s < shapes.size(); s++) {
//       size_t index_offset = 0;
//       for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
//         size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
//          // Loop over vertices in the face.
//         for (size_t v = 0; v < fv; v++) {
//           // access to vertex
//           tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
//           tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
//           tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
//           tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

//           // Check if `normal_index` is zero or positive. negative = no normal data
//           if (idx.normal_index >= 0) {
//             tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
//             tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
//             tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
//           }

//           std::cout << f +1 << ": " << "(" << vx << ", " << vy << ", " << vz << ")" << std::endl;
//         }
//         index_offset += fv;

//       }
//     }

// }
