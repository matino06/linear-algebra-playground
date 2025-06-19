#include "GUI.h"

GUI::GUI(GLFWwindow *window, Renderer &renderer, Monitor &transformationMonitor) : renderer(renderer), transformationMonitor(transformationMonitor)
{
    renderer.transformationMatrix = &finalTransformationMatrix.matrix;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::StyleColorsDark();
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.09f, 0.6f, 0.682f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.945f, 0.871f, 0.776f, 1.0f));

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.514f, 0.263f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.514f, 0.263f, 0.95f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.514f, 0.263f, 0.55f));

    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.945f, 0.871f, 0.776f, 0.95f)); // Color for Combo on hover
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.945f, 0.871f, 0.776f, 0.95f));

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.09f, 0.6f, 0.682f, 1.0f));

    ImGuiStyle &style = ImGui::GetStyle();
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.FramePadding = ImVec2(8, 8);
    style.ItemSpacing = ImVec2(10, 8);

    ImGuiIO &io = ImGui::GetIO();
    smallFont = io.Fonts->AddFontFromFileTTF("../resources/Lato,Roboto/Roboto/Roboto-Black.ttf", 16.0f);
    microFont = io.Fonts->AddFontFromFileTTF("../resources/Lato,Roboto/Roboto/Roboto-Black.ttf", 10.0f);
    mediumFont = io.Fonts->AddFontFromFileTTF("../resources/Lato,Roboto/Roboto/Roboto-Black.ttf", 20.0f);
    bigFont = io.Fonts->AddFontFromFileTTF("../resources/Lato,Roboto/Roboto/Roboto-Black.ttf", 26.0f);
}

GUI::~GUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::newFrame()
{
    // ImGui: početak novog frame-a renderer
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::render()
{
    // Renderiranje ImGui-a
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::renderLayout()
{
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.33f, ImGui::GetIO().DisplaySize.y));
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.67f, 0));

    ImGui::Begin("##NoTitleBar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

    ImGui::SetCursorPos(ImVec2(20, 20));
    ImGui::BeginGroup();

    static const char *items[] = {"Transform", "Cross Product"};

    const int oldType = layoutType;
    ImGui::Combo("Transform Type", &layoutType, items, IM_ARRAYSIZE(items));

    if (oldType != layoutType)
    {
        renderer.vectors.clear();
        resetTransformations();
    }

    ImGui::Spacing();
    ImGui::Spacing();

    switch (layoutType)
    {
    case static_cast<int>(LayoutType::Transform):
        renderLayoutTransform();
        break;
    case static_cast<int>(LayoutType::CrossProduct):
        renderer.showSystemArray = false;
        renderLayoutCrossProduct();
        break;
    default:
        break;
    }

    ImGui::EndGroup();
    ImGui::End();
}

void GUI::renderLayoutTransform()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_real_distribution<> distribution(0.0, 1.0);

    static glm::vec3 vector(0.0f, 0.0f, 0.0f);
    ImGui::InputFloat3("##NoTitleBarVec", &vector[0]);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.87f, 0.776f, 1.0f));
    if (ImGui::Button("Add Vector"))
    {
        renderer.registerRenderable(Vector(glm::vec3(0.0f, 0.0f, 0.0f), finalTransformationMatrix.matrix * vector, glm::vec3(distribution(generator), distribution(generator), distribution(generator))));
        vector = glm::vec3(0.0f);
    }
    ImGui::PopStyleColor();

    ImGui::Spacing();

    static glm::mat3 matrix = glm::mat3(1.0f);

    ImGui::InputFloat3("##NoTitleBarMat1", &matrix[0][0]);
    ImGui::InputFloat3("##NoTitleBarMat2", &matrix[1][0]);
    ImGui::InputFloat3("##NoTitleBarMat3", &matrix[2][0]);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.87f, 0.776f, 1.0f));
    if (ImGui::Button("Transform"))
    {
        std::thread transformationThread(&GUI::transformSystem, this, matrix);
        transformationThread.detach();
        matrix = glm::mat3(1.0f);
    }
    ImGui::PopStyleColor();

    ImGui::Spacing();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.871f, 0.776f, 1.0f));

    if (!renderer.vectors.empty())
    {
        ImGui::PushFont(mediumFont);
        ImGui::Text("Vectors: ");
        ImGui::PopFont();
        renderVectors();
    }

    ImGui::PushFont(mediumFont);
    ImGui::Text("Transformation Matrix: ");
    ImGui::PopStyleColor();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.514f, 0.263f, 1.0f));
    ImGui::Text("(?)");
    ImGui::PopFont();

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("This matrix transforms the system.");
    }
    ImGui::PopStyleColor();

    renderTransformationMatrices();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.87f, 0.776f, 1.0f));
    if (ImGui::Button("Reset The Transformations"))
    {
        resetTransformations();
    }
    ImGui::PopStyleColor();

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.87f, 0.776f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(1.0f, 0.514f, 0.263f, 1.0f));
    ImGui::Checkbox("Show System Array", &renderer.showSystemArray);
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
}

