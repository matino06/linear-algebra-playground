#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Local Headers
#include "GUI.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Vector.h"
#include "Monitor.h"

// System Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

////////////////////////////////
////////// MACOS ONLY //////////
////////////////////////////////

int main(int argc, char *argv[])
{
    Monitor transformationMonitor;
    Renderer renderer;
    GLFWwindow *window = renderer.getWindow();
    GUI gui(window, renderer, transformationMonitor);

    renderer.registerSystem("../resources/coordinateSystem/coordinateSystem.obj", "../resources/coordinateSystem/systemArray.obj");

    while (glfwWindowShouldClose(window) == false)
    {
        glCullFace(GL_BACK);

        glClearColor(0.9450980392156862f, 0.8705882352941177f, 0.7764705882352941f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width * 0.67, height);

        gui.newFrame();
        gui.renderLayout();
        gui.render();

        transformationMonitor.lock();
        renderer.renderAll();
        transformationMonitor.unlock();

        InputManager::mouseCallback(window, &renderer.camera);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    gui.~GUI();
    glfwTerminate();

    return EXIT_SUCCESS;
}
