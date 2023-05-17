#pragma once

// This File Will Be Replaced by Scene*.cpp

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/waterframebuffer.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"

#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/BillboardingShader.h"

#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"

#include "../../inc/scenes/scenePlaceHolderOutdoor.h"
#include "../../inc/scenes/sceneEarthAndSpace.h"


#define FBO_WIDTH 1920
#define FBO_HEIGHT 1080
//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####

#define ENABLE_STARFIELD
#define ENABLE_GODRAYS

extern GLfloat whiteSphere[3];
extern GLuint texture_Marble;

extern struct ADSUniform sceneOutdoorADSUniform;

extern mat4 viewMatrix;

extern GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

extern float displacementmap_depth;

// Variables For Starfieldx
extern GLuint texture_star; 
extern double deltaTime;
extern struct StarfieldUniform sceneStarfieldUniform;
extern struct ADSUniform adsUniform;

// Varaiables for God Rays
extern struct GodraysUniform sceneGodRaysUniform;
extern GLfloat lightPosition_gr[];

//struct FSQuadUniform fsqUniform;

// Camera angle for rotation
extern GLfloat cameraAngle;

extern struct FrameBufferDetails fboBlackPass;
extern struct FrameBufferDetails fboColorPass;
extern struct FrameBufferDetails fboGodRayPass;
extern struct FSQuadUniform fsqUniform;

extern int windowWidth;
extern int windowHeight;

extern GLfloat dispersal;
extern GLfloat haloWidth;
extern GLfloat intensity;
extern GLfloat distortion[];

extern GLfloat lightAmbient[];
extern GLfloat lightDiffuse[];
extern GLfloat lightSpecular[];
extern GLfloat lightPosition[];

extern GLfloat materialAmbient[];
extern GLfloat materialDiffuse[];
extern GLfloat materialSpecular[];
extern GLfloat materialShininess;

int initializeScene1_EarthAndSpace(void)
{
	// Function Declarations

	// set Camera location
	cameraEyeX = 0.0f;
	cameraEyeY = 0.0f;
	cameraEyeZ = 6.0f;

	cameraCenterX = 0.0f;
	cameraCenterY = 0.0f;
	cameraCenterZ = 0.0f;

	cameraUpX = 0.0f;
	cameraUpY = 1.0f;
	cameraUpZ = 0.0f;

    // Code.
	//initializeCamera(&camera);

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

#ifdef ENABLE_GODRAYS
	int initializeGodRays(void);
	initializeSphere(1.0f, 60, 60);
	initializeGodRays();
	initializeQuad();
#endif // ENABLE_GODRAYS

#ifdef ENABLE_STARFIELD
	if (initializeStarfield(&texture_star, TEXTURE_DIR"Starfield/Star.png") != 0)
	{
		LOG("initializeStarfield() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeStarfield() Successfull!!!\n");
	}

	if (LoadGLTexture_UsingSOIL(&texture_Marble, TEXTURE_DIR"Fog/smiley.bmp") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture for marble is Successful = %u!!!\n", texture_Marble);
	}

#endif // ENABLE_STARFIELD
	
	return 0;
}

void displayScene1_EarthAndSpace(void)
{
	// Function Declarations
	void displayPasses_EarthAndSpace(int,bool,bool,bool);
	void displayGodRays(int, int);

	// Code
	// Here The Game STarts

	// set cameraa

	setCamera();
	//setCamera(&camera);

	//rotateCamera(0.0f, 10.0f, 0.0f, 50.0f, cameraAngle);

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);

		// GodRay Black pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboBlackPass.textureWidth, (GLsizei)fboBlackPass.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPass.textureWidth / fboBlackPass.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//2 framebuffers for water effect
		//displayWaterFramebuffers(0);
		displayPasses_EarthAndSpace(0, false, false, true);

		sceneOutdoorADSUniform = useADSShader();
		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		translationMatrix = vmath::translate(lightPosition_gr[0], lightPosition_gr[1], lightPosition_gr[2]);
		modelMatrix = translationMatrix;
		
		glUniformMatrix4fv(sceneOutdoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(sceneOutdoorADSUniform.lightingEnableUniform, 0);
		glUniform1i(sceneOutdoorADSUniform.uniform_enable_godRays, 0);
		glUniform1i(sceneOutdoorADSUniform.godrays_blackpass_sphere, 1);
		float color[3] = { 1.0f, 1.0f, 1.0f };
		displaySphere(color);
		glUseProgram(0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// GodRay Color Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboColorPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboColorPass.textureWidth, (GLsizei)fboColorPass.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboColorPass.textureWidth / fboColorPass.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//displayWaterFramebuffers(1);
			displayPasses_EarthAndSpace(1, false, false, true);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// God Rays Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboGodRayPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboGodRayPass.textureWidth, (GLsizei)fboGodRayPass.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboGodRayPass.textureWidth / fboGodRayPass.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneGodRaysUniform = useGodRaysShader();

		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		//translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix;

		glUniform4fv(sceneGodRaysUniform.lightPositionOnScreen, 1, lightPosition_gr);
		glUniform1f(sceneGodRaysUniform.decay, 1.0f);
		glUniform1f(sceneGodRaysUniform.density, 0.92f);
		glUniform1f(sceneGodRaysUniform.exposure, 0.25f);
		glUniform1f(sceneGodRaysUniform.weight, 0.04f);

		glUniformMatrix4fv(sceneGodRaysUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneGodRaysUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneGodRaysUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(sceneGodRaysUniform.godrays_lfEnabled, 1);

		glUniform1f(sceneGodRaysUniform.dispersalUniform, dispersal);
		glUniform1f(sceneGodRaysUniform.haloWidthUniform, haloWidth);
		glUniform1f(sceneGodRaysUniform.intensityUniform, intensity);
		glUniform3fv(sceneGodRaysUniform.distortionUniform, 1, distortion);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboBlackPass.frameBufferTexture);
		glUniform1i(sceneGodRaysUniform.godraysampler, 0);
		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Godrays Default Frame Buffer
		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsqUniform = useFSQuadShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboGodRayPass.frameBufferTexture);
		glUniform1i(fsqUniform.textureSamplerUniform1, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fboColorPass.frameBufferTexture);
		glUniform1i(fsqUniform.textureSamplerUniform2, 1);
		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

}

