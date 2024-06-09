#pragma once

#include <unordered_map>

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "Light.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ShadowMap.h"
#include "OmniShadowMap.h"

class LightManager {
public:
  LightManager();
  ~LightManager();

  void AddLight(DirectionalLight* toAdd);
  void AddLight(PointLight* toAdd);
  void AddLight(SpotLight* toAdd);

private:
  // TODO remove restrictions
  DirectionalLight* dLight = nullptr;
  ShadowMap* directionalShadowMap = nullptr;
  std::unordered_map<PointLight*, OmniShadowMap*> pointLightToShadowMap;
  std::unordered_map<SpotLight*, OmniShadowMap*> spotLightToShadowMap;
};

