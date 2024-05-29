#include "Material.h"

Material::Material(std::shared_ptr<Shader> shader)  {
    this->shader = shader;
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(std::shared_ptr<Shader> shader, float sIntensity, float shine) {
    this->shader = shader;
	specularIntensity = sIntensity;
	shininess = shine;
}

Material::~Material() {
    
}
