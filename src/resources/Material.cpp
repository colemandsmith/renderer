#include "Material.h"

Material::Material(const ShaderId _shaderId) {
  shaderId = _shaderId;
  specularIntensity = 0.0f;
  shininess = 0.0f;
}

Material::Material(const ShaderId _shaderId, const float sIntensity,
                   const float shine) {
  shaderId = _shaderId;
  specularIntensity = sIntensity;
  shininess = shine;
}

Material::~Material() {}
