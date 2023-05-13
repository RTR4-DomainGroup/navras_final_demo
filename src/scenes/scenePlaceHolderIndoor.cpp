#pragma once

// This File Will Be Replaced by Scene*.cpp

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/waterframebuffer.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/texture_loader.h"

#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/BillboardingShader.h"

#include "../../inc/effects/videoEffect.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/CloudEffect.h"
#include "../../inc/effects/WaterEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
// #include "../../inc/effects/Billboarding.h"
#include "../../inc/effects/GaussianBlurEffect.h"

#include "../../inc/scenes/scenePlaceHolderIndoor.h"


#define FBO_WIDTH 1920
#define FBO_HEIGHT 1080
//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####

#define ENABLE_STATIC_MODELS	

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

struct ADSUniform sceneIndoorADSUniform;

extern GLfloat density;
extern GLfloat gradient;
extern GLfloat skyFogColor[];

extern GLfloat lightAmbient[];
extern GLfloat lightDiffuse[];
extern GLfloat lightSpecular[];
extern GLfloat lightPosition[];
						  
extern GLfloat materialAmbient[];
extern GLfloat materialDiffuse[];
extern GLfloat materialSpecular[];
extern GLfloat materialShininess;

//Model variables
STATIC_MODEL rockModel_in;
STATIC_MODEL streetLightModel_in;
STATIC_MODEL deskModel;
STATIC_MODEL schoolBagModel;

int initializeScene_PlaceHolderIndoor(void)
{
	// Function Declarations

    // Code.

	

#ifdef ENABLE_ADSLIGHT
    // Texture
	// if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
	if (LoadGLTexture_UsingSOIL(&texture_Marble, TEXTURE_DIR"marble.bmp") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}

#endif // ENABLE_ADSLIGHT

	
#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/rock/rock.obj", &rockModel_in);
	loadStaticModel("res/models/streetLight/StreetLight.obj", &streetLightModel_in);
	loadStaticModel("res/models/desk/desk.obj", &deskModel);
	loadStaticModel("res/models/schoolBag/schoolBag.fbx", &schoolBagModel);
#endif

	return 0;
}

void displayScene_PlaceHolderIndoor(void)
{
	// Function Declarations
	void displayPasses(int,bool,bool,bool);
	void displayGodRays(int, int);

	// Code
	// Here The Game STarts

	// set camera
	setCamera();

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);


	//Default Frame Buffer
	
	//glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
	//perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneIndoorADSUniform = useADSShader();

	// Sending Light Related Uniforms
	glUniform1i(sceneIndoorADSUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneIndoorADSUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneIndoorADSUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneIndoorADSUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneIndoorADSUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneIndoorADSUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneIndoorADSUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneIndoorADSUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneIndoorADSUniform.materialShininessUniform, materialShininess);

	glUniform1i(sceneIndoorADSUniform.fogEnableUniform, 0);
	glUniform1f(sceneIndoorADSUniform.densityUniform, density);
	glUniform1f(sceneIndoorADSUniform.gradientUniform, gradient);
	glUniform4fv(sceneIndoorADSUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneIndoorADSUniform.uniform_enable_godRays, 1);
	glUniform1i(sceneIndoorADSUniform.godrays_blackpass_sphere, 0);

	// ------ Rock Model ------
	translationMatrix = vmath::translate(-1.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(rockModel_in);

	// ------ Streetlight Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	
	translationMatrix = vmath::translate(1.0f, -2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(streetLightModel_in);

	// ------ SchoolBag Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();


	translationMatrix = vmath::translate(1.0f, 2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(schoolBagModel);


	// ------ Desk Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(-1.0f, -2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(deskModel);

	// Un-use ShaderProgramObject
	glUseProgram(0);
#endif

}

void updateScene_PlaceHolderIndoor(void)
{
	// Code
//#ifdef ENABLE_ADSLIGHT
//    angleCube = angleCube + 1.0f;
//	if (angleCube >= 360.0f)
//	{
//		angleCube -= 360.0f;
//	}
//
//#endif // ENABLE_ADSLIGHT


	//update camera using lerp
	/*cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);*/
}

void uninitializeScene_PlaceHolderIndoor(void)
{
	// Code

//#ifdef ENABLE_ADSLIGHT
//	if (texture_Marble)
//	{
//		glDeleteTextures(1, &texture_Marble);
//		texture_Marble = NULL;
//	}

//	// uninitializeSphere();
//	// uninitializeTriangle();
//	// uninitializeQuad();
//	// uninitializePyramid();
//	uninitializeCube();
//
//#endif // ENABLE_ADSLIGHT

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&schoolBagModel);
	unloadStaticModel(&deskModel);
	unloadStaticModel(&streetLightModel_in);
	unloadStaticModel(&rockModel_in);
#endif


}