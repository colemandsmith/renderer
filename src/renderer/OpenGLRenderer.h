#pragma once

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Renderer.h"
#include "Camera.h"
#include "CommonValues.h"
#include "Mesh.h"
#include "RenderObject.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "SpotLight.h"
#include "Texture.h"
#include "Window.h"

struct MeshBindingData {
  unsigned int VAO = 0;
  unsigned int VBO = 0;
  unsigned int IBO = 0;
  int indexCount = 0;
};

class OpenGLRenderer : public Renderer {
public:
  OpenGLRenderer();
  ~OpenGLRenderer();

  // ------ Setup ------
  bool SetupViewport(Window* window) override;

  // ------ Rendering ------

  // Shader and uniform management
  void UseShader(const Shader *shader) override;
  void UseMaterial(const Material *material) override;

  // Geometry and textures
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
  ShaderId currentShader;
  ShaderManager shaderManager;
  void RenderScene(Shader *shader);
  std::unordered_map<const Mesh *, MeshBindingData> meshBindings;
  std::unordered_map<const Texture *, unsigned int> textureBindings;
};
