#pragma once
// This File Will Be Replaced by Scene*.cpp



#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/scenes/scenePlaceHolderOutdoor.h"




#ifdef ENABLE_WATER
#include "../../inc/helper/waterframebuffer.h"
#endif // ENABLE_WATER

#ifdef ENABLE_GODRAYS
#endif // ENABLE_GODRAYS

#ifdef ENABLE_SHADOW
#include "../../inc/helper/shadowframebuffer.h"
#endif // ENABLE_SHADOW

#ifdef ENABLE_BILLBOARDING
#include "../../inc/shaders/BillboardingShader.h"
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_ATMOSPHERE
#include "../../inc/shaders/AtmosphereShader.h"
#include "../../inc/effects/AtmosphereEffect.h"
#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_VIDEO_RENDER
#include "../../inc/effects/videoEffect.h"
#endif // ENABLE_VIDEO_RENDER

#ifdef ENABLE_TERRIAN
#include "../../inc/effects/TerrainEffect.h"
#endif // ENABLE_TERRIAN

#ifdef ENABLE_STARFIELD
#include "../../inc/effects/StarfieldEffect.h"
#endif // ENABLE_STARFIELD

#ifdef ENABLE_CLOUD_NOISE
#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_SKYBOX
#include "../../inc/effects/SkyboxEffect.h"
#endif // ENABLE_SKYBOX

#ifdef ENABLE_CLOUD_NOISE
#include "../../inc/effects/CloudEffect.h"
#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_WATER
#include "../../inc/effects/WaterEffect.h"
#endif // ENABLE_WATER

#ifdef ENABLE_GODRAYS
#include "../../inc/effects/GodraysEffect.h"
#endif // ENABLE_GODRAYS

#ifdef ENABLE_GAUSSIAN_BLUR
#include "../../inc/effects/GaussianBlurEffect.h"
#endif // ENABLE_GAUSSIAN_BLUR





#define FBO_WIDTH 1920
#define FBO_HEIGHT 1080


GLfloat whiteSphere[3] = {1.0f, 1.0f, 1.0f};
GLuint texture_Marble;
TEXTURE texture_grass;
TEXTURE texture_flower;

struct ADSUniform sceneOutdoorADSUniform;

struct FSQuadUniform fsqUniform;

#ifdef ENABLE_TERRIAN
struct TerrainUniform terrainUniform;
#endif // ENABLE_TERRIAN

#ifdef ENABLE_CLOUD_NOISE
struct CloudNoiseUniform sceneCloudNoiseUniform;
#endif // ENABLE_CLOUD_NOISE

struct TextureVariables terrainTextureVariables;

#ifdef ENABLE_BILLBOARDING
// variables for billboarding
struct BillboardingUniform billboardingEffectUniform;
GLuint frameTime = 0;
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_WATER
// Water Related Variables
struct WaterUniform waterUniform;
struct TextureVariables waterTextureVariables;
struct WaterFrameBufferDetails waterReflectionFrameBufferDetails;
struct WaterFrameBufferDetails waterRefractionFrameBufferDetails;
#endif // ENABLE_WATER

#ifdef ENABLE_GAUSSIAN_BLUR
// Gaussian Blur related variables
struct GaussianBlurEffect gaussianBlurEffect;
struct HorrizontalBlurUniform horizontalBlurUniform;
struct VerticalBlurUniform verticalBlurUniform;
struct FrameBufferDetails fullSceneFbo;
struct FSQuadUniform fsGaussBlurQuadUniform;
#endif // ENABLE_GAUSSIAN_BLUR


#ifdef ENABLE_ATMOSPHERE
// Atmosphere Scattering
AtmosphereUniform atmosphereUniform;
AtmosphericVariables atmosVariables;
#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_SHADOW
// Shadow
ShadowFrameBufferDetails shadowFramebuffer;
mat4 viewmatrixDepth;
mat4 lightSpaceMatrix;
mat4 perspectiveProjectionDepth;
#endif // ENABLE_SHADOW

GLfloat waterHeight = 0.0f;
GLfloat moveFactor = 0.0f;
GLfloat planeReflection[] = { 0.0f, 1.0f, 0.0f, -waterHeight };
GLfloat planeRefration[] = { 0.0f, -1.0f, 0.0f, waterHeight };
struct FrameBufferDetails fboBlackPass;
struct FrameBufferDetails fboColorPass;
struct FrameBufferDetails fboGodRayPass;

extern int windowWidth;
extern int windowHeight;


float myScale = 1.0f;

float noiseScale = 2.0f;
bool noiseScaleIncrement = true;

GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition[] = { 10.0f, 10.0f, 10.0f, 1.0f };

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;

