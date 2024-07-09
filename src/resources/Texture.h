#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

#ifndef RGB
#define RGB 3
#define RGBA 4
#endif

struct TextureId {
  unsigned int deviceId;

  static TextureId Null;

  bool operator==(const TextureId& other) const { return deviceId == other.deviceId; }
  bool operator!=(const TextureId& other) const { return deviceId != other.deviceId; }
};

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
