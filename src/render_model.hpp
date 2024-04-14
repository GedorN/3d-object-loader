#pragma once
#include <GLFW/glfw3.h>
#include "lib/fileloader.hpp"
#include "stdio.h"
#include "lib/datadefinition.hpp"

class RenderModel {
  private:
    FileLoader* fileloader;
    std::vector<float> shape;
    PlanCoords3d translation_coords;
    PlanCoords3d rotation_angle;
    IncCoords t_increment_coords;
    IncCoords r_increment_angle;
    // float translate_x = .0f;
    // float translate_y = .0f;
    // float translate_z = .0f;
    // float translate_x_inc = .5f;
    // float translate_y_inc = .5f;
    // float translate_z_inc = .5f;
  public:
  RenderModel(char* modelPath);
  ~RenderModel();
  void handleTranslationKeyboardInput(GLFWwindow* window);
  void handleRotationKeyboardInput(GLFWwindow* window);
  void getShape(std::vector<float> &vec);
  PlanCoords3d getTranlationCoords();
  PlanCoords3d getRotationCoords();
};