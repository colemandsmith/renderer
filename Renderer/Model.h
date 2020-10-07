#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model {
public:
    Model();
    /** Constructor to read in a simple shame with a single mesh and single texture */
    Model(Mesh* mesh, Texture* texture);

    void LoadModel(const std::string fileName);
    void LoadModelWithNormalMap(const std::string fileName);
    void RenderModel();
    void ClearModel();
    bool HasNormalMaps() { return hasNormalMaps; }

    ~Model();

private:
    void LoadModelFromFile(const std::string fileName, bool withNormalMap);
    void LoadNode(aiNode *node, const aiScene *scene, bool withTangents);
    void LoadMesh(aiMesh *mesh, const aiScene *scene, bool withTangents);
    void LoadMaterials(const aiScene *scene, bool withNormalMap);

    std::vector<Mesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<Texture*> normalMapList;
    // Maps the mesh to the texture in the texture list
    std::vector<unsigned int> meshToTex;
    bool hasNormalMaps;
    std::vector<unsigned int> meshToNormalMap;
};