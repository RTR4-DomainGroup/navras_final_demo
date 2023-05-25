#pragma once

// This File Will Be Replaced by Scene*.cpp

//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/debug/debug_transformation.h"
#include "../../inc/effects/GodraysEffect.h"
#include "../../inc/debug/debug_transformation.h"

#ifdef ENABLE_SHADOW
#include "../../inc/helper/shadowframebuffer.h"
#endif // ENABLE_SHADOW

#ifdef ENABLE_STARFIELD
#include "../../inc/effects/StarfieldEffect.h"
#endif // ENABLE_STARFIELD

#ifdef ENABLE_STATIC_MODELS
#include "../../inc/effects/StaticModelLoadingEffect.h"
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#endif // ENABLE_DYNAMIC_MODELS

#include "../../inc/scenes/scene02_EarthAndSpace.h"

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT


extern GLfloat whiteSphere[3];
GLuint texture_earth;
GLuint texture_sun;

//struct ADSUniform sceneEarthAndSpaceADSUniform;
//struct ADSUniform cubeADSUniform;

extern mat4 viewMatrix;

extern GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

extern float displacementmap_depth;

// Variables For Starfieldx
extern GLuint texture_star; 
extern double deltaTime;
extern struct StarfieldUniform sceneStarfieldUniform;
struct ADSUniform adsEarthAndSpaceUniform;

// Varaiables for God Rays
struct GodraysUniform sceneEarthAndSpaceGodRaysUniform;
extern GLfloat lightPosition_gr[];

//struct FSQuadUniform fsqEarthAndSpaceUniform;

// Variables For Skybox
extern GLuint texture_skybox;
extern struct SkyboxUniform sceneSkyBoxUniform;

// Camera angle for rotation
extern GLfloat cameraAngle;

struct FSQuadUniform fsqEarthAndSpaceUniform;

extern int windowWidth;
extern int windowHeight;

extern GLfloat dispersal;
extern GLfloat haloWidth;
extern GLfloat intensity;
extern GLfloat distortion[];

static GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 4.0f, 3.0f, 3.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

extern GLfloat skyFogColor[];
extern GLfloat density;
extern GLfloat gradient;

extern struct FrameBufferDetails fboEarthAndSpace;
extern ShadowFrameBufferDetails shadowFramebuffer;

BOOL bFBRes = FALSE;

int initializeScene02_EarthAndSpace(void)
{
	//// Function Declarations
	//int initializeGodRays_EarthAndSpace(void);

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
	//int initializeGodRays(void);
	initializeCube();
	//initializeGodRays_EarthAndSpace();
	initializeSphere(1.0f, 60, 60);
	//initializeGodRays();
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

	if (LoadGLTexture_UsingSOIL(&texture_earth, TEXTURE_DIR"marble.bmp") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture is Successful = %u!!!\n", texture_earth);
	}

	if (LoadGLTexture_UsingSOIL(&texture_sun, TEXTURE_DIR"Starfield/Sun.png") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture is Successful = %u!!!\n", texture_sun);
	}
#endif // ENABLE_STARFIELD
	
	return 0;
}

