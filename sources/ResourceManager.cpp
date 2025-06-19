#include "ResourceManager.h"

const aiScene *ResourceManager::getScene(std::string path)
{
    static Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_CalcTangentSpace |
                                                 aiProcess_Triangulate |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    return scene;
}

Object ResourceManager::makeObject(aiMesh *mMesh, aiMaterial *mat)
{
    std::vector<glm::vec3> indeksiraniVrhovi;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    aiColor3D ambient, diffuse, specular;
    float shininess;

    mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
    mat->Get(AI_MATKEY_COLOR_SPECULAR, specular);
    mat->Get(AI_MATKEY_SHININESS, shininess);

    Material material = Material(glm::vec3(ambient.r, ambient.g, ambient.b),
                                 glm::vec3(diffuse.r, diffuse.g, diffuse.b),
                                 glm::vec3(specular.r, specular.g, specular.b),
                                 shininess);

    for (unsigned int i = 0; i < mMesh->mNumVertices; i++)
    {
        aiVector3D vec = mMesh->mVertices[i];
        indeksiraniVrhovi.push_back(glm::vec3(vec.x, vec.y, vec.z));

        vec = mMesh->mNormals[i];
        normals.push_back(glm::vec3(vec.x, vec.y, vec.z));
    }

    for (unsigned int i = 0; i < mMesh->mNumFaces; i++)
    {
        const aiFace face = mMesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Object(TriangleMesh(indeksiraniVrhovi, indices, normals), material);
}