#pragma once
#include "../common.h"

struct CloudNoiseUniform
{
	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;

	GLuint laUniform;
	GLuint ldUniform;
	GLuint lsUniform;
	GLuint lighPositionUniform;

	GLuint kaUniform;
	GLuint kdUniform;
	GLuint ksUniform;
	GLuint matrailShininessUniform;

	// extra to be use later for configuration
	GLuint scaleUniform;
	GLuint noiseScaleUniform;

	GLuint noiseSamplerUniform;
	GLuint skyColorUniform;
	GLuint cloudColorUniform;

	// more noise related Uniforms as required
};

int intializeCloudNoiseShader(void);
struct CloudNoiseUniform useCloudNoiseShader(void);
void uninitializeCloudNoiseShader(void);
