#pragma once
#include "../helper/common.h"

struct GodraysUniform
{
    GLuint modelMatrix;
    GLuint viewMatrix;
    GLuint projectionMatrix;
    GLuint density;
    GLuint weight;
    GLuint decay;
    GLuint exposure;
    GLuint lightPositionOnScreen;
    GLuint godraysampler;
    GLuint dispersalUniform;
    GLuint haloWidthUniform;
    GLuint intensityUniform;
    GLuint distortionUniform;
    GLuint godrays_lfEnabled;
};


int initializeGodraysShader(void);
struct GodraysUniform useGodRaysShader(void);
void uninitializeGodraysShader(void);