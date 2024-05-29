#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Camera.h"
#include "CommonValues.h"
#include "Mesh.h"
#include "RenderObject.h"
#include "Renderer.h"
#include "Shader.h"
#include "SpotLight.h"
#include "Texture.h"

struct MeshBindingData {
  unsigned int VAO = 0;
  unsigned int VBO = 0;
  unsigned int IBO = 0;
  int indexCount = 0;
};

class OpenGLRenderer : public RendererInterface {
public:
  OpenGLRenderer();
  ~OpenGLRenderer();

  //  ------ Rendering ------

  // Shader and uniform management
  void UseShader();
  void UseMaterial(const Material *material) override;

  //
  void UseTexture(const Texture *texture) override;
  void UseTexture(const Texture *texture, unsigned char textureUnit) override;
  void RenderMesh(const Mesh *mesh) override;

  // ------ Loading and binding ------

  // void SubmitModel(const Model* model);
  void SubmitMesh(const Mesh *mesh) override;
  void SubmitTexture(const Texture *texture) override;
  // void ClearModel(const Model* model);
  void ClearMesh(const Mesh *mesh) override;
  void ClearTexture(const Texture *texture) override;

private:
  void RenderScene(Shader *shader);
  std::unordered_map<const Mesh *, MeshBindingData> meshBindings;
  std::unordered_map<const Texture *, unsigned int> textureBindings;
};
