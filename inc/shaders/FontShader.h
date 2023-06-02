#pragma once
#include "../helper/common.h"

struct FontUniform
{
    GLint uniform_mvp_matrix;
    GLint textureSamplerUniform;
    GLint textColorUniform;
};

int initializeFontShader(void);
struct FontUniform useFontShader(void);
void uninitializeFontShader(void);
