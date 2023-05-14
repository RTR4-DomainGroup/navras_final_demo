#pragma once
#include "../helper/common.h"

struct StarfieldUniform
{
    GLuint modelMatrix;
    GLuint viewMatrix;
    GLuint projectionMatrix;
    GLuint textureSamplerUniform;
    GLuint uniform_enable_godRays;

    GLuint timeUniform;
    mat4 perspectiveProjectionMatrix;

};


int initializeStarfieldShader(void);
struct StarfieldUniform useStarfieldShader(void);
void uninitializeStarfieldShader(void);
