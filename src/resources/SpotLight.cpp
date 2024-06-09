#include "SpotLight.h"

SpotLight::SpotLight() : PointLight() {
  direction = glm::vec3(0.0f, -1.0f, 0.0f);
  edge = 0.0f;
  processedEdge = cosf(glm::radians(edge));
  isOn = true;
}

SpotLight::SpotLight(unsigned int shadowWidth, unsigned int shadowHeight,
                     float near, float far, float red, float green, float blue,
                     float amIntensity, float dIntensity, float xPos,
                     float yPos, float zPos, float xDir, float yDir, float zDir,
                     float con, float lin, float exp, float edge)
    : PointLight(shadowWidth, shadowHeight, near, far, red, green, blue,
                 amIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp) {
  direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
  this->edge = edge;
  processedEdge = cosf(glm::radians(edge));
  isOn = true;
}

void SpotLight::UseLight(
    unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
    unsigned int diffuseIntensityLocation, unsigned int positionLocation,
    unsigned int directionLocation, unsigned int constantLocation,
    unsigned int linearLocation, unsigned int exponentLocation,
    unsigned int edgeLocation) {
  glUniform3f(ambientColorLocation, color.x, color.y, color.z);
  if (isOn) {

    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
  } else {
    glUniform1f(ambientIntensityLocation, 0);
    glUniform1f(diffuseIntensityLocation, 0);
  }
  glUniform3f(positionLocation, position.x, position.y, position.z);
  glUniform1f(constantLocation, constant);
  glUniform1f(linearLocation, linear);
  glUniform1f(exponentLocation, exponent);

  glUniform3f(directionLocation, direction.x, direction.y, direction.z);
  glUniform1f(edgeLocation, processedEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir) {
  position = pos;
  direction = dir;
}

void SpotLight::Toggle() { isOn = !isOn; }

SpotLight::~SpotLight() {}
