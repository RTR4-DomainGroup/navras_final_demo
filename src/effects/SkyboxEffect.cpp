// Header Files

#include "../../inc/scenes/scenes.h"
#include "../../inc/helper/vmath.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/effects/SkyboxEffect.h"

using namespace vmath;

int initializeSkybox(GLuint* texture, const char * path)
{
	// Code.
	// Texture
	if (LoadGLTexture_Cubemap(texture, path) == false) 
	{
		LOG("LoadGLTexture_Cubemap() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture_Cubemap() Successfull = %u!!!\n", texture);
	}

	initializeCubemap();

	return 0;
}


void displaySkybox(GLuint texture)
{
	// Code
	glEnable(GL_TEXTURE_CUBE_MAP);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	displayCubemap();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDisable(GL_TEXTURE_CUBE_MAP);
}

void uninitialiseSkybox(GLuint texture)
{
	if (texture)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}
}
