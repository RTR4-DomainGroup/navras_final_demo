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

    //normal mapping
    GLuint viewpositionUniform;
    GLuint textureSamplerUniform_normal;

    // Shadow
    GLuint lightSpaceMatrixUniform;
    GLuint shadowMapSamplerUniform;
    GLuint actualSceneUniform;
    GLuint depthSceneUniform;
    GLuint depthQuadSceneUniform;
    GLuint depthTextureSamplerUniform;

};

int initializeADSDynamicShader(void);
struct ADSDynamicUniform useADSDynamicShader(void);
GLuint getDynamicShaderProgramObject(void);
void uninitializeADSDynamicShader(void);

