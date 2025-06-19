
#pragma once
#include "ResourceManager.h"
#include "Transform.h"
#include "Object.h"

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <cmath>
#include <iostream>

enum class VectorComponent
{
    VectorBody = 0,
    VectorHead = 1
};

class Vector
{
public:
    Object vectorBody;
    Object vectorHead;
    glm::vec3 position;
    glm::vec3 front;

    Vector(glm::vec3 position, glm::vec3 front, glm::vec3 color);
    ~Vector();

    void updateVector();
    void setVectorColor(glm::vec3 color);
    Object getVectorBody();
    Object getVectorHead();
    Object makeVectorPart(VectorComponent part);
};