//void displayScene1_EarthAndSpace(void)
//{
//	// Function Declarations
//	void displayPasses_EarthAndSpace(int,bool,bool,bool);
//	void displayGodRays(int, int);
//
//	// Code
//	// set cameraa
//	setCamera();
//	//setCamera(&camera);
//
//	//rotateCamera(0.0f, 10.0f, 0.0f, 50.0f, cameraAngle);
//
//	mat4 translationMatrix = mat4::identity();
//	mat4 modelMatrix = mat4::identity();
//	viewMatrix = mat4::identity();
//
//	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
//
//#ifdef ENABLE_STARFIELD
//
//	glBindFramebuffer(GL_FRAMEBUFFER, fboEarthAndSpace.frameBuffer);
//
//	glViewport(0, 0, (GLsizei)fboEarthAndSpace.textureWidth, (GLsizei)fboEarthAndSpace.textureHeight);
//	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboEarthAndSpace.textureWidth / fboEarthAndSpace.textureHeight, 0.1f, 1000.0f);
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	sceneStarfieldUniform = useStarfieldShader();
//
//	float time = (float)deltaTime;
//
//	time = time * 0.05f;
//	time = time - floor(time);
//	LOG("LoadGLTexture is Successful = %f!!!\n", time);
//
//	// Transformations
//	translationMatrix = mat4::identity();
//	mat4 rotationMatrix = mat4::identity();
//	mat4 scaleMatrix = mat4::identity();
//	modelMatrix = mat4::identity();
//
//	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);					// glTranslatef() is replaced by this line.
//	//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
//	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT
//
//	glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
//	glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
//	glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
//	glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);
//	glUniform1f(sceneStarfieldUniform.timeUniform, time);
//
//	displayStarfield(texture_star);
//	glUseProgram(0);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//#endif // ENABLE_STARFIELD
//
//
//	// GodRay Black pass
//	glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPass.frameBuffer);
//	glViewport(0, 0, (GLsizei)fboBlackPass.textureWidth, (GLsizei)fboBlackPass.textureHeight);
//		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPass.textureWidth / fboBlackPass.textureHeight, 0.1f, 1000.0f);
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//2 framebuffers for water effect
//	//displayWaterFramebuffers(0);
//	displayPasses_EarthAndSpace(0, false, false, false);
//
//	sceneEarthAndSpaceADSUniform = useADSShader();
//	translationMatrix = mat4::identity();
//	modelMatrix = mat4::identity();
//	translationMatrix = vmath::translate(lightPosition_gr[0], lightPosition_gr[1], lightPosition_gr[2]);
//	modelMatrix = translationMatrix;
//		
//	glUniformMatrix4fv(sceneEarthAndSpaceADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
//	glUniformMatrix4fv(sceneEarthAndSpaceADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
//	glUniformMatrix4fv(sceneEarthAndSpaceADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
//	glUniform1i(sceneEarthAndSpaceADSUniform.uniform_enable_godRays, 0);
//	glUniform1i(sceneEarthAndSpaceADSUniform.godrays_blackpass_sphere, 1);
//	float color[3] = { 1.0f, 1.0f, 1.0f };
//	displaySphere(color);
//	glUseProgram(0);
//		
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// GodRay Color Pass
//	glBindFramebuffer(GL_FRAMEBUFFER, fboColorPass.frameBuffer);
//	glViewport(0, 0, (GLsizei)fboColorPass.textureWidth, (GLsizei)fboColorPass.textureHeight);
//		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboColorPass.textureWidth / fboColorPass.textureHeight, 0.1f, 1000.0f);
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	//displayWaterFramebuffers(1);
//		displayPasses_EarthAndSpace(1, false, false, false);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// God Rays Pass
//	glBindFramebuffer(GL_FRAMEBUFFER, fboGodRayPass.frameBuffer);
//	glViewport(0, 0, (GLsizei)fboGodRayPass.textureWidth, (GLsizei)fboGodRayPass.textureHeight);
//		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboGodRayPass.textureWidth / fboGodRayPass.textureHeight, 0.1f, 1000.0f);
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	sceneEarthAndSpaceGodRaysUniform = useGodRaysShader();
//
//	translationMatrix = mat4::identity();
//	modelMatrix = mat4::identity();
//	//translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
//	modelMatrix = translationMatrix;
//
//	glUniformMatrix4fv(sceneEarthAndSpaceGodRaysUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
//	glUniformMatrix4fv(sceneEarthAndSpaceGodRaysUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
//	glUniformMatrix4fv(sceneEarthAndSpaceGodRaysUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
//	glUniform1i(sceneEarthAndSpaceGodRaysUniform.godrays_lfEnabled, 0);
//
//	glUniform4fv(sceneEarthAndSpaceGodRaysUniform.lightPositionOnScreen, 1, lightPosition_gr);
//	glUniform1f(sceneEarthAndSpaceGodRaysUniform.decay, 1.0f);
//	glUniform1f(sceneEarthAndSpaceGodRaysUniform.density, 0.92f);
//	glUniform1f(sceneEarthAndSpaceGodRaysUniform.exposure, 0.25f);
//	glUniform1f(sceneEarthAndSpaceGodRaysUniform.weight, 0.04f);
//
//	glUniform1f(sceneEarthAndSpaceGodRaysUniform.dispersalUniform, dispersal);
//	glUniform1f(sceneEarthAndSpaceGodRaysUniform.haloWidthUniform, haloWidth);
//	glUniform1f(sceneEarthAndSpaceGodRaysUniform.intensityUniform, intensity);
//	glUniform3fv(sceneEarthAndSpaceGodRaysUniform.distortionUniform, 1, distortion);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, fboBlackPass.frameBufferTexture);
//		glUniform1i(sceneEarthAndSpaceGodRaysUniform.godraysampler, 0);
//		displayQuad();
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glUseProgram(0);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// Godrays Default Frame Buffer
//	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
//	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	fsqEarthAndSpaceUniform = useFSQuadShader();
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, fboGodRayPass.frameBufferTexture);
//	glUniform1i(fsqEarthAndSpaceUniform.textureSamplerUniform1, 0);
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, fboColorPass.frameBufferTexture);
//		glUniform1i(fsqEarthAndSpaceUniform.textureSamplerUniform2, 1);
//		displayQuad();
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glUseProgram(0);
//
//}

