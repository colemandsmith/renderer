#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "ShadowMap.h"

class Light {
public:
	Light();
	Light(
        unsigned int shadowWidth, unsigned int shadowHeight,
        float red, float green, float blue,
        float amIntensity, float dIntensity
    );

    ShadowMap* GetShadowMap() { return shadowMap; };

	~Light();

protected:
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;

    glm::mat4 lightProj;

    ShadowMap* shadowMap;
};

