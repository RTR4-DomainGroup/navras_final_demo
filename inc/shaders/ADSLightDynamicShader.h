#pragma once
#include "../helper/common.h"

#define MAX_BONES 100

struct ADSDynamicUniform
{

    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform;

    // lIght
    GLuint laUniform;
    GLuint ldUniform;
    GLuint lsUniform;
    GLuint lightPositionUniform;

    GLuint kaUniform;
    GLuint kdUniform;
    GLuint ksUniform;
    GLuint materialShininessUniform;

    GLuint densityUniform;
    GLuint gradientUniform;
    GLuint skyFogColorUniform;
    GLuint fogEnableUniform;
    GLuint uniform_enable_godRays;

    GLuint godrays_blackpass_sphere;


    GLuint finalBonesMatricesUniform[MAX_BONES];

};

int initializeADSDynamicShader(void);
struct ADSDynamicUniform useADSDynamicShader(void);
void uninitializeADSDynamicShader(void);

