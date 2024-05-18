#pragma once

#include <string>
#include <vector>

#include <GL/glew.h>

#include "CommonValues.h"
#include "Mesh.h"
#include "Shader.h"

class Skybox {
public:
  Skybox();

  enum TextureType {
    CUBEMAP
  };

  Skybox(std::string skyboxLocation, Skybox::TextureType skyboxTextureType = Skybox::TextureType::CUBEMAP);
  Skybox(std::vector<std::string> faceLocations);
  void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

  ~Skybox();

private:
  Mesh *skyMesh;
  Shader *skyShader;

  GLuint textureId;
  GLuint uniformProjection, uniformView;
};
