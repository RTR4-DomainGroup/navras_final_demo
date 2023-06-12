#pragma once
#include "common.h"

struct SSAOFrameBufferStruct
{
    GLuint      render_fbo;
    GLuint      fbo_textures[3];
    GLint       textureWidth;
    GLint       textureHeight;

};

bool ssaoCreateFBO(struct SSAOFrameBufferStruct*);