mat4 viewMatrix;


GLfloat skyColor[] = { 0.0f, 0.0f, 0.8f, 0.0f };
GLfloat cloudColor[] = { 0.8f, 0.8f, 0.8f, 0.0f };

GLuint noise_texture;

GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

float displacementmap_depth;

#ifdef ENABLE_SKYBOX
// Variables For Skybox
GLuint texture_skybox;
struct SkyboxUniform sceneSkyBoxUniform;
#endif // ENABLE_SKYBOX

#ifdef ENABLE_STARFIELD
// Variables For Starfieldx
GLuint texture_star; 
double deltaTime;
struct StarfieldUniform sceneStarfieldUniform;
#endif // ENABLE_STARFIELD

GLfloat density = 0.15;
GLfloat gradient = 0.5;
GLfloat skyFogColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };


#ifdef ENABLE_GODRAYS
// Varaiables for God Rays
struct GodraysUniform sceneGodRaysUniform;
GLfloat lightPosition_gr[] = {0.0f, 10.0f, -100.0f, 1.0f};
#endif // ENABLE_GODRAYS

// Camera angle for rotation
GLfloat cameraAngle = 0.0f;
GLfloat dispersal = 0.1875f;
GLfloat haloWidth = 0.45f;
GLfloat intensity = 1.5f;
GLfloat distortion[] = { 0.94f, 0.97f, 1.0f };

typedef void (* DISPLAY_PASSES) (int,bool,bool,bool,int);
extern Camera camera;


int initializeScene_PlaceHolderOutdoor(void)
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
	// initializeCamera(&camera);

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

#ifdef ENABLE_SHADOW

	shadowFramebuffer.textureWidth = 1024;
	shadowFramebuffer.textureHeight = 1024;

	if (shadowCreateFBO(&shadowFramebuffer) == FALSE) {

		LOG("shadowCreateFBO() For Shadow FAILED!!!\n");
			return(-1);

	}
	else {

		LOG("shadowCreateFBO() Successfull for Shadow!!!\n");

	}

	initializeQuad();

#endif // ENABLE_SHADOW

#ifdef ENABLE_TERRIAN
	displacementmap_depth = 15.0f;

	terrainTextureVariables.albedoPath = TEXTURE_DIR"terrain/DiffuseMapTerrain.jpg";
	terrainTextureVariables.displacementPath = TEXTURE_DIR"terrain/DisplacementMapTerrain.jpg";
	terrainTextureVariables.normalPath = TEXTURE_DIR"terrain/NormalMapTerrain.jpg";

	if (initializeTerrain(&terrainTextureVariables) != 0) 
	{
		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}
	
#endif // ENABLE_TERRIAN

#ifdef ENABLE_ATMOSPHERE

	//
	atmosVariables.m_nSamples = 3;		// Number of sample rays to use in integral equation
	atmosVariables.m_Kr = 0.0035f;		// Rayleigh scattering constant
	atmosVariables.m_Kr4PI = atmosVariables.m_Kr * 4.0f * M_PI;
	atmosVariables.m_Km = 0.0015f;		// Mie scattering constant
	atmosVariables.m_Km4PI = atmosVariables.m_Km * 4.0f * M_PI;
	atmosVariables.m_ESun = 20.0f;		// Sun brightness constant
	atmosVariables.m_g = -0.990f;		// The Mie phase asymmetry factor
	atmosVariables.m_fExposure = 2.0f;

	atmosVariables.m_fInnerRadius = 10.0f;
	atmosVariables.m_fOuterRadius = 50.0f;
	//atmosVariables.m_fOuterRadius = atmosVariables.m_fInnerRadius + (atmosVariables.m_fInnerRadius * 2.5f);
	atmosVariables.m_fScale = 1 / (atmosVariables.m_fOuterRadius - atmosVariables.m_fInnerRadius);

	atmosVariables.m_fWavelength[0] = 0.650f;		// 650 nm for red
	atmosVariables.m_fWavelength[1] = 0.570f;		// 570 nm for green
	atmosVariables.m_fWavelength[2] = 0.475f;		// 475 nm for blue
	atmosVariables.m_fWavelength4[0] = powf(atmosVariables.m_fWavelength[0], 4.0f);
	atmosVariables.m_fWavelength4[1] = powf(atmosVariables.m_fWavelength[1], 4.0f);
	atmosVariables.m_fWavelength4[2] = powf(atmosVariables.m_fWavelength[2], 4.0f);

	atmosVariables.m_fRayleighScaleDepth = 0.25f;
	atmosVariables.m_fMieScaleDepth = 0.1f;

	atmosVariables.m_vLight = vec3(0, 0, -350);
	atmosVariables.m_vLightDirection = atmosVariables.m_vLight / sqrtf(atmosVariables.m_vLight[0] * atmosVariables.m_vLight[0] + atmosVariables.m_vLight[1] * atmosVariables.m_vLight[1] + atmosVariables.m_vLight[2] * atmosVariables.m_vLight[2]);

	//
	initializeAtmosphere(atmosVariables);

