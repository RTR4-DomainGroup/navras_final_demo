#pragma once
#include "../helper/common.h"

struct TerrainUniform
{
    GLint uniform_mvp_matrix;
    GLint uniform_mv_matrix;
    GLint uniform_proj_matrix;
    GLint uniform_dmap_depth;
    GLint uniform_enable_fog;
    GLint uniform_enable_godRays;

    GLuint textureSamplerUniform1;
    GLuint textureSamplerUniform2;

};

int initializeTerrainShader(void);
struct TerrainUniform useTerrainShader(void);
void uninitializeTerrainShader(void);
