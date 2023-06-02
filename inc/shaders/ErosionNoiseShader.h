#pragma once
#include "../helper/common.h"

struct ErosionNoiseUniform
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
	GLuint textureSamplerUniform;

	// uniform specifically for erosion
	GLuint offsetUniform;

	GLuint uniform_enable_godRays;
};

int intializeErosionNoiseShader(void);
struct ErosionNoiseUniform useErosionNoiseShader(void);
void uninitializeErosionNoiseShader(void);