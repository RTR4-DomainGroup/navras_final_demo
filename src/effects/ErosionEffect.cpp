#include "../../inc/effects/ErosionEffect.h"
#include "../../inc/Noise.h"
#include "../../inc/helper/geometry.h"

extern mat4 perspectiveProjectionMatrix;

GLuint initializeErosion(void)
{
	// function declarations
	GLuint CreateNoise3D(void);

	// code
	initializeQuad();

	GLuint texture_id = 0;
	texture_id = CreateNoise3D();

	return(texture_id);
}

void updateErosion(bool offsetIncrement, float offset[3], float iterator)
{
	// code
	offset[0] = offset[0] - iterator;
	offset[1] = offset[1] - iterator;
	offset[2] = offset[2] - iterator;
	if (offset[2] < 0.17f)
	{
		offset[0] = 0.17f;
		offset[1] = 0.17f;
		offset[2] = 0.17f;
		/*offsetIncrement = true;*/
	}
}

void uninitializeErosion(void)
{
	// code
	uninitializeQuad();
}