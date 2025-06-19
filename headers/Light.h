#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Transform.h"

class Light : public Transform
{
public:
    glm::vec3 intensity;
    glm::vec3 ambientIntensity;

    Light(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right, glm::vec3 scale, glm::vec3 intensity, glm::vec3 ambientIntensity);
};