#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_GODRAYS
	int initializeGodRays(void);
	initializeSphere(1.0f, 60, 60);
	initializeGodRays();
	initializeQuad();
#endif // ENABLE_GODRAYS

#ifdef ENABLE_WATER

	waterTextureVariables.displacementPath = "res/textures/water/waterDUDV.bmp";

	if (initializeWater(&waterTextureVariables) != 0) {

		LOG("initializeWater() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("initializeWater() Successfull!!!\n");
	}

	//
	waterReflectionFrameBufferDetails.textureWidth = 1280;
	waterReflectionFrameBufferDetails.textureHeight = 720;

	if (waterCreateFBO(&waterReflectionFrameBufferDetails) == FALSE) {

		LOG("waterCreateFBO() For WaterReflection FAILED!!!\n");
		return(-1);

	}
	else {

		LOG("waterCreateFBO() Successfull for WaterReflection!!!\n");

	}

	waterRefractionFrameBufferDetails.textureWidth = 1280;
	waterRefractionFrameBufferDetails.textureHeight = 720;

	if (waterCreateFBO(&waterRefractionFrameBufferDetails) == FALSE) {

		LOG("waterCreateFBO() For WaterRefraction FAILED!!!\n");
		return(-1);

	}
	else {

		LOG("waterCreateFBO() Successfull for Water Refraction!!!\n");

	}

#endif // ENABLE_WATER

#ifdef ENABLE_SKYBOX
	if (initializeSkybox(&texture_skybox, TEXTURE_DIR"Skybox\\") != 0)
	{

		LOG("initializeSkybox() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("initializeSkybox() Successfull!!!\n");
	}
#endif // ENABLE_SKYBOX

#ifdef ENABLE_CLOUD_NOISE

	noise_texture = initializeCloud();
	if (noise_texture == 0)
	{
		LOG("initializeCloud() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeCloud() Successfull!!!\n");
	}

#endif // ENABLE_CLOUD_NOISE

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
#endif // ENABLE_STARFIELD


#ifdef ENABLE_BILLBOARDING
	GLfloat instance_positions[NO_OF_INSTANCES * 4] = {};
	// generate positions per instance
	for(int i = 0; i < NO_OF_INSTANCES; i++)
	{
		instance_positions[(i*4)+0] = (((GLfloat)rand() / RAND_MAX) * (X_MAX - X_MIN)) + X_MIN;
		instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (Y_MAX - Y_MIN)) + Y_MIN;
		instance_positions[(i*4)+2] = (((GLfloat)rand() / RAND_MAX) * (Z_MAX - Z_MIN)) + Z_MIN;
		instance_positions[(i*4)+3] = 1.0f;
		// LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
	}

	// sort z vertices
	for(int i = 0; i < NO_OF_INSTANCES; i++)
	{
		for (int j = i + 1; j < NO_OF_INSTANCES; ++j)
		{
			if(instance_positions[(i*4)+2] > instance_positions[(j*4)+2]) 
			{
				auto a = instance_positions[(i*4)+2];
				instance_positions[(i*4)+2] = instance_positions[(j*4)+2];
				instance_positions[(j*4)+2] = a; 
			}
		}
	}

	initializeInstancedQuad(NO_OF_INSTANCES, instance_positions);

	char imagefile[64] = {};
	sprintf(imagefile, "%s", TEXTURE_DIR"\\billboarding\\grass.png");
	if (LoadGLTextureData_UsingSOIL(&texture_grass, imagefile) == GL_FALSE)
	{
		LOG("Texture loading failed for image %s\n", imagefile);
		return (-6);
	}

	sprintf(imagefile, "%s", TEXTURE_DIR"\\billboarding\\flower.png");
	if (LoadGLTextureData_UsingSOIL(&texture_flower, imagefile) == GL_FALSE)
	{
		LOG("Texture loading failed for image %s\n", imagefile);
		return (-6);
	}

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_GAUSSIAN_BLUR
	initializeQuad();
	if(initializeGaussianBlur(&gaussianBlurEffect) == false)
	{
		LOG("Initialize Gaussian Blur Effect FAILED!!");
		return (-7);
	}

	fullSceneFbo.textureWidth = 1920;
	fullSceneFbo.textureHeight = 1080;

	if (createFBO(&fullSceneFbo) == false)
	{
		LOG("Unable to create FBO for entire scene");
		return (-8);
	}
	
#endif // ENABLE_GAUSSIAN_BLUR

	return 0;
}

