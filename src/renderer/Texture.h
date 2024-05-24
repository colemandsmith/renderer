#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

#define RGB 3
#define RGBA 4

class Texture {
public:
  Texture();
  Texture(const char *fileLoc);

  const bool LoadTexture();
  void UseTexture();
  void UseTexture(GLenum textureUnit);
  void ClearTexture();
  inline int GetHeight() const { return height; }
  inline int GetWidth() const { return width; }
  inline int GetBitDepth() const { return bitDepth; }
  inline unsigned char *GetTextureData() const { return textureData; }
  ~Texture();

private:
  GLuint textureId;
  int width, height, bitDepth;
  unsigned char *textureData = nullptr;
  const char *fileLocation;
};
