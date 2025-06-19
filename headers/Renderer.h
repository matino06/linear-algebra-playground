#pragma once

#include "Object.h"
#include "Vector.h"
#include "TriangleMesh.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>
#include <iostream>

#define INITIAL_WIDTH 1280
#define INITIAL_HEIGHT 800

class Renderer
{
private:
    GLFWwindow *window;

public:
    std::vector<Object> coordinateSystem;
    std::vector<Object> systemArray;
    std::vector<Vector> vectors;
    Camera camera;
    Light light;
    bool showSystemArray = true;
    glm::mat3 *transformationMatrix = nullptr;

    Renderer();
    ~Renderer();

    void preRender();
    void renderAll();
    void registerRenderable(Vector vector);
    std::vector<Object> registerRenderable(std::string path);
    void registerSystem(std::string coordinateSystemPath, std::string systemArrayPath);
    void transformVectors(glm::mat3 transformationMatrix);
    void reset();

    GLFWwindow *getWindow();
};
