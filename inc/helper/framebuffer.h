#pragma once
#include "common.h"

struct FrameBufferDetails
{
    GLuint frameBuffer;
    GLuint renderBuffer;
    GLint textureWidth;
    GLint textureHeight;
    GLuint frameBufferTexture;
};

bool createFBO(struct FrameBufferDetails*);