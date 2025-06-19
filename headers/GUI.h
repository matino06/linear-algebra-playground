#pragma once

#include "Renderer.h"
#include "GUIMatrix.h"
#include "Monitor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Vector.h"

#include <thread>
#include <chrono>
#include <random>

enum class LayoutType
{
    Transform = 0,
    CrossProduct = 1
};

class GUI
{
private:
    Renderer &renderer;
    Monitor &transformationMonitor;
    std::vector<GUIMatrix> transformationMatrices;
    GUIMatrix finalTransformationMatrix = GUIMatrix(glm::mat3(1.0f));
    ImFont *bigFont;
    ImFont *mediumFont;
    ImFont *smallFont;
    ImFont *microFont;
    int layoutType = 0;

public:
    GUI(GLFWwindow *window, Renderer &renderer, Monitor &transformationMonitor);
    ~GUI();

    void renderGUI();
    void newFrame();
    void render();
    void renderLayout();
    void renderLayoutTransform();
    void renderLayoutCrossProduct();
    void renderLayoutDotProduct();
    void renderVectors();
    void renderTransformationMatrices();
    void transformSystem(glm::mat3 transformationMatrix);
    void resetTransformations();
};