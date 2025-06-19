#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 right, glm::vec3 scale)
    : Transform(position, glm::vec3(0.0f) - position, up, right, scale) {}

Camera::~Camera() {}

glm::mat4 Camera::getPerspectiveMatrix(GLFWwindow *window)
{
    static int width, height;
    glfwGetWindowSize(window, &width, &height);

    return glm::perspective(glm::radians(FOV_DEG), ((float)width * 0.67f / (float)height), NEAR_PLANE, FAR_PLANE);
}
