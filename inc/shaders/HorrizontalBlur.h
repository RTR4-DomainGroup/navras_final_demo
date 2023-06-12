#pragma once
#include "../helper/common.h"

struct HorrizontalBlurUniform
{
    /* data */
    GLuint targetWidth;
    GLuint hblurTexSamplerUniform;
    GLuint blurFactor;
};


int initialize_horrizontalBlur(void);

HorrizontalBlurUniform useHorrizontalBlurShader(void);

void uninitialize_horrizontalBlurShader(void);