void GUI::renderLayoutCrossProduct()
{
    static glm::vec3 vector1(0.0f, 0.0f, 0.0f);
    ImGui::InputFloat3("##NoTitleBarVec1", &vector1[0]);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.87f, 0.776f, 1.0f));
    ImGui::PushFont(bigFont);
    ImGui::SetCursorPosX(140);
    ImGui::Text("X");
    ImGui::PopFont();
    ImGui::PopStyleColor();

    static glm::vec3 vector2(0.0f, 0.0f, 0.0f);
    ImGui::InputFloat3("##NoTitleBarVec2", &vector2[0]);

    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.945f, 0.87f, 0.776f, 1.0f));
    if (ImGui::Button("Cross Product"))
    {
        renderer.vectors.clear();
        renderer.registerRenderable(Vector(glm::vec3(0.0f), vector1, glm::vec3(0.5, 0.5, 0.5)));
        renderer.registerRenderable(Vector(glm::vec3(0.0f), vector2, glm::vec3(0.5, 0.5, 0.5)));

        renderer.registerRenderable(Vector(glm::vec3(0.0f), glm::cross(vector1, vector2), glm::vec3(0.6, 0.2, 0.8)));

        vector1 = glm::vec3(0.0f);
        vector2 = glm::vec3(0.0f);
    }
    ImGui::PopStyleColor();
}

void GUI::renderVectors()
{
    int sectionHeight;
    int maxVectorsShowing = 5;
    if (renderer.vectors.size() >= maxVectorsShowing)
    {
        sectionHeight = 34 * maxVectorsShowing + 8;
    }
    else
    {
        sectionHeight = 34 * renderer.vectors.size() + 8;
    }

    ImGui::BeginChild("VectorsScrollableSection", ImVec2(380, sectionHeight), false);

    // Use a standard for loop so we can safely modify the vector
    for (int id = 0; id < renderer.vectors.size(); id++)
    {
        auto &v = renderer.vectors[id]; // Access the vector element by index
        glm::vec3 vectorColor = v.vectorBody.material.diffuse;
        ImVec4 color = ImVec4(vectorColor.r, vectorColor.g, vectorColor.b, 1.0f);

        ImGui::PushFont(bigFont);
        ImGui::Text("(%.2f, %.2f, %.2f)", v.front.x, v.front.y, v.front.z);
        ImGui::PopFont();

        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(color.x * 0.8f, color.y * 0.8f, color.z * 0.8f, color.w));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(color.x * 0.6f, color.y * 0.6f, color.z * 0.6f, color.w));

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.09f, 0.6f, 0.682f, 1.0f));
        if (ImGui::Button((std::string("##ColorButton") + std::to_string(id)).c_str(), ImVec2(26, 26)))
        {
            ImGui::OpenPopup((std::string("ColorPickerPopup") + std::to_string(id)).c_str());
        }
        ImGui::PopStyleColor();

        ImGui::PopStyleColor(3);

        ImGui::SameLine();

        ImGui::PushFont(microFont);
        if (ImGui::Button((std::string("X##") + std::to_string(id)).c_str()))
        {
            renderer.vectors.erase(renderer.vectors.begin() + id);
        }
        ImGui::PopFont();

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.09f, 0.6f, 0.682f, 1.0f));
        if (ImGui::BeginPopup((std::string("ColorPickerPopup") + std::to_string(id)).c_str()))
        {
            ImGui::ColorPicker4((std::string("Color Picker") + std::to_string(id)).c_str(), (float *)&color);
            v.setVectorColor(glm::vec3(color.x, color.y, color.z));
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor();
    }

    ImGui::EndChild();
}

void GUI::renderTransformationMatrices()
{
    ImGui::BeginChild("MatrixTransformationsScrollableSection", ImVec2(380, ImGui::GetTextLineHeightWithSpacing() * 5.5f), false, ImGuiWindowFlags_HorizontalScrollbar);

    int xOffset = 0;
    int yOffset = 25;
    if (!transformationMatrices.empty())
    {
        for (auto matrix : transformationMatrices)
        {
            ImGui::SetCursorPos(ImVec2(xOffset, yOffset));

            matrix.renderMatrix(xOffset);
            xOffset += 250;
        }

        xOffset += 30;

        ImGui::SetCursorPos(ImVec2(xOffset, yOffset));

        finalTransformationMatrix.setEqualsDrawList();
    }
    else
    {
        ImGui::SetCursorPos(ImVec2(xOffset, yOffset));
    }

    finalTransformationMatrix.renderMatrix(xOffset);
    ImGui::SetCursorPosX(xOffset + 240);
    ImGui::EndChild();
}

void GUI::transformSystem(glm::mat3 transformationMatrix)
{
    constexpr float steps = 20;
    glm::mat3 baseMatrix = (transformationMatrix - glm::mat3(1.0f)) / steps;
    glm::mat3 tempMatrix = glm::mat3(1.0f) + baseMatrix;

    for (float i = 1; i < steps; i++)
    {
        finalTransformationMatrix.matrix = (tempMatrix)*finalTransformationMatrix.matrix;
        renderer.transformVectors(tempMatrix);
        transformationMonitor.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(20));

        transformationMonitor.lock();
        finalTransformationMatrix.matrix = glm::inverse(tempMatrix) * finalTransformationMatrix.matrix;
        renderer.transformVectors(glm::inverse(tempMatrix));

        tempMatrix += baseMatrix;
    }
    finalTransformationMatrix.matrix = (tempMatrix)*finalTransformationMatrix.matrix;
    renderer.transformVectors(tempMatrix);
    transformationMonitor.unlock();
    transformationMatrices.insert(transformationMatrices.begin(), GUIMatrix(transformationMatrix));
}

void GUI::resetTransformations()
{
    renderer.transformVectors(glm::inverse(finalTransformationMatrix.matrix));
    transformationMatrices.clear();
    finalTransformationMatrix = glm::mat3(1.0f);
}