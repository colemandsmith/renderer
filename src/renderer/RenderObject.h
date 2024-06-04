#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "Shader.h"
#include "Model.h"

class RenderObject {
public:
    RenderObject();
    RenderObject(Model* model, Material* material, std::string name);
    RenderObject(Model* model, Material* material, std::string name,
                 glm::vec3 position, glm::vec3 scale,
                 GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);

    void Render(Shader* shader);
    glm::mat4 GetModelMatrix();

    ~RenderObject();
private:
    std::string name;
    Model* model;
    Material* material;

    glm::vec3 position;
    glm::vec3 scale;

    // Using a simple rotation model
    // Combination of rotations around z, y, and z axes
    // All in degrees
    GLfloat xRotation, yRotation, zRotation;
};
