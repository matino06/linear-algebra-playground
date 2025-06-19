#pragma once

#include "Object.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>
#include <iostream>

class ResourceManager
{
private:
public:
    static const aiScene *getScene(std::string path);
    static Object makeObject(aiMesh *mMesh, aiMaterial *mat);
};
