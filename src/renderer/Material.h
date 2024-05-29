#pragma once

#include <memory>
#include <GL/glew.h>
#include "Shader.h"

class Material {
public:
	Material(std::shared_ptr<Shader> shader);
	Material(std::shared_ptr<Shader> shader, float sIntensity, float shine);
    inline Shader* GetShader() const { return shader; }
    inline float GetSpecularIntensity() const { return specularIntensity; }
    inline float GetShininess() const { return shininess; }
	~Material();

private:
	float specularIntensity = 0.0f;
	float shininess = 0.0f;
    Shader* shader = nullptr;
};
