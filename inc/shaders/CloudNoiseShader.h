#pragma once
#include "../helper/common.h"

struct CloudNoiseUniform
{
	GLuint modelMatrixUniform;
	GLuint viewMatrixUniform;
	GLuint projectionMatrixUniform;

	GLuint laUniform;
	GLuint ldUniform;
	GLuint lsUniform;
	GLuint lightPositionUniform;

	GLuint kaUniform;
	GLuint kdUniform;
	GLuint ksUniform;
	GLuint materialShininessUniform;

	// extra to be use later for configuration
	GLuint scaleUniform;
	GLuint noiseScaleUniform;

	GLuint noiseSamplerUniform;
	GLuint skyColorUniform;
	GLuint cloudColorUniform;
};

int intializeCloudNoiseShader(void);
struct CloudNoiseUniform useCloudNoiseShader(void);
void uninitializeCloudNoiseShader(void);
