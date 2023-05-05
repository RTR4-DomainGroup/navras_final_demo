#pragma once
#include "common.h"

struct WaterFrameBufferDetails
{
    GLuint frameBuffer;
    GLint textureWidth;
    GLint textureHeight;
    GLuint frameBufferTexture;
    GLuint frameBufferDepthTexture;
};

bool waterCreateFBO(struct WaterFrameBufferDetails*);
