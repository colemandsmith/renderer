#include "ShaderManager.h"

const ShaderId ShaderManager::AddShaderFromFiles(const char *vertexLocation,
                                                 const char *fragmentLocation) {
  Shader* newShader = new Shader();
  newShader->CreateFromFiles(vertexLocation, fragmentLocation);

  return newShader->GetShaderId();
}
const ShaderId ShaderManager::AddShaderFromFiles(const char *vertexLocation,
                                                 const char *geometryLocation,
                                                 const char *fragmentLocation) {
  Shader* newShader = new Shader();
  newShader->CreateFromFiles(vertexLocation, geometryLocation, fragmentLocation);

  return newShader->GetShaderId();
}

const Shader *ShaderManager::GetShaderById(const ShaderId shaderId) {
  if (shaderMap.find(shaderId) == shaderMap.end()) {
    return nullptr;
  }
  return shaderMap[shaderId];
}
