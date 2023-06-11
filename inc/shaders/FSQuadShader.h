#pragma once
#include "../helper/common.h"

struct FSQuadUniform
{
    GLuint textureSamplerUniform1;
    GLuint textureSamplerUniform2;
    GLuint textureSamplerUniform3;
    GLuint singleTexture;
};


int initializeFSQuadShader(void);
struct FSQuadUniform useFSQuadShader(void);
void uninitializeFSQuadShader(void);