#pragma once
#include "../helper/common.h"

struct FSVQuadUniform
{
    GLuint textureSamplerUniform1;
    GLuint textureSamplerUniform2;
};


int initializeFSVQuadShader(void);
struct FSVQuadUniform useFSVQuadShader(void);
void uninitializeFSVQuadShader(void);