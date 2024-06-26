#pragma once
#include <GLFW/glfw3.h>
#include "lib/fileloader.hpp"
#include "stdio.h"
#include "lib/datadefinition.hpp"


class RenderModel {
  private:
    FileLoader* fileloader;
    std::vector<float> shape;
    std::vector<float> normal;
    std::vector<uint> indices;
    PlanCoords3d translation_coords;
    PlanCoords3d rotation_angle;
    PlanCoords3d scale_factor;
    IncCoords t_increment_coords;
    IncCoords r_increment_angle;
    IncCoords s_increment_factor;
    unsigned int textureID;
    unsigned int normalMapID;
  public:
  RenderModel(char* modelPath);
  ~RenderModel();
  void handleTranslationKeyboardInput(GLFWwindow* window);
  void handleRotationKeyboardInput(GLFWwindow* window);
  void handleScaleKeyboardInput(GLFWwindow* window);
  void getShape(std::vector<float> &vec);
  void getNormal(std::vector<float> &norm);
  void getIndices(std::vector<uint> &ind);
  PlanCoords3d getTranlationCoords();
  PlanCoords3d getRotationCoords();
  PlanCoords3d getScaleCoords();
  void loadTextures(char* texture_faces);
  void loadNormalMap(char* filename);
  unsigned int getTextureID();
  unsigned int getNormalMapID();
};