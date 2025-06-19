#pragma once

#include "Transform.h"
#include "Material.h"

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

class TriangleMesh
{
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    int vertexCount = 0;
    int indexCount = 0;

    unsigned int VAO;
    unsigned int VBO[2];
    unsigned int EBO;

    TriangleMesh(std::vector<glm::vec3> Vertices, std::vector<unsigned int> Indices, std::vector<glm::vec3> Normals);
    ~TriangleMesh();

    void initializeBuffers();
    std::pair<glm::vec3, glm::vec3> getBoundingBox();
};
