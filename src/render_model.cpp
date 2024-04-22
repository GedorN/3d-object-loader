#include "render_model.hpp"
#include <iostream>


RenderModel::RenderModel(char* modelPath) {
  fileloader = new FileLoader(modelPath);
  r_increment_angle.x = 1;
  r_increment_angle.y = 1;
  r_increment_angle.z = 1;

  scale_factor.x = 1;
  scale_factor.y = 1;
  scale_factor.z = 1;

  s_increment_factor.x = 5.f;
  s_increment_factor.y = 5.f;
  s_increment_factor.z = 5.f;
  // fileloader.getShapes(shape);
}
RenderModel::~RenderModel() {
  delete fileloader;
}
void RenderModel::getShape(std::vector<float> &vec) {
  fileloader->getShapes(vec);
}

PlanCoords3d RenderModel::getTranlationCoords() {
  return translation_coords;
}
PlanCoords3d RenderModel::getRotationCoords() {
  return rotation_angle;
}
PlanCoords3d RenderModel::getScaleCoords() {
  return scale_factor;
}

void RenderModel::handleTranslationKeyboardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    translation_coords.y += t_increment_coords.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    translation_coords.y -= t_increment_coords.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    translation_coords.x -= t_increment_coords.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    translation_coords.x += t_increment_coords.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    translation_coords.z += t_increment_coords.z;
  } 
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    translation_coords.z -= t_increment_coords.z;
  } 
}
void RenderModel::handleRotationKeyboardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    rotation_angle.x = ((rotation_angle.x - r_increment_angle.x) < 360.0f) ? rotation_angle.x - r_increment_angle.x : 360.0 - rotation_angle.x - r_increment_angle.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    rotation_angle.x = ((rotation_angle.x + r_increment_angle.x) < 360.0f) ? rotation_angle.x + r_increment_angle.x : 360.0 - rotation_angle.x + r_increment_angle.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    rotation_angle.y = ((rotation_angle.y - r_increment_angle.y) < 360.0f) ? rotation_angle.y - r_increment_angle.y : 360.0 - rotation_angle.y- r_increment_angle.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    rotation_angle.y = ((rotation_angle.x + r_increment_angle.y) < 360.0f) ? rotation_angle.y + r_increment_angle.y : 360.0 - rotation_angle.y + r_increment_angle.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    rotation_angle.z = ((rotation_angle.z + r_increment_angle.z) < 360.0f) ? rotation_angle.z + r_increment_angle.z : 360.0 - rotation_angle.z + r_increment_angle.z;
  } 
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    rotation_angle.z = ((rotation_angle.z - r_increment_angle.z) < 360.0f) ? rotation_angle.z - r_increment_angle.z : 360.0 - rotation_angle.z - r_increment_angle.z;
  } 
}
void RenderModel::handleScaleKeyboardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    scale_factor.y += s_increment_factor.y;

  } 
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    scale_factor.y = (scale_factor.y  >  s_increment_factor.y) ? (scale_factor.y - s_increment_factor.y) : scale_factor.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    scale_factor.x = (scale_factor.x  >  s_increment_factor.x) ? scale_factor.x - s_increment_factor.x : scale_factor.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    scale_factor.x += s_increment_factor.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    scale_factor.z += s_increment_factor.z;
  } 
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    scale_factor.z = (scale_factor.z  >  s_increment_factor.z) ? scale_factor.z - s_increment_factor.z : scale_factor.z;
  } 
}