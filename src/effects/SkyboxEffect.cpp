// Header Files

#include "../../inc/scenes/scenes.h"
#include "../../inc/helper/vmath.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/effects/SkyboxEffect.h"

using namespace vmath;

GLuint texture_skybox;

struct SkyboxUniform sceneSkyBoxUniform;

extern mat4 viewMatrix;
extern mat4 perspectiveProjectionMatrix;

GLfloat angleCubemap;

int initializeSkybox(void)
{
	// Code.
	// Texture
	if (LoadGLTexture_Cubemap(&texture_skybox, "res\\textures\\Skybox\\") == FALSE) 
	{
		LOG("LoadGLTexture_Cubemap() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture_Cubemap() Successfull = %u!!!\n", texture_skybox);
	}

	initializeCubemap();

	return 0;
}


void displaySkybox(void)
{
	// Code
	glEnable(GL_TEXTURE_CUBE_MAP);

	sceneSkyBoxUniform = useSkyboxShader();

	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -10.0f);					// glTranslatef() is replaced by this line.
	scaleMatrix = vmath::scale(30.0f, 30.0f, 30.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT
	
	glUniformMatrix4fv(sceneSkyBoxUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneSkyBoxUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneSkyBoxUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_skybox);

	displayCubemap();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDisable(GL_TEXTURE_CUBE_MAP);

	// Un-use the Shader Program Object
	glUseProgram(0);
}

void updateSkybox(void)
{
	// Code
	angleCubemap = angleCubemap + 0.5f;
	if (angleCubemap >= 360.0f)
		angleCubemap = angleCubemap - 360.0f;
}

void uninitialiseSkybox(void)
{
	if (texture_skybox)
	{
		glDeleteTextures(1, &texture_skybox);
		texture_skybox = 0;
	}
}