void displayPasses_EarthAndSpace(int godRays = 1, bool recordWaterReflectionRefraction = false ,bool isReflection = false, bool waterDraw = false) 
{	
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	mat4 rotateX = mat4::identity();

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	setCamera();
	//setCamera(&camera);

#ifdef ENABLE_STARFIELD

	sceneStarfieldUniform = useStarfieldShader();

	float time = (float)deltaTime;

	time = time * 0.05f;
	time = time - floor(time);

	// Transformations
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -56.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);
	glUniform1i(sceneStarfieldUniform.uniform_enable_godRays, godRays);
	glUniform1f(sceneStarfieldUniform.timeUniform, time);

	displayStarfield(texture_star);
	glUseProgram(0);

	adsUniform = useADSShader();

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -8.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(adsUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(adsUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(adsUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniform1i(adsUniform.uniform_enable_godRays, godRays);

	glUniformMatrix4fv(adsUniform.laUniform, 1, GL_FALSE, lightAmbient);
	glUniformMatrix4fv(adsUniform.ldUniform, 1, GL_FALSE, lightDiffuse);
	glUniformMatrix4fv(adsUniform.lsUniform, 1, GL_FALSE, lightSpecular);
	glUniformMatrix4fv(adsUniform.lightPositionUniform, 1, GL_FALSE, lightPosition);

	glUniformMatrix4fv(adsUniform.kaUniform, 1, GL_FALSE, materialAmbient);
	glUniformMatrix4fv(adsUniform.kdUniform, 1, GL_FALSE, materialDiffuse);
	glUniformMatrix4fv(adsUniform.ksUniform, 1, GL_FALSE, materialSpecular);
	glUniform1f(adsUniform.materialShininessUniform, materialShininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_Marble);
	glUniform1i(adsUniform.textureSamplerUniform, 0);
	
	float color[3] = { 0.0f, 0.0f, 1.0f };
	if (godRays == 1)
	{
		color[0] = 1.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
	}
	displaySphere(color);

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

#endif // ENABLE_STARFIELD

}

int initializeGodRays_EarthAndSpace(void)
{
	fboBlackPass.textureWidth = FBO_WIDTH;
	fboBlackPass.textureHeight = FBO_HEIGHT;

	fboColorPass.textureWidth = FBO_WIDTH;
	fboColorPass.textureHeight = FBO_HEIGHT;

	fboGodRayPass.textureWidth = FBO_WIDTH;
	fboGodRayPass.textureHeight = FBO_HEIGHT;

	createFBO(&fboBlackPass);
	createFBO(&fboColorPass);
	createFBO(&fboGodRayPass);

	return(0);
}

void resize_godRayPasses_EarthAndSpace(int fboWidh, int fboHeight)
{
	glViewport(0, 0, (GLsizei)fboWidh, (GLsizei)fboHeight);
}

void updateScene1_EarthAndSpace(void)
{
	// Code
#ifdef ENABLE_ADSLIGHT
    angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
	{
		angleCube -= 360.0f;
	}

#endif // ENABLE_ADSLIGHT

#ifdef ENABLE_STARFIELD
	//deltaTime = updateStarfield(deltaTime);
#endif

	// update camera using lerp
	//cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	//cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);
	cameraAngle = cameraAngle + 0.5f;
	if (cameraAngle >= 360.0f)
		cameraAngle -= 360.0f;
}

void uninitializeScene1_EarthAndSpace(void)
{
	// Code
#ifdef ENABLE_STARFIELD
	uninitializeStarfield(texture_star);
#endif // ENABLE_STARFIELD

#ifdef ENABLE_ADSLIGHT
	if (texture_Marble)
	{
		glDeleteTextures(1, &texture_Marble);
		texture_Marble = NULL;
	}

	// uninitializeSphere();
	uninitializeCube();

#endif // ENABLE_ADSLIGHT

	//uninitializeCamera(&camera);

}
