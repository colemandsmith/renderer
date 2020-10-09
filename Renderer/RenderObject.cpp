#include "RenderObject.h"

RenderObject::RenderObject() {
    name = "";
    model = nullptr;

    scale = glm::vec3(1.0, 1.0, 1.0);
    position = glm::vec3(0.0, 0.0, 0.0);
    xRotation = 0.0f;
    yRotation = 0.0f;
    zRotation = 0.0f;
}

RenderObject::RenderObject(Model* model, std::string name) {
    this->name = name;
    this->model = model;

    scale = glm::vec3(1.0, 1.0, 1.0);
    position = glm::vec3(0.0, 0.0, 0.0);
    xRotation = 0.0f;
    yRotation = 0.0f;
    zRotation = 0.0f;
}

RenderObject::RenderObject(Model* model, std::string name, glm::vec3 position, glm::vec3 scale,
                           GLfloat xRotation, GLfloat yRotation, GLfloat zRotation) {
    this->name = name;
    this->model = model;

    this->scale = scale;
    this->position = position;

    this->xRotation = xRotation;
    this->yRotation = yRotation;
    this->zRotation = zRotation;
}

void RenderObject::Render(Shader* shader) {
    GLuint uniformModel = shader->GetModelLocation();
    GLuint uniformProjection = shader->GetProjectionLocation();
    GLuint uniformView = shader->GetViewLocation();

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Transforms are effectively "processed" in reverse order:
    // scale -> z rotation -> y rotation -> x rotation -> translation
    modelMatrix = glm::translate(modelMatrix, position);
    if (xRotation != 0.0f) {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(xRotation), glm::vec3(1.0f, 0.0f, 0.0f));
    }
    if (yRotation != 0.0f) {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(yRotation), glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (zRotation != 0.0f) {
        modelMatrix = glm::rotate(modelMatrix, glm::radians(zRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    modelMatrix = glm::scale(modelMatrix, scale);

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    model->RenderModel();
}

RenderObject::~RenderObject() {
    //delete model;
}
