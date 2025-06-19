#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Transform.h"

#include <iostream>

#define FOV_DEG 45.0f
#define NEAR_PLANE 0.1f
#define FAR_PLANE 500.0f

class Camera : public Transform
{
public:
    float speed = 0.0005f;
    float sensitivity = 100.0f;

    Camera(glm::vec3 position, glm::vec3 up, glm::vec3 right, glm::vec3 scale);
    ~Camera();

    glm::mat4 getPerspectiveMatrix(GLFWwindow *window);
};
