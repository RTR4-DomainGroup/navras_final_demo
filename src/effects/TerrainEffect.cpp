#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"

extern FILE* gpFile;

struct TerrainUniform terrainUniform;

GLuint texture_Displacement;
GLuint texture_Diffuse;

float displacementmap_depth;
bool enable_fog;

extern mat4 perspectiveProjectionMatrix;

int initializeTerrain() {
	
	if (LoadGLTexture_UsingSOIL(&texture_Displacement, "res/textures/DisplacementMapTerrain.jpg") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture texture_Displacement For Terrain FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull for terrain = %u!!!\n", texture_Displacement);
	}

	if (LoadGLTexture_UsingSOIL(&texture_Diffuse, "res/textures/DiffuseMapTerrain.jpg") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture texture_Diffuse For Terrain FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull for terrain = %u!!!\n", texture_Diffuse);
	}

	displacementmap_depth = 15.0f;

	return 0;

}

void displayTerrain() {

	// Code

	terrainUniform = useTerrainShader();

	vmath::mat4 mv_matrix = viewMatrix * (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));
	
	vmath::mat4 proj_matrix = perspectiveProjectionMatrix;

	glUniformMatrix4fv(terrainUniform.uniform_mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);

	glUniform1f(terrainUniform.uniform_dmap_depth, displacementmap_depth);
	//glUniform1i(terrainUniform.uniform_enable_fog, enable_fog ? 1 : 0);
	glUniform1i(terrainUniform.uniform_enable_fog, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_Displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_Diffuse);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

}

void uninitializeTerrain() {

	if (texture_Diffuse)
	{
		glDeleteTextures(1, &texture_Diffuse);
		texture_Diffuse = NULL;
	}

	if (texture_Displacement)
	{
		glDeleteTextures(1, &texture_Displacement);
		texture_Displacement = NULL;
	}

}
