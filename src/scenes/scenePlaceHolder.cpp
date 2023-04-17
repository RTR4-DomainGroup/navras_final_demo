// This File Will Be Replaced by Scene*.cpp

#include "../../inc/scenes/scenePlaceHolder.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/TerrainEffect.h"

GLuint texture_Marble;

struct ADSUniform sceneADSUniform;

struct TerrainUniform terrainUniform1;

extern mat4 viewMatrix;

GLfloat LightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 100.0f, 1.0f };

GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialShininess = 50.0f;

GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

extern FILE* gpFile;

int initializeScene_PlaceHolder(void)
{

    // Code.
	initializeScene_Skybox();

	//ZeroMemory(&sceneADSUniform, sizeof(struct ADSUniform));

	return 0;

}

void displayScene_PlaceHolder(void)
{

	// Code
	displaySkybox();

	// Un-use ShaderProgramObject
	glUseProgram(0);

	displayTerrain();
	
	


}


void updateScene_PlaceHolder(void)
{
	updateSkybox();

	// Code
	angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
	{
		angleCube -= 360.0f;
	}

}

void uninitializeScene_PlaceHolder(void)
{

	// Code
	uninitializeTerrain();
    uninitializeSphere();
    // uninitializeTriangle();
    // uninitializeQuad();
    // uninitializePyramid();
    // uninitializeCube();

	if(texture_Marble)
	{
		glDeleteTextures(1, &texture_Marble);
		texture_Marble = NULL;
	}

	
}

