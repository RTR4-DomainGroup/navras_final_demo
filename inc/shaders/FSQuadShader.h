#pragma once
#include "../helper/common.h"

struct FSQuadUniform
{
    GLuint textureSamplerUniform1;
    GLuint textureSamplerUniform2;
    GLuint textureSamplerUniform3;
    GLuint textureSamplerUniform4;
    GLuint singleTexture;
    GLuint alphablend;
    GLuint intensity;
    GLuint maskOrFont;
};


int initializeFSQuadShader(void);
struct FSQuadUniform useFSQuadShader(void);
void uninitializeFSQuadShader(void);