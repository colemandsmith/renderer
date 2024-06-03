#pragma once

#include <unordered_map>
#include <vector>

#include "Shader.h"

class ShaderManager {
public:
  const ShaderId AddShaderFromFiles(const char *vertexLocation,
                                    const char *fragmentLocation);
  const ShaderId AddShaderFromFiles(const char *vertexLocation,
                                    const char *geometryLocation,
                                    const char *fragmentLocation);
  const Shader *GetShaderById(const ShaderId shaderId);
private:
  std::unordered_map<ShaderId,Shader*> shaderMap;
};
