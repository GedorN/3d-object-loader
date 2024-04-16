#include "view.hpp"

View::View() {
  world_position.x = 0;
  world_position.y = 0;
  world_position.z = 3;

  interest_point.x = 0;
  interest_point.y = 0;
  interest_point.z = 0;

  head_up.x = 0;
  head_up.y = 1;
  head_up.z = 0;


  world_position_increment_ratio.x = .05f;
  world_position_increment_ratio.y = .05f;
  world_position_increment_ratio.z = .05f;
}

glm::mat4 View::getView() {
  return glm::lookAt(
    glm::vec3(world_position.x, world_position.y, world_position.z), // Camera is at (4,3,-3), in World Space
    glm::vec3(interest_point.x , interest_point.y, interest_point.z), // and looks at the origin
    glm::vec3(head_up.x, head_up.y, head_up.z)  // Head is up (set to 0,-1,0 to look upside-down)
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
}