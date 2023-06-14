#pragma once
#include "../helper/common.h"


struct ADSUniform
{

    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint projectionMatrixUniform;

    GLuint textureSamplerUniform_diffuse;

    //normal mapping
    GLuint viewpositionUniform;
    GLuint textureSamplerUniform_normal;

    //blending
    GLuint textureSamplerUniform1;
    GLuint textureSamplerUniform2;
    GLuint blendingUniform;
    GLuint uniform_enable_blending;

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

    GLuint lightSpaceMatrixUniform;
    GLuint shadowMapSamplerUniform;
    GLuint depthTextureSamplerUniform;
    GLuint actualSceneUniform;
    GLuint depthSceneUniform;
    GLuint depthQuadSceneUniform;
    GLuint nearUniform;
    GLuint farUniform;

    GLuint densityUniform;
    GLuint gradientUniform;
    GLuint skyFogColorUniform;
    GLuint fogEnableUniform;
    GLuint uniform_enable_godRays;
    GLuint isInstanced;
    GLuint godrays_blackpass_sphere;

    GLuint blackOrWhiteRoomUniform;
    GLuint blackOrWhiteRoomMixDeltaUniform;
    GLuint ssaoIntensityDeltaUniform;

    GLuint colorCorrectionUniform;

};

int initializeADSShader(void);
struct ADSUniform useADSShader(void);
GLuint getADSShaderProgramObject(void);
void uninitializeADSShader(void);

