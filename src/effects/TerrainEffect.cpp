#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/geometry.h"

extern mat4 perspectiveProjectionMatrix;

int initializeTerrain(TextureVariables *terrainTexture) {
	
	if (LoadGLTexture_UsingSOIL(&terrainTexture->displacement, terrainTexture->displacementPath) == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture texture_Displacement For Terrain FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull for terrain = %u!!!\n", terrainTexture->displacement);
	}

	if (LoadGLTexture_UsingSOIL(&terrainTexture->albedo, terrainTexture->albedoPath) == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture texture_Diffuse For Terrain FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull for terrain = %u!!!\n", terrainTexture->albedo);
	}

	return 0;
}

void displayTerrain() {

	// Code
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArraysInstanced(GL_PATCHES, 0, 4, 64 * 64);
}

void uninitializeTerrain(TextureVariables *terrainTexture) {

	if (terrainTexture->displacement)
	{
		glDeleteTextures(1, &terrainTexture->displacement);
		terrainTexture->displacement = NULL;
	}

	if (terrainTexture->albedo)
	{
		glDeleteTextures(1, &terrainTexture->albedo);
		terrainTexture->albedo = NULL;
	}
}
