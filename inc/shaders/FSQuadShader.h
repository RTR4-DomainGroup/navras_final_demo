#pragma once
#include "../helper/common.h"

struct FSQuadUniform
{
    GLuint textureSamplerUniform1;
    GLuint textureSamplerUniform2;
};


int initializeFSQuadShader(void);
struct FSQuadUniform useFSQuadShader(void);
void uninitializeFSQuadShader(void);