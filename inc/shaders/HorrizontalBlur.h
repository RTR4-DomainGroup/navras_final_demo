#pragma once
#include "../helper/common.h"

struct HorrizontalBlurUniform
{
    /* data */
    GLuint targetWidth;
    GLuint hblurTexSamplerUniform;
};


int initialize_horrizontalBlur(void);

HorrizontalBlurUniform useHorrizontalBlurShader(void);

void uninitialize_horrizontalBlurShader(void);
