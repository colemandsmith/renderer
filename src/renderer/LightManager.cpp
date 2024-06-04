#include <iostream>
#include "LightManager.h"

LightManager::LightManager() {

}

void LightManager::AddLight(DirectionalLight* toAdd) {
  dLight = toAdd;
}

void LightManager::AddLight(PointLight* toAdd) {
}

void LightManager::AddLight(SpotLight* toAdd) {
}

LightManager::~LightManager() {

}
