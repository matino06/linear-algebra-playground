#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(std::vector<glm::vec3> vertices, std::vector<unsigned int> indices, std::vector<glm::vec3> normals)
    : vertices(vertices), normals(normals), indices(indices), vertexCount(vertices.size()), indexCount(indices.size())
{
    initializeBuffers();
}

TriangleMesh::~TriangleMesh() {}

void TriangleMesh::initializeBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // buffer za koordinate i povezi s nultim mjestom u VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexCount, &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

    // buffer za normale i povezi s drugim mjestom u VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertexCount, &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // buffer za indekse
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, (void *)(&indices[0]), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

std::pair<glm::vec3, glm::vec3> TriangleMesh::getBoundingBox()
{
    const auto [minX, maxX] = std::minmax_element(vertices.cbegin(), vertices.cend(),
                                                  [](const glm::vec3 &a, const glm::vec3 &b)
                                                  { return a.x < b.x; });

    const auto [minY, maxY] = std::minmax_element(vertices.cbegin(), vertices.cend(),
                                                  [](const glm::vec3 &a, const glm::vec3 &b)
                                                  { return a.y < b.y; });

    const auto [minZ, maxZ] = std::minmax_element(vertices.cbegin(), vertices.cend(),
                                                  [](const glm::vec3 &a, const glm::vec3 &b)
                                                  { return a.z < b.z; });

    return {glm::vec3(minX->x, minY->y, minZ->z), glm::vec3(maxX->x, maxY->y, maxZ->z)};
}
