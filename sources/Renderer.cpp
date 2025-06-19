#include "Renderer.h"

Renderer::Renderer() : camera(Camera(glm::vec3(6.0f, 5.0f, 30.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1))), light(Light(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.8f)))
{
    preRender();
    glfwSetWindowUserPointer(window, &camera);
}

Renderer::~Renderer() {};

void Renderer::preRender()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Needed for macOS compatibility
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create a GLFWwindow object
    window = glfwCreateWindow(INITIAL_WIDTH, INITIAL_HEIGHT, "Linear Algebra Playground", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_MULTISAMPLE); // anti-aliasing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
    glEnable(GL_CULL_FACE);

    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(window, InputManager::framebufferSizeCallback);
    glfwSetScrollCallback(window, InputManager::scrollCallback);
}

void Renderer::renderAll()
{
    for (Object &c : coordinateSystem)
    {
        c.shader->use();
        c.transformationMatrix = *transformationMatrix;
        c.render(camera.position, camera.getPerspectiveMatrix(window), camera.getViewMatrix(), light);
    }

    if (showSystemArray)
    {
        for (Object &s : systemArray)
        {
            s.shader->use();
            s.transformationMatrix = *transformationMatrix;
            s.render(camera.position, camera.getPerspectiveMatrix(window), camera.getViewMatrix(), light);
        }
    }

    for (Vector &v : vectors)
    {
        Object vectorBody = v.getVectorBody();
        vectorBody.shader->use();
        vectorBody.render(camera.position, camera.getPerspectiveMatrix(window), camera.getViewMatrix(), light);

        Object vectorHead = v.getVectorHead();
        vectorHead.shader->use();
        vectorHead.render(camera.position, camera.getPerspectiveMatrix(window), camera.getViewMatrix(), light);
    }
}

void Renderer::registerRenderable(Vector vector)
{
    vectors.push_back(vector);
}

std::vector<Object> Renderer::registerRenderable(std::string path)
{
    std::vector<Object> objects;
    const aiScene *scene = ResourceManager::getScene(path);
    for (int i = 0; i < scene->mNumMeshes; i++)
    {
        aiMesh *mMesh = scene->mMeshes[i];
        objects.push_back(ResourceManager::makeObject(mMesh, scene->mMaterials[mMesh->mMaterialIndex]));
    }

    return objects;
}

void Renderer::registerSystem(std::string coordinateSystemPath, std::string systemArrayPath)
{
    coordinateSystem = registerRenderable(coordinateSystemPath);
    systemArray = registerRenderable(systemArrayPath);
}

void Renderer::transformVectors(glm::mat3 transformationMatrix)
{
    for (Vector &v : vectors)
    {
        v.front = glm::vec3(transformationMatrix * v.front);
        v.position = glm::vec3(transformationMatrix * v.position);
        // v.position = glm::vec3(glm::vec3(transformationMatrix[0][3], transformationMatrix[1][3], transformationMatrix[2][3]) + v.position);  Translacija

        v.updateVector();
    }
}

GLFWwindow *Renderer::getWindow()
{
    return window;
}

void Renderer::reset()
{
    vectors.clear();
}