void displayScene_PlaceHolderOutdoor(DISPLAY_PASSES displayPasses, bool isGodRequired, bool isWaterRequired, bool isGaussianBlurRequired)
{
	// Function Declarations
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

	if(isWaterRequired) {
		// Water Frame Buffers
		// Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionFrameBufferDetails.frameBuffer);
		glViewport(0, 0, (GLsizei)waterReflectionFrameBufferDetails.textureWidth, (GLsizei)waterReflectionFrameBufferDetails.textureHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterReflectionFrameBufferDetails.textureWidth / waterReflectionFrameBufferDetails.textureHeight, 0.1f, 1000.0f);
		displayPasses(1, true, true, false, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Refraction
		glBindFramebuffer(GL_FRAMEBUFFER, waterRefractionFrameBufferDetails.frameBuffer);
		glViewport(0, 0, (GLsizei)waterRefractionFrameBufferDetails.textureWidth, (GLsizei)waterRefractionFrameBufferDetails.textureHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterRefractionFrameBufferDetails.textureWidth / waterRefractionFrameBufferDetails.textureHeight, 0.1f, 1000.0f);
		displayPasses(1, true, false, false, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CLIP_DISTANCE0);
	}

	#ifdef ENABLE_SHADOW

		// Shadow
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer.frameBuffer);
		glViewport(0, 0, (GLsizei)shadowFramebuffer.textureWidth, (GLsizei)shadowFramebuffer.textureHeight);
		glClear(GL_DEPTH_BUFFER_BIT);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)shadowFramebuffer.textureWidth / shadowFramebuffer.textureHeight, 0.1f, 100.0f);
		displayPasses(1, true, true, isWaterRequired, 1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	#endif // ENABLE_SHADOW

		//////////////////////////////////////////////////////////////
		/*glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight,
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsqUniform = useFSQuadShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
		glUniform1i(fsqUniform.textureSamplerUniform1, 0);

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);*/

	if(!isGaussianBlurRequired && !isGodRequired) {
		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, 
			(GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(1, false, false, isWaterRequired, 0);
	}
	
	else if(isGaussianBlurRequired) {

		displayPasses(1, false, false, isWaterRequired, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, fullSceneFbo.frameBuffer);
		glViewport(0, 0, (GLsizei)fullSceneFbo.textureWidth, (GLsizei)fullSceneFbo.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fullSceneFbo.textureWidth / fullSceneFbo.textureHeight, 
		0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(1, false, false, isWaterRequired, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		displayGaussianBlur();

		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsGaussBlurQuadUniform = useFSQuadShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurEffect.verticalFBDetails.frameBufferTexture);
		glUniform1i(fsGaussBlurQuadUniform.textureSamplerUniform1, 0);
		displayQuad();
		glUseProgram(0);
    	glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		// GodRay Black pass
		if(isGodRequired)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPass.frameBuffer);
			glViewport(0, 0, (GLsizei)fboBlackPass.textureWidth, (GLsizei)fboBlackPass.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPass.textureWidth / fboBlackPass.textureHeight, 
				0.1f, 1000.0f);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			displayPasses(0, false, false, isWaterRequired, 0);

			sceneOutdoorADSUniform = useADSShader();
			translationMatrix = mat4::identity();
			modelMatrix = mat4::identity();
			translationMatrix = vmath::translate(lightPosition_gr[0], lightPosition_gr[1], lightPosition_gr[2]);
			modelMatrix = translationMatrix;
			
			glUniformMatrix4fv(sceneOutdoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(sceneOutdoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(sceneOutdoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
			glUniform1i(sceneOutdoorADSUniform.uniform_enable_godRays, 0);
			glUniform1i(sceneOutdoorADSUniform.godrays_blackpass_sphere, 1);
			float color[3] = {1.0f, 1.0f, 1.0f};
			glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f,1.0f,1.0f));
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
			displayPasses(1, false, false, isWaterRequired, 0);
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
	}
}


#ifdef ENABLE_BILLBOARDING
void displayBillboarding(int godRays = 1)
{
	// variable declaration
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	
	// code
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	billboardingEffectUniform = useBillboardingShader();

	// instanced quads with grass texture
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	/// Grass
	if (texture_grass.height > texture_grass.width)
		scaleMatrix = vmath::scale(texture_grass.width / (GLfloat)texture_grass.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_grass.height / (GLfloat)texture_grass.width, 1.0f);

	translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniform1i(billboardingEffectUniform.textureSamplerUniform, 0);
	glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 1);
	glUniform1i(billboardingEffectUniform.frameTimeUniform, frameTime);
	glUniform1i(billboardingEffectUniform.uniform_enable_godRays, godRays);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_grass.id);
	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw
	glBindTexture(GL_TEXTURE_2D, 0);


	/// Flower
	if (texture_flower.height > texture_flower.width)
		scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

	translationMatrix = vmath::translate(-1.5f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_flower.id);
	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
	glDisable(GL_BLEND);

}
#endif // ENABLE_BILLBOARDING	

