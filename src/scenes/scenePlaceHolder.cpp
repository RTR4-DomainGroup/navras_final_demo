// This File Will Be Replaced by Scene*.cpp

#include "../../inc/scenes/scenePlaceHolder.h"

#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/ADSLights.h"

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/common.h"



/// Steps
// 1. initialize(load) texture
// 2. initialize geometry
// 3. initialize geometry
int initializeScene_PlaceHolder(void)
{

    // Code.
	// initializeADSLight();


	if (initializeTerrain() != 0) 
	{
		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}

    // initializeCube();
    // initializePyramid();
    // initializeQuad();
    // initializeTriangle();
     //initializeSphere();

	

	//
	//ZeroMemory(&sceneADSUniform, sizeof(struct ADSUniform));



	return 0;

}

void displayScene_PlaceHolder(void)
{

	// Code
	// displayADSLights();

	// Call Geometry over here 
	// displayCube();
	// displayTriangle();
    // displayQuad();
    // displayPyramid();
	// displaySphere();

	displayTerrain();
	
	

	// Un-use ShaderProgramObject
	glUseProgram(0);
}

void updateScene_PlaceHolder(void)
{

	// Code
	// updateADSLight();

}

void uninitializeScene_PlaceHolder(void)
{

	// Code
	uninitializeTerrain();

    // uninitializeSphere();
    // uninitializeTriangle();
    // uninitializeQuad();
    // uninitializePyramid();
    // uninitializeCube();


	
}

