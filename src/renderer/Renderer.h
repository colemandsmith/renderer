#pragma once

// forward declarations
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"

class RendererInterface {
public:
  virtual void UseTexture(const Texture *texture) = 0;
  virtual void UseTexture(const Texture *texture, unsigned char textureUnit);
  virtual void UseShader(const Shader *shader);
  virtual void UseMaterial(const Material *material);
  virtual void RenderMesh(const Mesh *mesh) = 0;

  // Loading and binding

  // void SubmitModel(const Model* model);
  virtual void SubmitMesh(const Mesh *mesh) = 0;
  virtual void SubmitTexture(const Texture *texture) = 0;
  // void ClearModel(const Model* model);
  virtual void ClearMesh(const Mesh *mesh) = 0;
  virtual void ClearTexture(const Texture *texture) = 0;
  virtual void AddDirectionalLight(const DirectionalLight* dLight) = 0;
};
