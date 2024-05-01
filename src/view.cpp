#include "view.hpp"
#include <iostream>
#include "stdio.h"
View::View(float objHeight, float objectWidth, float ObjectDepth) {

  float higher = objHeight > objectWidth ? objHeight : objectWidth;

  world_position.x = 0;
  world_position.y = 0;
  world_position.z = higher > 1 ? higher : 2 * higher;

  world_position_increment_ratio.x = objectWidth / 10;
  world_position_increment_ratio.y = objHeight / 10;
  world_position_increment_ratio.z = ObjectDepth / 10;

  rotation_angle.x = 0;
  rotation_angle.y = 0;
  rotation_angle.z = 0;

  r_angle_increment_ratio.x = .5f;
  r_angle_increment_ratio.y = .5f;
  r_angle_increment_ratio.z = .5f;
  rotated_cam = glm::vec3(0, 0, 0);
  head = glm::vec3(0, 1, 0);


}
glm::mat4 View::getView() {
  return glm::lookAt(
    glm::vec3(world_position.x, world_position.y, world_position.z), // camera position in space
    rotated_cam, // interest point
    head // Head up
  );
}

void View::handleTranslationKeyboardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    world_position.y += world_position_increment_ratio.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    world_position.y -= world_position_increment_ratio.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    world_position.x -= world_position_increment_ratio.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    world_position.x += world_position_increment_ratio.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    world_position.z += world_position_increment_ratio.z;
  } 
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    world_position.z -= world_position_increment_ratio.z;
  } 

  // Follow the rotation while deactivated to prevent abrupt changes in values when it becomes activated.
  glm::vec3 current_camera_position = glm::vec3(world_position.x, world_position.y, world_position.z);
  glm::vec3 direction = glm::vec3(0, 0, 0) - current_camera_position;
  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle.x), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation = glm::rotate(rotation, glm::radians(rotation_angle.y), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::vec3 rotatedDirection = glm::vec3(rotation * glm::vec4(direction, 0.0f));
  rotated_cam = current_camera_position + rotatedDirection;
}
void View::handleRotationKeyboardInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    rotation_angle.y -= r_angle_increment_ratio.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    rotation_angle.y += r_angle_increment_ratio.y;
  } 
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    rotation_angle.x -= r_angle_increment_ratio.x;
  } 
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    rotation_angle.x += r_angle_increment_ratio.x;
    glm::vec3 current_camera_position = glm::vec3(world_position.x, world_position.y, world_position.z);
  } 
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    rotation_angle.z -= r_angle_increment_ratio.z;
  } 
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    rotation_angle.z += r_angle_increment_ratio.z;
  }

  // rotation in x and y axis
  glm::vec3 current_camera_position = glm::vec3(world_position.x, world_position.y, world_position.z);
  glm::vec3 direction = glm::vec3(0, 0, 0) - current_camera_position;
  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle.x), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation = glm::rotate(rotation, glm::radians(rotation_angle.y), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::vec3 rotatedDirection = glm::vec3(rotation * glm::vec4(direction, 0.0f));
  rotated_cam = current_camera_position + rotatedDirection;

  // head rotation to simulate z axis rotation
  glm::mat4 head_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
  head = glm::vec3(head_rotation_matrix * glm::vec4(glm::vec3(0, 1, 0), 1.0f));







  
}
