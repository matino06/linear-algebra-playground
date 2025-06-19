#include "Vector.h"

Vector::Vector(glm::vec3 position, glm::vec3 front, glm::vec3 color)
    : position(position), front(front),
      vectorBody(makeVectorPart(VectorComponent::VectorBody)),
      vectorHead(makeVectorPart(VectorComponent::VectorHead))
{
    updateVector();
    setVectorColor(color);
}

Vector::~Vector() {}

void Vector::updateVector()
{
    const auto [min, max] = vectorHead.mesh.getBoundingBox();
    const float dy = max[1] - min[1];
    const float vectorLength = std::hypot(front.x, front.y, front.z);
    const float scalingFactor = 1 / (1 - dy);

    vectorBody.front = front;
    vectorBody.position = position;

    vectorHead.front = front;
    vectorHead.position = glm::normalize(front) * (vectorLength - 1) + position;

    vectorBody.scale[1] = scalingFactor * (vectorLength - dy);
}

Object Vector::getVectorBody()
{
    return vectorBody;
}

Object Vector::getVectorHead()
{
    return vectorHead;
}

Object Vector::makeVectorPart(VectorComponent part)
{
    const aiScene *scene = ResourceManager::getScene("../resources/vector/vector.obj");
    // part = 0 -> body, part = 1 -> head
    aiMesh *mMesh = scene->mMeshes[static_cast<int>(part)];
    return ResourceManager::makeObject(mMesh, scene->mMaterials[mMesh->mMaterialIndex]);
}

void Vector::setVectorColor(glm::vec3 color)
{
    vectorBody.material.diffuse = color;
    vectorHead.material.diffuse = color;
}