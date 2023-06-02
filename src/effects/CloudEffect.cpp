#include "../../inc/effects/CloudEffect.h"
#include "../../inc/Noise.h"
#include "../../inc/helper/geometry.h"

extern mat4 perspectiveProjectionMatrix;

GLuint initializeCloud(void)
{
	// function declarations
	GLuint CreateNoise3D(void);

	// code
	initializeSphere(1.0f, 60.0f, 60.0f);

	// BLENDING
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//loadNoiseTexture();
	//noise_texture = CreateNoise3D();

	GLuint texture_id = 0;
	texture_id = CreateNoise3D();

	return(texture_id);
}

void updateCloud(bool noiseScaleIncrement, float noiseScale, float iterator)
{
	// code
	if (noiseScaleIncrement)
	{
		noiseScale = noiseScale + iterator;
		if (noiseScale > 0.5f)
		{
			noiseScaleIncrement = false;
		}
	}
	else
	{
		noiseScale = noiseScale - iterator;
		if (noiseScale < 0.01f)
		{
			noiseScaleIncrement = true;
		}
	}
}

void updateErode(bool offsetIncrement, GLfloat offset[3], float iterator)
{
	// code
	if (offsetIncrement == true)
	{
		offset[0] = offset[0] + iterator;
		offset[1] = offset[1] + iterator;
		offset[2] = offset[2] + iterator;
		if (offset[2] > 0.48f)
		{
			offsetIncrement = false;
		}
	}
	else
	{
		offset[0] = offset[0] - iterator;
		offset[1] = offset[1] - iterator;
		offset[2] = offset[2] - iterator;
		if (offset[2] < 0.17f)
		{
			offsetIncrement = true;
		}
	}
}

void uninitializeCloud(void)
{
	// code
	uninitializeSphere();

	uninitializeNoiseTexture();
	/*if (noise_texture)
	{
		glDeleteTextures(1, &noise_texture);
		noise_texture = 0;
	}*/
}
