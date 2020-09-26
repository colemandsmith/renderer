#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {}

bool OmniShadowMap::Init(unsigned int width, unsigned height) {
    shadowWidth = width;
    shadowHeight = height;

    glGenFramebuffers(1, &FBO);
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

    for (size_t i = 0; i < 6; i++) {
        // instead of defining each cube face manually, can use enum math
        // since they're contiguous
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // x
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // y
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // z
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer Error: %i\n", status);
        return false;
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}


void OmniShadowMap::Write() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void OmniShadowMap::Read(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}


OmniShadowMap::~OmniShadowMap() {
    if (FBO) {
        glDeleteFramebuffers(1, &FBO);
    }
    if (shadowMap) {
        glDeleteTextures(1, &shadowMap);
    }
}
