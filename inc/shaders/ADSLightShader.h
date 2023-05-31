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

    // light
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

    GLuint godrays_blackpass_sphere;

	// Uniforms for Spotlight
	GLuint spotlightLAZeroUniform;
	GLuint spotlightLDZeroUniform;
	GLuint spotlightLSZeroUniform;
	GLuint spotlightPositionZeroUniform;
	GLuint spotDirectionZeroUniform;
	GLuint constantAttenuationZeroUniform;
	GLuint linearAttenuationZeroUniform;
	GLuint quadraticAttenuationZeroUniform;
	GLuint spotCosCutoffZeroUniform;
	GLuint spotCosOuterCutoffZeroUniform;

	GLuint spotlightLAOneUniform;
	GLuint spotlightLDOneUniform;
	GLuint spotlightLSOneUniform;
	GLuint spotlightPositionOneUniform;
	GLuint spotDirectionOneUniform;
	GLuint constantAttenuationOneUniform;
	GLuint linearAttenuationOneUniform;
	GLuint quadraticAttenuationOneUniform;
	GLuint spotCosCutoffOneUniform;
	GLuint spotCosOuterCutoffOneUniform;

	GLuint spotlightLATwoUniform;
	GLuint spotlightLDTwoUniform;
	GLuint spotlightLSTwoUniform;
	GLuint spotlightPositionTwoUniform;
	GLuint spotDirectionTwoUniform;
	GLuint constantAttenuationTwoUniform;
	GLuint linearAttenuationTwoUniform;
	GLuint quadraticAttenuationTwoUniform;
	GLuint spotCosCutoffTwoUniform;
	GLuint spotCosOuterCutoffTwoUniform;

	GLuint spotlightLAThreeUniform;
	GLuint spotlightLDThreeUniform;
	GLuint spotlightLSThreeUniform;
	GLuint spotlightPositionThreeUniform;
	GLuint spotDirectionThreeUniform;
	GLuint constantAttenuationThreeUniform;
	GLuint linearAttenuationThreeUniform;
	GLuint quadraticAttenuationThreeUniform;
	GLuint spotCosCutoffThreeUniform;
	GLuint spotCosOuterCutoffThreeUniform;

	GLuint spotlightLAFourUniform;
	GLuint spotlightLDFourUniform;
	GLuint spotlightLSFourUniform;
	GLuint spotlightPositionFourUniform;
	GLuint spotDirectionFourUniform;
	GLuint constantAttenuationFourUniform;
	GLuint linearAttenuationFourUniform;
	GLuint quadraticAttenuationFourUniform;
	GLuint spotCosCutoffFourUniform;
	GLuint spotCosOuterCutoffFourUniform;

	GLuint alphaSpotlightZeroUniform;
	GLuint alphaSpotlightOneUniform;
	GLuint alphaSpotlightTwoUniform;
	GLuint alphaSpotlightThreeUniform;
	GLuint alphaSpotlightFourUniform;

	GLuint spotDirectionUniform;
	GLuint constantAttenuationUniform;
	GLuint linearAttenuationUniform;
	GLuint quadraticAttenuationUniform;
	GLuint spotCosCutoffUniform;
	GLuint spotCosOuterCutoffUniform;

};

int initializeADSShader(void);
struct ADSUniform useADSShader(void);
GLuint getADSShaderProgramObject(void);
void uninitializeADSShader(void);

