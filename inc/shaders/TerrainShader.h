#pragma once
#include "../helper/common.h"

struct TerrainUniform
{
    GLint uniform_mvp_matrix;
    GLint uniform_mv_matrix;
    GLint uniform_proj_matrix;
    GLint uniform_dmap_depth;
    GLint uniform_enable_fog;

    GLuint textureSamplerUniform1;
    GLuint textureSamplerUniform2;

    GLuint densityUniform;
    GLuint gradientUniform;
    GLuint skyFogColorUniform;
    GLuint fogEnableUniform;

};

int initializeTerrainShader(void);
struct TerrainUniform useTerrainShader(void);
void uninitializeTerrainShader(void);
