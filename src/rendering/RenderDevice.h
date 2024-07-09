#pragma once

#include "rendering/Window.h"
#include "resources/Material.h"
#include "resources/Mesh.h"
#include "resources/PointLight.h"
#include "resources/SpotLight.h"
#include "resources/Texture.h"

class RenderDevice {
  virtual ~RenderDevice();

  // void SubmitModel(const Model* model);
  virtual void SubmitMesh(const Mesh *mesh) = 0;
  virtual void SubmitTexture(const Texture *texture) = 0;
  // void ClearModel(const Model* model);
  virtual void ClearMesh(const Mesh *mesh) = 0;
  virtual void ClearTexture(const Texture *texture) = 0;
};
