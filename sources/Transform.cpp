#include "Transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Transform::Transform(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::vec3 right, glm::vec3 scale)
    : position(position), front(front), up(up), right(right), scale(scale) {}

Transform::~Transform() {}

glm::mat4 getRotationMatrix(glm::vec3 up, glm::vec3 right, glm::vec3 front)
{
    // Normalizacija vektora
    up = glm::normalize(up);
    right = glm::normalize(right);
    front = glm::normalize(front);
    glm::mat4 rotationZ = glm::mat4(1.0f);
    glm::vec3 rotatedVector;

    // Rotacija oko X osi da se up podesi prema front u YZ ravnini
    float dotProduct = glm::dot(front, up);
    const float rotX = glm::acos(dotProduct);
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), rotX, right);

    rotatedVector = glm::vec3(rotationX * glm::vec4(up, 0.0f));

    if (!rotX)
    {
        dotProduct = glm::dot(front, up);
        const float rotZ = glm::acos(dotProduct);
        glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), rotZ, glm::cross(up, right));

        rotatedVector = glm::vec3(rotationZ * glm::vec4(up, 0.0f));
    }

    // Rotacija oko Y osi da se novi up usmjeri kao front u XY ravnini
    const float rotY = std::atan2(front.x, front.z) - std::atan2(rotatedVector.x, rotatedVector.z);
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), rotY, up);

    // Konačna rotacijska matrica
    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;

    return rotationMatrix;
}

glm::mat4 Transform::getModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 scalingMatrix = glm::scale(model, scale);
    glm::mat4 translateMatrix = glm::translate(model, position);
    glm::mat4 rotationMatrix = glm::mat4(1.0f);

    if (front != up)
    {
        rotationMatrix = getRotationMatrix(up, right, front);
    }

    return glm::mat4(transformationMatrix) * translateMatrix * rotationMatrix * scalingMatrix;
}

glm::mat4 Transform::getViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Transform::updateTranslate(glm::vec3 newTranslate)
{
    position += newTranslate;
}

void Transform::updateScale(glm::vec3 factor)
{
    scale *= factor;
}