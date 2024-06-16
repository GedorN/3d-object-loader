#include "utils.hpp"
float get_object_height(const std::vector<float>& original) {
		float lower = original[1];
		float higher = original[1];
		for (int i = 4; i < original.size(); i += 3){
			if (original[i] < lower) lower = original[i];
			if (original[i] > higher) higher = original[i];
		}

		return (higher - lower);
}

float get_object_width(const std::vector<float>& original) {
		float lower = original[0];
		float higher = original[0];
		for (int i = 3; i < original.size(); i += 3){
			if (original[i] < lower) lower = original[i];
			if (original[i] > higher) higher = original[i];
		}

		return (higher - lower);
}
float get_object_depth(const std::vector<float>& original) {
		float lower = original[2];
		float higher = original[2];
		for (int i = 5; i < original.size(); i += 3){
			if (original[i] < lower) lower = original[i];
			if (original[i] > higher) higher = original[i];
		}

		return (higher - lower);
}
float get_object_height_center_point(const std::vector<float>& original) {
		float lower = original[1];
		float higher = original[1];
		for (int i = 4; i < original.size(); i += 3){
			if (original[i] < lower) lower = original[i];
			if (original[i] > higher) higher = original[i];
		}

		return (higher + lower) /2;
}

float get_object_width_center_point(const std::vector<float>& original) {
		float lower = original[0];
		float higher = original[0];
		for (int i = 3; i < original.size(); i += 3){
			if (original[i] < lower) lower = original[i];
			if (original[i] > higher) higher = original[i];
		}

		return (higher + lower) /2;
}
float get_object_depth_center_point(const std::vector<float>& original) {
		float lower = original[2];
		float higher = original[2];
		for (int i = 5; i < original.size(); i += 3){
			if (original[i] < lower) lower = original[i];
			if (original[i] > higher) higher = original[i];
		}

		return (higher + lower) /2;
}

std::vector<float> color_generator(const std::vector<float>& original) {
		float hight = get_object_height(original);
		float limiar = hight / 3;

    
    std::vector<float> resultado;
    resultado.reserve(original.size());
		float r1 = 0.8, g1 = 0, b1 = 0; // Cor inicial vermelha
    float r2 = 0, g2 = 0, b2 = 0.8; // Cor final azul


		for (int i = 1; i < original.size(); i+=3) {
			if (original[i] >= limiar) {
				resultado.push_back(r1);
				resultado.push_back(g1);
				resultado.push_back(b1);
				resultado.push_back(1.f / (hight / (original[i] != 0 ? original[i] : 1)));

			} else {
				resultado.push_back(r2);
				resultado.push_back(g2);
				resultado.push_back(b2);
				resultado.push_back(1.f / (hight / (original[i] != 0 ? original[i] : 1)));
			}
		}

    return resultado;
}


float calculateBoundingSphereRadius(std::vector<float> vec, std::vector<float> center) {
    // float maxDistance = 0.0f;
    // for (const auto& shape : shapes) {
    //     for (const auto& index : shape.mesh.indices) {
    //         glm::vec3 vertex(
    //             attrib.vertices[3 * index.vertex_index + 0],
    //             attrib.vertices[3 * index.vertex_index + 1],
    //             attrib.vertices[3 * index.vertex_index + 2]
    //         );
    //         float distance = glm::length(vertex - center);
    //         maxDistance = std::max(maxDistance, distance);
    //     }
    // }
    // return maxDistance;
}

void positionCamera(float boundingSphereRadius, std::vector<float> modelCenter) {
    // float distance = boundingSphereRadius / sin(glm::radians(45.0f / 2.0f));  // Considering that 45.0f is the FoV
    // position = modelCenter + glm::vec3(0.0f, 0.0f, distance);
}

void calculateTangentsAndBitangents(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<unsigned int>& indices, std::vector<float>& tangents, std::vector<float>& bitangents) {
    size_t vertexCount = vertices.size() / 3;
    tangents.resize(vertexCount * 3, 0.0f);
    bitangents.resize(vertexCount * 3, 0.0f);

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3 v0 = glm::make_vec3(&vertices[i0 * 3]);
        glm::vec3 v1 = glm::make_vec3(&vertices[i1 * 3]);
        glm::vec3 v2 = glm::make_vec3(&vertices[i2 * 3]);

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        glm::vec3 tangent = glm::normalize(edge1);
        glm::vec3 bitangent = glm::normalize(edge2);

        for (int j = 0; j < 3; ++j) {
            tangents[i0 * 3 + j] += tangent[j];
            tangents[i1 * 3 + j] += tangent[j];
            tangents[i2 * 3 + j] += tangent[j];

            bitangents[i0 * 3 + j] += bitangent[j];
            bitangents[i1 * 3 + j] += bitangent[j];
            bitangents[i2 * 3 + j] += bitangent[j];
        }
    }

    for (size_t i = 0; i < vertexCount; ++i) {
        glm::vec3 tangent = glm::make_vec3(&tangents[i * 3]);
        glm::vec3 bitangent = glm::make_vec3(&bitangents[i * 3]);

        tangent = glm::normalize(tangent);
        bitangent = glm::normalize(bitangent);

        for (int j = 0; j < 3; ++j) {
            tangents[i * 3 + j] = tangent[j];
            bitangents[i * 3 + j] = bitangent[j];
        }
    }
}