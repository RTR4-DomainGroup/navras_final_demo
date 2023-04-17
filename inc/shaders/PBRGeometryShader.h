#pragma once
#include "../helper/common.h"


struct PBRGeometryUniform
{

    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform_albedo;
    GLuint textureSamplerUniform_normal;
    GLuint textureSamplerUniform_metallic;
    GLuint textureSamplerUniform_roughness;
    GLuint textureSamplerUniform_ao;

    // lIght
    GLuint laUniform;
    GLuint ldUniform;
    GLuint lsUniform;
    GLuint lightPositionUniform;

    GLuint kaUniform;
    GLuint kdUniform;
    GLuint ksUniform;
    GLuint materialShininessUniform;

    GLuint lightingEnableUniform;

    GLuint lightColorUniform;
    GLuint cameraPositionUniform;

};

int initializePBRGeometryShader(void);
struct PBRGeometryUniform usePBRGeometryShader(void);
void uninitializePBRGeometryShader(void);

