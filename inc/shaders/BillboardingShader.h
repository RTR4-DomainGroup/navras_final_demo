#pragma once
#include "../helper/common.h"


struct BillboardingUniform
{
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform;
    
    // billboarding enable uniform
    GLuint billboardingEnableUniform;
    GLuint uniform_enable_godRays;
};

int initializeBillboardingShader(void);
struct BillboardingUniform useBillboardingShader(void);
void uninitializeBillboardingShader(void);
