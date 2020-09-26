#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
    lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLuint shadowWidth, GLuint shadowHeight,
                                   GLfloat red, GLfloat green, GLfloat blue,
								   GLfloat amIntensity, GLfloat dIntensity,
	                               GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(shadowWidth, shadowHeight, red, green, blue, amIntensity, dIntensity) {
	direction = glm::vec3(xDir, yDir, zDir);
    lightProj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
	                            GLuint diffuseIntensityLocation, GLuint directionLocation) {
	glUniform3f(ambientColorLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}

glm::mat4 DirectionalLight::CalculateLightTransform() {
    // want to look at the origin in this space
    return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight() {

}