void displayScene02_Passes(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0)
{	
	// Code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	setCamera();
	//setCamera(&camera);

	mat4 lightSpaceMatrix;
	mat4 finalViewMatrix = mat4::identity();

	if (actualDepthQuadScene == 0) 
	{

		finalViewMatrix = viewMatrix;

	}
	else if (actualDepthQuadScene == 1) {

		finalViewMatrix = mat4::identity();
		finalViewMatrix = lookat(vec3(lightPosition[0], lightPosition[1], lightPosition[2]), vec3(0.0f, -5.0f, -20.0f), vec3(0.0f, 1.0f, 0.0f));
		//finalViewMatrix = viewMatrix;

#ifdef ENABLE_SHADOW
		lightSpaceMatrix = mat4::identity();
		lightSpaceMatrix = perspectiveProjectionMatrix * finalViewMatrix;
		//lightSpaceMatrix = ortho(-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f);
		//lightSpaceMatrix = lightSpaceMatrix * finalViewMatrix;
#endif // ENABLE_SHADOW

	}
	adsEarthAndSpaceUniform = useADSShader();

	glUniformMatrix4fv(adsEarthAndSpaceUniform.laUniform, 1, GL_FALSE, lightAmbient);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.ldUniform, 1, GL_FALSE, lightDiffuse);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.lsUniform, 1, GL_FALSE, lightSpecular);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.lightPositionUniform, 1, GL_FALSE, lightPosition);

	glUniformMatrix4fv(adsEarthAndSpaceUniform.kaUniform, 1, GL_FALSE, materialAmbient);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.kdUniform, 1, GL_FALSE, materialDiffuse);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.ksUniform, 1, GL_FALSE, materialSpecular);
	glUniform1f(adsEarthAndSpaceUniform.materialShininessUniform, materialShininess);

	glUniform1i(adsEarthAndSpaceUniform.uniform_enable_godRays, godRays);

	glUniform1f(adsEarthAndSpaceUniform.gradientUniform, gradient);
	glUniform1f(adsEarthAndSpaceUniform.densityUniform, density);

	glUniform1i(adsEarthAndSpaceUniform.fogEnableUniform, 0);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.skyFogColorUniform, 1, GL_FALSE, skyFogColor);
	glUniform1i(adsEarthAndSpaceUniform.godrays_blackpass_sphere, 0);

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(adsEarthAndSpaceUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_earth);
	//glUniform1i(fboEarthAndSpace.textureSamplerUniform_diffuse, 0);

	float color[3] = { 0.0f, 0.0f, 0.0f };
	if (godRays == 1)
	{
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
	}
	displaySphere(color);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Cube
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);					// glTranslatef() is replaced by this line.
	scaleMatrix = vmath::scale(200.0f, 200.0f, 200.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(adsEarthAndSpaceUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboEarthAndSpace.frameBufferTexture);
	//glUniform1i(adsEarthAndSpaceUniform.textureSamplerUniform_diffuse, 0);

	displayCube();
	glBindTexture(GL_TEXTURE_2D, 0);

	// Sun
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 20.0f, -100.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(0.0f, 10.0f, -100.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(adsEarthAndSpaceUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	if (actualDepthQuadScene == 1) 
	{
		glUniform1i(adsEarthAndSpaceUniform.actualSceneUniform, 0);
		glUniform1i(adsEarthAndSpaceUniform.depthSceneUniform, 1);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
	}
	else 
	{
		glUniform1i(adsEarthAndSpaceUniform.actualSceneUniform, 1);
		glUniform1i(adsEarthAndSpaceUniform.depthSceneUniform, 0);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
	}

	glUniformMatrix4fv(adsEarthAndSpaceUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(adsEarthAndSpaceUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_sun);
	glUniform1i(adsEarthAndSpaceUniform.textureSamplerUniform_diffuse, 0);

	displayCube();
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

}

//int initializeGodRays_EarthAndSpace(void)
//{
//	fboBlackPass.textureWidth = FBO_WIDTH;
//	fboBlackPass.textureHeight = FBO_HEIGHT;
//
//	fboColorPass.textureWidth = FBO_WIDTH;
//	fboColorPass.textureHeight = FBO_HEIGHT;
//
//	fboGodRayPass.textureWidth = FBO_WIDTH;
//	fboGodRayPass.textureHeight = FBO_HEIGHT;
//
//	createFBO(&fboBlackPass);
//	createFBO(&fboColorPass);
//	createFBO(&fboGodRayPass);
//
//	fboEarthAndSpace.textureWidth = FBO_WIDTH;
//	fboEarthAndSpace.textureHeight = FBO_HEIGHT;
//	createFBO(&fboEarthAndSpace);
//
//	return(0);
//}

//void resize_godRayPasses_EarthAndSpace(int fboWidh, int fboHeight)
//{
//	glViewport(0, 0, (GLsizei)fboWidh, (GLsizei)fboHeight);
//}

void updateScene02_EarthAndSpace(void)
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
	deltaTime = updateStarfield(deltaTime);
#endif

	// update camera using lerp
	//cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	//cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);
	cameraAngle = cameraAngle + 0.5f;
	if (cameraAngle >= 360.0f)
		cameraAngle -= 360.0f;
}

void uninitializeScene02_EarthAndSpace(void)
{
	// Code
#ifdef ENABLE_STARFIELD
	if (texture_sun)
	{
		glDeleteTextures(1, &texture_sun);
		texture_sun = NULL;
	}

	if (texture_earth)
	{
		glDeleteTextures(1, &texture_earth);
		texture_earth = NULL;
	}

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


