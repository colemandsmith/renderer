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
    /** Constructors to read in a simple shape with a single mesh, a single texture,
      * and optionally a normal map 
      */
    Model(Mesh* mesh, Texture* texture);
    Model(Mesh* mesh, Texture* texture, Texture* normalMap);

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
    void LoadTextures(const aiScene *scene, bool withNormalMap);

    std::vector<Mesh*> meshList;
    std::vector<Texture*> textureList;
    std::vector<Texture*> normalMapList;
    // Maps the mesh to the texture in the texture list
    std::vector<unsigned int> meshToTex;
    bool hasNormalMaps;
    // Maps the mesh to the normal map texture in the normal map list
    std::vector<unsigned int> meshToNormalMap;
};