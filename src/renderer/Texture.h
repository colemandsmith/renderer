#pragma once

#include <GL/glew.h>

#include "CommonValues.h"


enum TextureColorSpace {
    RGB,
    RGBA
};


class Texture {
public:
	Texture();
	Texture(const char* fileLoc);

	const bool LoadTexture();
    const bool LoadTextureA();
	void UseTexture();
    void UseTexture(GLenum textureUnit);
	void ClearTexture();
    inline TextureColorSpace GetColorSpace() const { return colorSpace; }
    inline int GetHeight() const { return height; }
    inline int GetWidth() const { return width; }
    inline int GetBitDepth() const { return bitDepth; }
    inline unsigned char* GetTextureData() const { return textureData; }
	~Texture();

private:
	GLuint textureId;
	int width, height, bitDepth;
    unsigned char* textureData;
	const char* fileLocation;
    TextureColorSpace colorSpace;
};
