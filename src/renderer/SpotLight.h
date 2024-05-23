#pragma once
#include "PointLight.h"
class SpotLight : public PointLight {
public:
    SpotLight();

    SpotLight(unsigned int shadowWidth, unsigned int shadowHeight,
              float near, float far,
              float red, float green, float blue,
              float amIntensity, float dIntensity,
              float xPos, float yPos, float zPos,
              float xDir, float yDir, float zDir,
              float con, float lin, float exp,
              float edge);

    void UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                  unsigned int diffuseIntensityLocation, unsigned int positionLocation, unsigned int directionLocation,
                  unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation,
                  unsigned int edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir);
    void Toggle();
    const inline bool IsOn() { return isOn; }

    ~SpotLight();
private:
    glm::vec3 direction;

    float edge, processedEdge;

    bool isOn;
};
