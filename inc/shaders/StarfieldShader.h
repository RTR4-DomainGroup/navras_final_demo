#pragma once
#include "../helper/common.h"

struct StarfieldUniform
{
    GLuint modelMatrix;
    GLuint viewMatrix;
    GLuint projectionMatrix;
    GLuint textureSamplerUniform;

    GLuint timeUniform;
    mat4 perspectiveProjectionMatrix;

};


int initializeStarfieldShader(void);
struct StarfieldUniform useStarfieldShader(void);
void uninitializeStarfieldShader(void);
