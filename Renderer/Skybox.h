#pragma once

#include <vector>
#include <string>

#include <GL/glew.h>

#include "Mesh.h"
#include "Shader.h"
#include "CommonValues.h"

class Skybox {
public:
    Skybox();

    Skybox(std::vector<std::string> faceLocations);
    void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

    ~Skybox();

private:
    Mesh* skyMesh;
    Shader* skyShader;

    GLuint textureId;
    GLuint uniformProjection, uniformView;
};

