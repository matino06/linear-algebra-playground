#include "Object.h"

Object::Object(TriangleMesh mesh, Material material)
    : Transform(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f)), material(material), mesh(mesh), shader(loadShader()) {}

Object::~Object() {}

void Object::render(glm::vec3 cameraPosition, glm::mat4 perspectiveMatrix, glm::mat4 viewMatrix, Light light)
{
    glBindVertexArray(mesh.VAO);

    shader->setUniform("viewPos", cameraPosition);
    shader->setUniform("lightPos", light.position);
    shader->setUniform("lightAmbient", light.ambientIntensity);
    shader->setUniform("lightColor", light.intensity);

    shader->setUniform("materialAmbient", material.ambient);
    shader->setUniform("materialDiffuse", material.diffuse);
    shader->setUniform("materialSpecular", material.specular);
    shader->setUniform("materialShininess", material.shininess);

    shader->setUniform("mMatrix", getModelMatrix());
    shader->setUniform("vMatrix", viewMatrix);
    shader->setUniform("pMatrix", perspectiveMatrix);

    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
}

Shader *Object::loadShader()
{
    const std::string pathVert = "../shaders/shader.vert";
    const std::string pathFrag = "../shaders/shader.frag";
    // std::string pathGeom = "../shaders/shader.geom";

    return new Shader(pathVert.c_str(), pathFrag.c_str(), nullptr);
}
