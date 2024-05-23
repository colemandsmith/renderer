#include "Light.h"

Light::Light() {
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(unsigned int shadowWidth, unsigned int shadowHeight, float red, float green, float blue, float amIntensity, float dIntensity) {
    shadowMap = new ShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);

    color = glm::vec3(red, green, blue);
	ambientIntensity = amIntensity;
	diffuseIntensity = dIntensity;
}

Light::~Light() {
    // delete shadowMap;
}
