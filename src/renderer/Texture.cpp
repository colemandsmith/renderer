#include "Texture.h"

Texture::Texture() {
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = NULL;
}

Texture::Texture(const char* fileLoc) {
	textureId = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
  LoadTexture();
}

const bool Texture::LoadTexture() {
    unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);
    if (!textureData) {
        printf("Failed to find %s\n", fileLocation);
        return false;
    }

    return true;
}

void Texture::ClearTexture() {
  if (textureData != nullptr) {
    stbi_image_free(textureData);
  }
	// glDeleteTextures(1, &textureId);
	// textureId = 0;
	// width = 0;
	// height = 0;
	// bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture() {
	ClearTexture();
}
