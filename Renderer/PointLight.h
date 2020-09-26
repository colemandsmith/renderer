#pragma once

#include <vector>

#include "Light.h"
#include "OmniShadowMap.h"

class PointLight : public Light {
public:
	PointLight();

	PointLight(GLuint shadowWidth, GLuint shadowHeight,
               GLfloat near, GLfloat far,
               GLfloat red, GLfloat green, GLfloat blue,
			   GLfloat amIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
		       GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		          GLuint diffuseIntensityLocation, GLuint positionLocation,
	        	  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    std::vector<glm::mat4> CalculateLightTransform();

    GLfloat GetFarPlane();
    glm::vec3 GetPosition();

	~PointLight();
protected:
	glm::vec3 position;

	GLfloat constant, linear, exponent;

    GLfloat nearPlane, farPlane;

};
