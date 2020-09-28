#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "ShadowMap.h";

class Light {
public:
	Light();
	Light(
        GLuint shadowWidth, GLuint shadowHeight,
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat amIntensity, GLfloat dIntensity
    );

    ShadowMap* GetShadowMap() { return shadowMap; };

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;

    glm::mat4 lightProj;

    ShadowMap* shadowMap;
};

