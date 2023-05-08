#pragma once
#include "../helper/common.h"

struct WaterUniform
{
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint reflectionTextureSamplerUniform;
    GLuint refractionTextureSamplerUniform;

    GLuint dudvTextureSamplerUniform;
    GLuint moveFactorUniform;
    GLuint planeUniform;
    GLuint cameraPositionUniform;
    GLuint uniform_enable_godRays;
};

int initializeWaterShader(void);
struct WaterUniform useWaterShader(void);
void uninitializeWaterShader(void);
