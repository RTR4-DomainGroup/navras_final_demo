#pragma once
#include "../helper/common.h"

struct SkyboxUniform
{
    GLuint modelMatrix;
    GLuint viewMatrix;
    GLuint projectionMatrix;

    GLuint textureSamplerUniform;
};


int initializeSkyboxShader(void);
struct SkyboxUniform useSkyboxShader(void);
void uninitializeSkyboxShader(void);