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
    inline const TextureColorSpace GetColorSpace() { return colorSpace; }
	~Texture();

private:
	GLuint textureId;
	int width, height, bitDepth;
    unsigned char* textureData;
	const char* fileLocation;
    TextureColorSpace colorSpace;
};
