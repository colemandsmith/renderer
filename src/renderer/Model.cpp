#include "Model.h"

Model::Model() {

}

Model::Model(Mesh* mesh, Texture* texture) {
    meshList.push_back(mesh);
    textureList.push_back(texture);
    meshToTex.push_back(0);
}

void Model::LoadModel(const std::string fileName) {
    LoadModelFromFile(fileName, false);
}

void Model::LoadModelWithNormalMap(const std::string fileName) {
    LoadModelFromFile(fileName, true);
}

void Model::RenderModel() {
    for (size_t i = 0; i < meshList.size(); i++) {
        unsigned int materialIndex = meshToTex[i];
        if (materialIndex < textureList.size() && textureList[materialIndex]) {
            textureList[materialIndex]->UseTexture();
        }
        if (hasNormalMaps) {
            unsigned int normalMapIndex = meshToNormalMap[i];
            if (normalMapIndex < normalMapList.size() && normalMapList[normalMapIndex]) {
                normalMapList[normalMapIndex]->UseTexture(GL_TEXTURE2);
            }
        }

        meshList[i]->RenderMesh();
    }
}

void Model::ClearModel() {
    for (size_t i = 0; i < meshList.size(); i++) {
        if (meshList[i]) {
            delete meshList[i];
            meshList[i] = nullptr;
        }
    }

    for (size_t i = 0; i < textureList.size(); i++) {
        if (textureList[i]) {
            delete textureList[i];
            textureList[i] = nullptr;
        }
    }

    for (size_t i = 0; i < normalMapList.size(); i++) {
        if (normalMapList[i]) {
            delete normalMapList[i];
            normalMapList[i] = nullptr;
        }
    }
}

void Model::LoadModelFromFile(const std::string fileName, bool withNormalMap) {
    Assimp::Importer importer;
    unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (withNormalMap) {
        flags |= aiProcess_CalcTangentSpace;
    }
    else {
        flags |= aiProcess_GenSmoothNormals;
    }
    const aiScene* scene = importer.ReadFile(fileName, flags);
    if (!scene) {
        printf("Model (%s) failed to load: %s\n", fileName.c_str(), importer.GetErrorString());
        return;
    }

    LoadNode(scene->mRootNode, scene, withNormalMap);

    LoadTextures(scene, withNormalMap);
}

void Model::LoadNode(aiNode* node, const aiScene* scene, bool withTangents) {
    for (size_t i = 0; i < node->mNumMeshes; i++) {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene, withTangents);
    }
    for (size_t i = 0; i < node->mNumChildren; i++) {
        LoadNode(node->mChildren[i], scene, withTangents);
    }
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene, bool withTangents) {
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++) {
        // vertex position
        vertices.insert(vertices.end(), { mesh->mVertices[i].x,  mesh->mVertices[i].y, mesh->mVertices[i].z });
        // vertex texture coords
        if (mesh->mTextureCoords[0]) {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }
        else {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }
        // vertex normals
        vertices.insert(vertices.end(), { -mesh->mNormals[i].x,  -mesh->mNormals[i].y, -mesh->mNormals[i].z });
        // vertex tangents
        if (withTangents) {
            vertices.insert(vertices.end(), {
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            });
        }
        else {
            vertices.insert(vertices.end(), {
                0.0f, 0.0f, 0.0f
            });
        }
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh* newMesh = new Mesh();
    newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
    meshList.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);
    meshToNormalMap.push_back(mesh->mMaterialIndex);
}

void Model::LoadTextures(const aiScene* scene, bool withNormalMap) {
    textureList.resize(scene->mNumMaterials);
    normalMapList.resize(scene->mNumMaterials);

    for (size_t i = 0; i < scene->mNumMaterials; i++) {
        aiMaterial* material = scene->mMaterials[i];

        textureList[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
                int idx = std::string(path.data).rfind("\\");
                std::string fname = std::string(path.data).substr(idx + 1);

                std::string texPath = std::string("Textures/") + fname;
                textureList[i] = new Texture(texPath.c_str());

                if (!textureList[i]->LoadTexture()) {
                    printf("Failed to load texture at: %s", texPath.c_str());
                    delete textureList[i];
                    textureList[i] = nullptr;
                }
            }
        }
        // for obj format, normals get brought in via aiTextureType_HEIGHT
        // only officially support obj for now so this is the code path to follow
        if (withNormalMap && material->GetTextureCount(aiTextureType_HEIGHT)) {
            aiString path;
            if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS) {
                // Ignore everything except the last part of the path
                // to account for a model creator's local filepaths having
                // been saved in the model
                int idx = std::string(path.data).rfind("\\");
                std::string fname = std::string(path.data).substr(idx + 1);

                std::string texPath = std::string("Textures/") + fname;
                normalMapList[i] = new Texture(texPath.c_str());

                if (!normalMapList[i]->LoadTexture()) {
                    printf("Failed to load texture at: %s", texPath.c_str());
                    delete normalMapList[i];
                    normalMapList[i] = nullptr;
                }
                else {
                    hasNormalMaps = true;
                }
            }
        }

        if (!textureList[i]) {
            textureList[i] = new Texture("Textures/plain.png");
            textureList[i]->LoadTextureA();
        }
    }
}

Model::~Model() {
    ClearModel();
}