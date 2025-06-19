#include "GUIMatrix.h"

GUIMatrix::GUIMatrix(glm::mat3 matrix)
    : matrix(matrix), matrixHeight(calculateMatrixHeight()), matrixWidth(calculateMatrixWidth()) {}

GUIMatrix::~GUIMatrix() {}

int GUIMatrix::calculateMatrixWidth()
{
    return TEXT_AREA_WIDTH * 3 - 5;
}

int GUIMatrix::calculateMatrixHeight()
{
    return 16 * 3 + 16 + 30;
}

void GUIMatrix::renderMatrix(int offset)
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.871f, 0.776f, 1.0f));

    ImGui::BeginGroup();
    setMatrixDrawList();

    std::string id = "matrix_table" + std::to_string(offset);

    ImGui::Columns(3, id.c_str(), false);
    ImGui::SetColumnWidth(0, TEXT_AREA_WIDTH);
    ImGui::SetColumnWidth(1, TEXT_AREA_WIDTH);
    ImGui::SetColumnWidth(2, TEXT_AREA_WIDTH);

    for (int row = 0; row < 3; ++row)
    {
        for (int col = 0; col < 3; ++col)
        {
            char buffer[64];
            std::snprintf(buffer, sizeof(buffer), "%.2f", matrix[row][col]);

            if (std::strcmp(buffer, "-0.00") == 0)
            {
                std::snprintf(buffer, sizeof(buffer), "0.00");
            }

            ImVec2 textSize = ImGui::CalcTextSize(buffer);

            float cursorPosX = ImGui::GetCursorPosX() + (TEXT_AREA_WIDTH - textSize.x) / 2;
            ImGui::SetCursorPosX(cursorPosX - 10);
            ImGui::TextUnformatted(buffer);

            ImGui::NextColumn();
        }
    }

    ImGui::Columns(1);
    ImGui::EndGroup();
    ImGui::PopStyleColor();
}

void GUIMatrix::setMatrixDrawList()
{

    ImVec2 cursorPosition = ImGui::GetCursorScreenPos();

    ImVec2 positionMin = ImVec2(cursorPosition.x, cursorPosition.y - 15);
    ImVec2 positionMax = ImVec2(positionMin.x + 5, positionMin.y + matrixHeight);
    setRectangleDrawList(positionMin, positionMax);

    positionMin = ImVec2(positionMin.x + matrixWidth, positionMin.y);
    positionMax = ImVec2(positionMin.x - 3, positionMax.y);
    setRectangleDrawList(positionMin, positionMax);

    positionMax = ImVec2(positionMin.x - 30, positionMin.y + 5);
    setRectangleDrawList(positionMin, positionMax);

    positionMin = ImVec2(positionMin.x, positionMin.y + matrixHeight);
    positionMax = ImVec2(positionMax.x, positionMax.y + matrixHeight - 8);
    setRectangleDrawList(positionMin, positionMax);

    positionMin = ImVec2(positionMin.x - matrixWidth, positionMin.y);
    positionMax = ImVec2(positionMax.x - matrixWidth + 60, positionMax.y);
    setRectangleDrawList(positionMin, positionMax);

    positionMin = ImVec2(positionMin.x, positionMin.y - matrixHeight);
    positionMax = ImVec2(positionMax.x, positionMax.y - matrixHeight + 8);
    setRectangleDrawList(positionMin, positionMax);
}

void GUIMatrix::setRectangleDrawList(ImVec2 t1, ImVec2 t2)
{
    static ImU32 rectangleColor = IM_COL32(241, 222, 198, 255);
    static ImDrawList *drawList = ImGui::GetWindowDrawList();

    drawList->AddRectFilled(t1, t2, rectangleColor);
    drawList->AddRect(t1, t2, rectangleColor, 0.0f, 0, 3.0f);
}

void GUIMatrix::multiplyWith(glm::mat3 multiplierMatrix)
{
    matrix = multiplierMatrix * matrix;
}

void GUIMatrix::setEqualsDrawList()
{
    ImVec2 cursorPosition = ImGui::GetCursorScreenPos();

    ImVec2 positionMin = ImVec2(cursorPosition.x - 30, cursorPosition.y + 43);
    ImVec2 positionMax = ImVec2(positionMin.x + 17, positionMin.y - 3);
    setRectangleDrawList(positionMin, positionMax);

    positionMin = ImVec2(positionMin.x, positionMin.y - 15);
    positionMax = ImVec2(positionMax.x, positionMin.y - 3);
    setRectangleDrawList(positionMin, positionMax);
}