#pragma once

#include <unordered_map>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ShadowMap.h"

class LightManager {
public:
  LightManager();
  ~LightManager();

  void AddLight(DirectionalLight* toAdd);
  void AddLight(PointLight* toAdd);
  void AddLight(SpotLight* toAdd);
private:
  // TODO remove restrictions
  DirectionalLight* dLight;
  std::unordered_map<LightId, PointLight*> pointLightMap;
  std::unordered_map<LightId, SpotLight*> spotLightMap;
};

