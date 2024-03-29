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

    GLuint densityUniform;
    GLuint gradientUniform;
    GLuint skyFogColorUniform;
    GLuint fogEnableUniform;

    //Normal mapping
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform_diffuse;
    GLuint textureSamplerUniform_normal;

    GLuint laUniform;	
    GLuint ldUniform;	
    GLuint lsUniform;	
    GLuint lightPositionUniform;

    GLuint kaUniform;	
    GLuint kdUniform;
    GLuint ksUniform;
    GLuint materialShininessUniform;

    // Shadow
    GLuint shadowMapSamplerUniform;
    GLuint actualSceneUniform;
    GLuint depthSceneUniform;
    GLuint lightSpaceMatrixUniform;

};

int initializeTerrainShader(void);
struct TerrainUniform useTerrainShader(void);
void uninitializeTerrainShader(void);
