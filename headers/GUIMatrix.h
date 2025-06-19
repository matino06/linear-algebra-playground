#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdio>
#include <string>

#define TEXT_AREA_WIDTH 80

class GUIMatrix
{
private:
    int matrixHeight;
    int matrixWidth;

public:
    glm::mat3 matrix;

    GUIMatrix(glm::mat3 matrix);
    ~GUIMatrix();

    int calculateMatrixWidth();
    int calculateMatrixHeight();
    void renderMatrix(int offset);
    void setMatrixDrawList();
    void setRectangleDrawList(ImVec2 t1, ImVec2 t2);
    void multiplyWith(glm::mat3 multiplierMatrix);
    void setEqualsDrawList();
};
