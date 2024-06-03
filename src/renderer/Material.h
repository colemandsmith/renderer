#pragma once

#include "Shader.h"
#include <GL/glew.h>
#include <memory>

class Material {
public:
  Material(const ShaderId shaderId);
  Material(const ShaderId shaderId, const float sIntensity, const float shine);
  inline ShaderId GetShaderId() const { return shaderId; }
  inline float GetSpecularIntensity() const { return specularIntensity; }
  inline float GetShininess() const { return shininess; }
  ~Material();

private:
  float specularIntensity = 0.0f;
  float shininess = 0.0f;
  ShaderId shaderId = 0;
};
