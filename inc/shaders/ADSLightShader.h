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

    GLuint densityUniform;
    GLuint gradientUniform;
    GLuint skyFogColorUniform;
    GLuint fogEnableUniform;

};

int initializeADSShader(void);
struct ADSUniform useADSShader(void);
void uninitializeADSShader(void);

