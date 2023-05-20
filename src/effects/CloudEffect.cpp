#include "../../inc/effects/CloudEffect.h"
#include "../../inc/Noise.h"
#include "../../inc/helper/geometry.h"

extern mat4 perspectiveProjectionMatrix;

GLuint initializeCloud(void)
{
	// function declarations
	GLuint CreateNoise3D(void);

	// code
	initializeQuad();

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

void uninitializeCloud(void)
{
	// code
	uninitializeQuad();
	uninitializeNoiseTexture();
	/*if (noise_texture)
	{
		glDeleteTextures(1, &noise_texture);
		noise_texture = 0;
	}*/
}
