#pragma once
#include <vector>
#include <unordered_map>

#include <glm/glm.hpp>

#include "RenderObject.h"
#include "SpotLight.h"
#include "Camera.h"
#include "CommonValues.h"

struct RenderOp {
  Shader* shader;
};

struct MeshBindingData {
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    GLsizei indexCount;
};

class OpenGLRenderer {
public:
    OpenGLRenderer();

    void Render(const RenderOp& renderOp);

    void SubmitModel(const Model* model);
    void SubmitMesh(const Mesh* mesh);
    void SubmitTexture(const Texture* texture);
    void ClearModel(const Model* model);
    void ClearMesh(const Mesh* mesh);
    void ClearTexture(const Texture* texture);
    
private:
    void RenderScene(Shader* shader);
    std::unordered_map<Mesh*, MeshBindingData> meshBindings;
    std::unordered_map<Texture*, GLuint> textureBindings;
};
