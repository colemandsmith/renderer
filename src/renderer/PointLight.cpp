#include "PointLight.h"

PointLight::PointLight() : Light() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
    nearPlane = 0.0f;
    farPlane = 0.0f;
}

PointLight::PointLight(unsigned int shadowWidth, unsigned int shadowHeight,
                       float near, float far,
                       float red, float green, float blue,
                       float amIntensity, float dIntensity,
                       float xPos, float yPos, float zPos,
                       float con, float lin, float exp) : Light(shadowWidth, shadowHeight, red, green, blue, amIntensity, dIntensity) {
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear = lin;
    exponent = exp;
    nearPlane = near;
    farPlane = far;

    float aspect = (float)shadowWidth / (float)shadowHeight;
    lightProj = glm::perspective(glm::radians(90.0f), aspect, nearPlane, farPlane);

    shadowMap = new OmniShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::UseLight(unsigned int ambientIntensityLocation, unsigned int ambientColorLocation,
                          unsigned int diffuseIntensityLocation, unsigned int positionLocation,
                          unsigned int constantLocation, unsigned int linearLocation, unsigned int exponentLocation) {
    glUniform3f(ambientColorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}

std::vector<glm::mat4> PointLight::CalculateLightTransform() {
    std::vector<glm::mat4> lightMatrices;
    // +x
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    // -x
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    // +y
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    // -y
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    // +z
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
    // -z
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

    return lightMatrices;
}

float PointLight::GetFarPlane() {
    return farPlane;
}

glm::vec3 PointLight::GetPosition() {
    return position;
}

PointLight::~PointLight() {
}
