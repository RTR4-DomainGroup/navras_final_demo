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
	uninitializeSphere();

	uninitializeNoiseTexture();
}
