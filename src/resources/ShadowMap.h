#pragma once

#include <GL/glew.h>
#include <stdio.h>

class ShadowMap {
public:
    ShadowMap();

    virtual bool Init(unsigned int width, unsigned height);

    virtual void Write();
    virtual void Read(GLenum textureUnit);

    unsigned int GetShadowWidth() { return shadowWidth;  }
    unsigned int GetShadowHeight() { return shadowHeight; }

    ~ShadowMap();

protected:
    unsigned int FBO, shadowMap;
    unsigned int shadowWidth, shadowHeight;
};

