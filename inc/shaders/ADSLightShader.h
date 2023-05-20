#pragma once
#include "../helper/common.h"


struct ADSUniform
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

    GLuint lightingEnableUniform;

    GLuint lightSpaceMatrixUniform;
    GLuint shadowMapSamplerUniform;
    GLuint depthTextureSamplerUniform;
    GLuint actualSceneUniform;
    GLuint depthSceneUniform;
    GLuint depthQuadSceneUniform;
    GLuint nearUniform;
    GLuint farUniform;

    GLuint densityUniform;
    GLuint gradientUniform;
    GLuint skyFogColorUniform;
    GLuint fogEnableUniform;
    GLuint uniform_enable_godRays;

    GLuint godrays_blackpass_sphere;

};

int initializeADSShader(void);
struct ADSUniform useADSShader(void);
void uninitializeADSShader(void);

