#pragma once

// forward declarations
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Window.h"

class Renderer {
public:
  virtual ~Renderer();

  // Setup
  virtual bool SetupViewport(Window* window) = 0;
  virtual void ClearWindow() = 0;

  // Rendering
  virtual void UseTexture(const Texture *texture) = 0;
  virtual void UseTexture(const Texture *texture, unsigned char textureUnit) = 0;
  virtual void UseShader(const Shader *shader) = 0;
  virtual void UseMaterial(const Material *material) = 0;
  virtual void RenderMesh(const Mesh *mesh) = 0;

  // Loading and binding

  // void SubmitModel(const Model* model);
  virtual void SubmitMesh(const Mesh *mesh) = 0;
  virtual void SubmitTexture(const Texture *texture) = 0;
  // void ClearModel(const Model* model);
  virtual void ClearMesh(const Mesh *mesh) = 0;
  virtual void ClearTexture(const Texture *texture) = 0;
};
