#pragma once
#include "../helper/common.h"

struct GaussianBlurUniform
{
    /* data */
    GLuint time;
    GLuint hblurTexSamplerUniform;
    GLuint resolution;
};


int initialize_gaussianBlur(void);

GaussianBlurUniform useGaussianBlurShader(void);

void uninitialize_gaussianBlurShader(void);
