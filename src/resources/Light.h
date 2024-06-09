#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShadowMap.h"

enum LightType {
  DIRECTIONAL_LIGHT,
  POINT_LIGHT,
  SPOT_LIGHT
};

typedef unsigned int LightId;

class Light {
public:
  Light();
  Light(unsigned int shadowWidth, unsigned int shadowHeight, float red,
        float green, float blue, float amIntensity, float dIntensity);

  virtual ~Light() = 0;

protected:
  glm::vec3 color;
  float ambientIntensity;
  float diffuseIntensity;

  glm::mat4 lightProj;
};
