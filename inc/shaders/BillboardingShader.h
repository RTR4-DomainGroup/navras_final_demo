#pragma once
#include "../common.h"


struct BillboardingUniform
{
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform;
    
    // billboarding enable uniform
    GLuint billboardingEnableUniform;
};

int initializeBillboardingShader(void);
struct BillboardingUniform useBillboardingShader(void);
void uninitializeBillboardingShader(void);
