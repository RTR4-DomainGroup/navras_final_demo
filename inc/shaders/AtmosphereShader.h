#pragma once
#include "../helper/common.h"

struct AtmosphereUniform
{
    GLuint modelMatrix;
    GLuint viewMatrix;
    GLuint projectionMatrix;
    GLuint cameraPosUniform;
    GLuint lightPosUniform;
    GLuint invWavelengthUniform;
    GLuint cameraHeightUniform;
    GLuint cameraHeight2Uniform;
    GLuint innerRadiusUniform;
    GLuint innerRadius2Uniform;
    GLuint outerRadiusUniform;
    GLuint outerRadius2Uniform;
    GLuint KrESunUniform;
    GLuint KmESunUniform;
    GLuint Kr4PIUniform;
    GLuint Km4PIUniform;
    GLuint scaleUniform;
    GLuint scaleDepthUniform;
    GLuint scaleOverScaleDepthUniform;
    GLuint gUniform;
    GLuint g2Uniform;
};


int initializeAtmosphereShader(void);
struct AtmosphereUniform useAtmosphereShader(void);
void uninitializeAtmosphereShader(void);
