#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/string_cast.hpp>

class Transform
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 scale;

    glm::mat3 transformationMatrix = glm::mat3(1.0f);

    int height;
    int width;

    Transform(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right, glm::vec3 scale);

    ~Transform();

    glm::mat4 getModelMatrix();
    glm::mat4 getViewMatrix();

    void updateTranslate(glm::vec3 translate);
    void updateScale(glm::vec3 factor);
};