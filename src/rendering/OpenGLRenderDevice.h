#pragma once

#include "RenderDevice.h"

class OpenGLRenderDevice : public RenderDevice {
public:
  OpenGLRenderDevice();
  ~OpenGLRenderDevice();

  // void SubmitModel(const Model* model);
  virtual void SubmitMesh(const Mesh *mesh) override;
  virtual void SubmitTexture(const Texture *texture) override;
  // void ClearModel(const Model* model);
  virtual void ClearMesh(const Mesh *mesh) override;
  virtual void ClearTexture(const Texture *texture) override;
};
