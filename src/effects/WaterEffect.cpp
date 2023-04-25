#include "../../inc/effects/WaterEffect.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"


int initializeWater(TextureVariables *waterTexture) {
	
	if (LoadGLTexture_UsingSOIL(&waterTexture->displacement, waterTexture->displacementPath) == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture texture_Displacement For Water FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull for Water = %u!!!\n", waterTexture->displacement);
	}

	//
	initializeWaterQuad();


	return 0;

}

void displayWater() {

	// Code
	displayWaterQuad();
	
}

void uninitializeWater(TextureVariables *waterTexture) {

	uninitializeWaterQuad();

	if (waterTexture->displacement)
	{
		glDeleteTextures(1, &waterTexture->displacement);
		waterTexture->displacement = NULL;
	}

}
