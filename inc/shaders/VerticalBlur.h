#pragma once
#include "../helper/common.h"

struct VerticalBlurUniform
{
    /* data */
    GLuint targetHeight;
    GLuint vblurTexSamplerUniform;
};

int initialize_verticalBlur(void);
VerticalBlurUniform useHorrizontalBlurShader(void);
