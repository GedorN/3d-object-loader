#include "render_model.hpp"
#include <iostream>
#include "utils.hpp"


RenderModel::RenderModel(char* modelPath) {
  fileloader = new FileLoader(modelPath);

  fileloader->getShapes(shape, normal);

  float y = get_object_height_center_point(shape);
  float x = get_object_width_center_point(shape);
  float z = get_object_depth_center_point(shape);

  float widht = get_object_width(shape);
  float height = get_object_height(shape);
  float depth = get_object_depth(shape);

  translation_coords.y = -y;
  translation_coords.x = -x;
  translation_coords.z = -z;

  r_increment_angle.x = 1;
  r_increment_angle.y = 1;
  r_increment_angle.z = 1;

  scale_factor.x = 1;
  scale_factor.y = 1;
  scale_factor.z = 1;

  s_increment_factor.x = widht / 10;
  s_increment_factor.y = height / 10;
  s_increment_factor.z = depth / 10;

  float lower = widht < height ? widht : height;

  t_increment_coords.x = lower / 15;
  t_increment_coords.y = lower / 15;
  t_increment_coords.z = lower / 15;
  // fileloader.getShapes(shape);
}
RenderModel::~RenderModel() {
  delete fileloader;
}
void RenderModel::getShape(std::vector<float> &vec) {
  vec = shape;
}

void RenderModel::getNormal(std::vector<float> &norm) {
  norm = normal;
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

void RenderModel::loadTextures(char* texture_faces) {
  textureID = fileloader->loadTextures(texture_faces);
}

unsigned int RenderModel::getTextureID() {
  return textureID;
}
