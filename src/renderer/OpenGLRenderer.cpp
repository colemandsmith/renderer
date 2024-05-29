#include "OpenGLRenderer.h"

#include <GL/glew.h>

OpenGLRenderer::OpenGLRenderer() {}

void OpenGLRenderer::UseTexture(const Texture *texture) {
  UseTexture(texture, 1);
}

void OpenGLRenderer::UseTexture(const Texture *texture,
                                unsigned char textureUnit) {
  if (texture == nullptr)
    return;
  unsigned int textureId = textureBindings[texture];
  glActiveTexture(GL_TEXTURE1 + textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureId);
}

void OpenGLRenderer::UseMaterial(const Material *material) {
  if (material == nullptr)
    return;
  Shader *shader = material->GetShader();
  if (shader) {
    glUniform1f(shader->GetSpecularIntensityLocation(),
                material->GetSpecularIntensity());
    glUniform1f(shader->GetShininessLocation(), material->GetShininess());
  }
}

void OpenGLRenderer::RenderMesh(const Mesh *mesh) {
  if (mesh == nullptr)
    return;
  // unbind
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  MeshBindingData d = meshBindings[mesh];

  glBindVertexArray(d.VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d.IBO);
  glDrawElements(GL_TRIANGLES, d.indexCount, GL_UNSIGNED_INT, 0);

  // unbind
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::SubmitTexture(const Texture *texture) {
  if (texture == nullptr)
    return;
  textureBindings.emplace(texture, 0);
  glGenTextures(1, &(textureBindings[texture]));
  glBindTexture(GL_TEXTURE_2D, textureBindings[texture]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (texture->GetBitDepth() == RGBA) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->GetWidth(),
                 texture->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 texture->GetTextureData());
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->GetWidth(),
                 texture->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texture->GetTextureData());
  }
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRenderer::SubmitMesh(const Mesh *mesh) {
  if (mesh == nullptr)
    return;
  MeshBindingData d = {};
  meshBindings.emplace(mesh, d);
  int numIndices, numVertices;

  const unsigned int *indices = mesh->GetIndices(numIndices);
  const float *vertices = mesh->GetVertices(numVertices);

  meshBindings[mesh].indexCount = numIndices;

  glGenVertexArrays(1, &(meshBindings[mesh].VAO));
  glBindVertexArray(meshBindings[mesh].VAO);

  glGenBuffers(1, &(meshBindings[mesh].IBO));
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshBindings[mesh].IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices,
               indices, GL_STATIC_DRAW);

  glGenBuffers(1, &(meshBindings[mesh].VBO));
  glBindBuffer(GL_ARRAY_BUFFER, meshBindings[mesh].VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11, 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11,
                        (void *)(sizeof(vertices[0]) * 3));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11,
                        (void *)(sizeof(vertices[0]) * 5));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 11,
                        (void *)(sizeof(vertices[0]) * 8));
  glEnableVertexAttribArray(3);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind the vertex array
  glBindVertexArray(0);

  // unbind the IBO
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLRenderer::ClearTexture(const Texture *texture) {
  if (texture) {
    glDeleteTextures(1, &textureBindings[texture]);
  }
}

void OpenGLRenderer::ClearMesh(const Mesh *mesh) {
  MeshBindingData bindingData = meshBindings[mesh];
  if (bindingData.IBO != 0) {
    glDeleteBuffers(1, &bindingData.IBO);
    bindingData.IBO = 0;
  }
  if (bindingData.VBO != 0) {
    glDeleteBuffers(1, &bindingData.VBO);
    bindingData.VBO = 0;
  }
  if (bindingData.VAO != 0) {
    glDeleteVertexArrays(1, &bindingData.VAO);
    bindingData.VAO = 0;
  }
  bindingData.indexCount = 0;
}

OpenGLRenderer::~OpenGLRenderer() { meshBindings.clear(); }