int initializeGodRays(void)
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

void resize_godRayPasses(int fboWidh, int fboHeight)
{
	glViewport(0, 0, (GLsizei)fboWidh, (GLsizei)fboHeight);
}

#ifdef ENABLE_GAUSSIAN_BLUR
void displayGaussianBlur(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurEffect.horrizontalFBDetails.frameBuffer);
	glViewport(0, 0, (GLsizei)gaussianBlurEffect.horrizontalFBDetails.textureWidth, 
	(GLsizei)gaussianBlurEffect.horrizontalFBDetails.textureHeight);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    horizontalBlurUniform = useHorrizontalBlurShader();

    glUniform1f(horizontalBlurUniform.targetWidth, 960.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fullSceneFbo.frameBufferTexture);
    glUniform1i(horizontalBlurUniform.hblurTexSamplerUniform, 0);
	displayQuad();    
    glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurEffect.verticalFBDetails.frameBuffer);
	glViewport(0, 0, (GLsizei)gaussianBlurEffect.verticalFBDetails.textureWidth, 
	(GLsizei)gaussianBlurEffect.verticalFBDetails.textureHeight);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	verticalBlurUniform = useVerticalBlurShader();
	glUniform1f(verticalBlurUniform.targetHeight, 540.0f);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gaussianBlurEffect.horrizontalFBDetails.frameBufferTexture);
    glUniform1i(verticalBlurUniform.vblurTexSamplerUniform, 0);
	displayQuad();
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
#endif // ENABLE_GAUSSIAN_BLUR

void updateScene_PlaceHolderOutdoor(void)
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
#endif // ENABLE_STARFIELD

#ifdef ENABLE_CLOUD_NOISE
	// update Cloud
	updateCloud(noiseScaleIncrement, noiseScale, 0.0001f);
#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_BILLBOARDING
	frameTime += 1;

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_WATER

	moveFactor += 0.0003f;
	if (moveFactor >= 360.0f)
		moveFactor -= 360.0f;
#endif // ENABLE_WATER

	// update camera using lerp
	//cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	//cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);
	cameraAngle = cameraAngle + 0.5f;
	if (cameraAngle >= 360.0f)
		cameraAngle -= 360.0f;
}

void uninitializeScene_PlaceHolderOutdoor(void)
{
	// Code
#ifdef ENABLE_BILLBOARDING
    uninitializeInstancedQuads();

	// texture
    if(texture_flower.id)
    {
        glDeleteTextures(1, &texture_flower.id);
        texture_flower.id = 0;
    }
    if(texture_grass.id)
    {
        glDeleteTextures(1, &texture_grass.id);
        texture_grass.id = 0;
    }
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_WATER
	uninitializeWater(&waterTextureVariables);
#endif // ENABLE_WATER

#ifdef ENABLE_STARFIELD
	uninitializeStarfield(texture_star);
#endif // ENABLE_STARFIELD

#ifdef ENABLE_SKYBOX
	uninitialiseSkybox(texture_skybox);
#endif // ENABLE_SKYBOX

#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariables);
#endif // ENABLE_TERRIAN

#ifdef ENABLE_ATMOSPHERE
	uninitializeAtmosphere();
#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_CLOUD_NOISE
	
	uninitializeCloud();
	if (noise_texture)
	{
		glDeleteTextures(1, &noise_texture);
		noise_texture = 0;
	}
#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_ADSLIGHT
	if (texture_Marble)
	{
		glDeleteTextures(1, &texture_Marble);
		texture_Marble = NULL;
	}

	// uninitializeTerrain();
	// uninitializeSphere();
	// uninitializeTriangle();
	// uninitializeQuad();
	// uninitializePyramid();
	uninitializeCube();

#endif // ENABLE_ADSLIGHT


#ifdef ENABLE_GAUSSIAN_BLUR
	uninitializeGaussianBlur(&gaussianBlurEffect);
#endif // ENABLE_GAUSSIAN_BLUR

	//uninitializeCamera(&camera);

}

