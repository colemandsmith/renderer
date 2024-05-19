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
    EQUIRECTANGULAR,
    CUBEMAP,
    SEPARATE_IMAGES
  };

  Skybox(std::string skyboxLocation, Skybox::TextureType inputTextureType = Skybox::TextureType::EQUIRECTANGULAR);
  Skybox(std::vector<std::string> faceLocations);
  void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

  ~Skybox();

private:
  void CreateMesh();
  void ConvertEquirectToCubemap(GLuint equirectTextureId);
  Mesh *skyMesh;
  Shader *skyShader;

  GLuint textureId;
  GLuint uniformProjection, uniformView;
  Skybox::TextureType skyboxTextureType;
};
