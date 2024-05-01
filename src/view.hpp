#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/datadefinition.hpp"

class View {
  private:
    PlanCoords3d world_position;
    PlanCoords3d rotation_angle;
    IncCoords world_position_increment_ratio;
    IncCoords r_angle_increment_ratio;
    IncCoords head_up_increment_ratio;
    glm::vec3 rotated_cam;
    glm::vec3 head;
  public:
    View(float objHeight, float objectWidth, float ObjectDepth);
    void handleTranslationKeyboardInput(GLFWwindow* window);
    void handleRotationKeyboardInput(GLFWwindow* window);
    void setRotationRadius();
    glm::mat4 getView();
};