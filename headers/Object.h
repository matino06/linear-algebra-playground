#pragma once

#include "Transform.h"
#include "TriangleMesh.h"
#include "Shader.h"
#include "Light.h"
#include "Material.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

// Deklaracija funkcije
class Object : public Transform
{
public:
    Material material;
    TriangleMesh mesh;
    Shader *shader;

    Object(TriangleMesh Mesh, Material Material);
    ~Object();

    void render(glm::vec3 cameraPosition, glm::mat4 perspectiveMatrix, glm::mat4 viewMatrix, Light light);

private:
    Shader *loadShader();
};