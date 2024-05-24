#pragma once
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "RenderObject.h"
#include "SpotLight.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "CommonValues.h"

struct RenderOp {
  Shader* shader;
};

struct MeshBindingData {
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int IBO = 0;
    int indexCount = 0;
};

class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();

    void Render(const RenderOp& renderOp);

    // Rendering

    // Loading and binding
    // void SubmitModel(const Model* model);
    void SubmitMesh(const Mesh* mesh);
    void SubmitTexture(const Texture* texture);
    // void ClearModel(const Model* model);
    void ClearMesh(const Mesh* mesh);
    void ClearTexture(const Texture* texture);
    // void SetAmbientLight(float red, float green, float blue);
    
private:
    void RenderScene(Shader* shader);
    std::unordered_map<const Mesh*, MeshBindingData> meshBindings;
    std::unordered_map<const Texture*, unsigned int> textureBindings;
};
