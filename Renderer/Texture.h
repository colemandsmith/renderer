#pragma once

#include <GL/glew.h>

#include "CommonValues.h"

class Texture {
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
    bool LoadTextureA();
	void UseTexture();
    void UseTexture(GLenum textureUnit);
	void ClearTexture();
	~Texture();

private:
	GLuint textureId;
	int width, height, bitDepth;
	const char* fileLocation;
};