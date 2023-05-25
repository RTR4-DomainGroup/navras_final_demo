#pragma once
#include "common.h"

struct ShadowFrameBufferDetails
{
    GLuint frameBuffer;
    GLint textureWidth;
    GLint textureHeight;
    GLuint frameBufferTexture;
    GLuint frameBufferDepthTexture;
};

bool shadowCreateFBO(struct ShadowFrameBufferDetails*);
