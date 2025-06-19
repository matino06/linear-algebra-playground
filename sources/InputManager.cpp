#include "InputManager.h"

void InputManager::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void InputManager::mouseCallback(GLFWwindow *window, Camera *camera)
{
    static double initX, initY; // Static to retain value between function calls
    static bool isDragging = false;

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Check mouse button state
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!isDragging) // Start dragging
        {
            glfwGetCursorPos(window, &initX, &initY);
            if (initX > width * 0.67f)
            {
                return;
            }
            isDragging = true;
        }

        // Handle dragging
        if (isDragging)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Calculate rotation
            const float rotX = sensitivity * (float)(initY - mouseY) / height;
            const float rotY = sensitivity * (float)(mouseX - initX) / width;

            // Rotate camera
            glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotX), glm::normalize(glm::cross(glm::vec3(0.0f) - camera->front, camera->up)));
            glm::vec4 rotatedPosition = glm::vec4(camera->position, 0.0f) * rotationX;
            glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotY), camera->up);
            rotatedPosition = rotatedPosition * rotationY;

            glm::vec3 newPosition = glm::vec3(rotatedPosition);
            glm::vec3 newFront = glm::normalize(glm::vec3(0.0f) - newPosition);

            // Prevent camera flipping
            if (glm::angle(newFront, camera->up) <= glm::radians(5.0f) || glm::angle(newFront, -camera->up) <= glm::radians(5.0f))
            {
                newPosition = camera->position;
                newFront = camera->front;
            }

            camera->position = newPosition;
            camera->front = newFront;

            // Update initial position
            initX = mouseX;
            initY = mouseY;
        }
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        isDragging = false;
    }
}

void InputManager::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    static double mouseX;
    static int width, height;
    glfwGetCursorPos(window, &mouseX, nullptr);
    glfwGetWindowSize(window, &width, nullptr);

    if (mouseX > width * 0.75)
    {
        return;
    }

    static Camera *camera = static_cast<Camera *>(glfwGetWindowUserPointer(window));

    const glm::vec3 newCamPosition = camera->position + camera->front * ((float)yoffset * 0.05f);
    const float newCamDistance = std::hypot(newCamPosition[0], newCamPosition[1], newCamPosition[2]);

    if (newCamDistance < 45 and newCamDistance > 2)
    {
        camera->position += camera->front * ((float)yoffset * 0.05f);
    }
}
