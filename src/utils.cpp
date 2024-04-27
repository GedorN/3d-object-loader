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
    resultado.reserve(original.size()); // Otimização para alocar espaço de antemão
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