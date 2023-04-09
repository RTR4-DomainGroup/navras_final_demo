#pragma once
#include "../common.h"


struct BillboardingUniform
{
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform;
    // billboarding enable uniform
    GLuint billboardingUniform;
};

int initializeBillboardingShader(void);
struct BillboardingUniform useBillboardingShader(void);
void uninitializeBillboardingShader(void);
