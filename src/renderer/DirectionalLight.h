#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
	DirectionalLight();
	DirectionalLight(
        unsigned int shadowWidth, unsigned int shadowHeight,
        float red, float green, float blue,
        float amIntensity, float dIntensity,
        float xDir, float yDir, float zDir
    );

	void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation, unsigned int diffuseIntensityLocation, unsigned int directionLocation);

    glm::mat4 CalculateLightTransform();

    ~DirectionalLight();

private:
	glm::vec3 direction;
};

