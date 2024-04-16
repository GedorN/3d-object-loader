#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "lib/datadefinition.hpp"

class View {
  private:
    PlanCoords3d world_position;
    PlanCoords3d interest_point;
    PlanCoords3d head_up;
    IncCoords world_position_increment_ratio;
    IncCoords interest_point_increment_ratio;
    IncCoords head_up_increment_ratio;
  public:
    View();
    void handleTranslationKeyboardInput(GLFWwindow* window);
    glm::mat4 getView();
};