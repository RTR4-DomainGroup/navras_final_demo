#pragma once

// This File Will Be Replaced by Scene*.cpp

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/waterframebuffer.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/shadowframebuffer.h"

#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/BillboardingShader.h"
#include "../../inc/shaders/AtmosphereShader.h"

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
#include "../../inc/effects/AtmosphereEffect.h"

#include "../../inc/scenes/scenePlaceHolderOutdoor.h"
#include "../../inc/scenes/scene10_Shrungar.h"


#define FBO_WIDTH 1920
#define FBO_HEIGHT 1080
//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####

#define ENABLE_TERRIAN
//#define ENABLE_ATMOSPHERE
#define ENABLE_WATER
#define ENABLE_CLOUD_NOISE
//#define ENABLE_SKYBOX
#define ENABLE_STARFIELD
//#define ENABLE_FOG
#define ENABLE_STATIC_MODELS	
#define ENABLE_BILLBOARDING
//#define ENABLE_VIDEO_RENDER
//#define ENABLE_GAUSSIAN_BLUR
#define ENABLE_GODRAYS
#define ENABLE_SHADOW

GLfloat whiteSphereShrungar[3] = {1.0f, 1.0f, 1.0f};
GLuint texture_MarbleShrungar;
TEXTURE texture_grassShrungar;
TEXTURE texture_flowerSHrungar;

struct ADSUniform sceneADSUniformShrungar;
struct FSQuadUniform fsqUniformShrungar;

struct TerrainUniform terrainUniformShrungar;

struct CloudNoiseUniform sceneCloudNoiseUniformShrungar;

struct TextureVariables terrainTextureVariablesShrungar;

// variables for billboarding
struct BillboardingUniform billboardingEffectUniformShrungar;
GLuint frameTimeShrungar = 0;

// Water Related Variables
struct WaterUniform waterUniformShrungar;
struct TextureVariables waterTextureVariablesShrungar;
struct WaterFrameBufferDetails waterReflectionFrameBufferDetailsShrungar;
struct WaterFrameBufferDetails waterRefractionFrameBufferDetailsShrungar;

// Gaussian Blur related variables
struct GaussianBlurEffect gaussianBlurEffectShrungar;
struct HorrizontalBlurUniform horizontalBlurUniformShrungar;
struct VerticalBlurUniform verticalBlurUniformShrungar;
struct FrameBufferDetails fullSceneFboShrungar;
struct FSQuadUniform fsGaussBlurQuadUniformShrungar;

// Atmosphere Scattering
AtmosphereUniform atmosphereUniformShrungar;
AtmosphericVariables atmosVariablesShrungar;

// Shadow
ShadowFrameBufferDetails shadowFramebufferShrungar;
mat4 lightSpaceMatrixShrungar;
//mat4 perspectiveProjectionDepth;

GLfloat waterHeightShrungar = 0.0f;
GLfloat moveFactorShrungar = 0.0f;
GLfloat planeReflectionShrungar[] = { 0.0f, 1.0f, 0.0f, -waterHeightShrungar };
GLfloat planeRefrationShrungar[] = { 0.0f, -1.0f, 0.0f, waterHeightShrungar };
struct FrameBufferDetails fboBlackPassShrungar;
struct FrameBufferDetails fboColorPassShrungar;
struct FrameBufferDetails fboGodRayPassShrungar;

extern int windowWidth;
extern int windowHeight;

extern mat4 viewMatrix;

float myScaleShrungar = 1.0f;

float noiseScaleShrungar = 2.0f;
bool noiseScaleIncrementShrungar = true;

GLfloat lightAmbientShrungar[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuseShrungar[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecularShrungar[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPositionShrungar[] = { 10.0f, 10.0f, 0.0f, 1.0f };

GLfloat materialAmbientShrungar[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuseShrungar[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecularShrungar[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininessShrungar = 128.0f;

GLfloat skyColorShrungar[] = { 0.0f, 0.0f, 0.8f, 0.0f };
GLfloat cloudColorShrungar[] = { 0.8f, 0.8f, 0.8f, 0.0f };

GLuint noise_textureShrungar;

GLfloat angleCubeShrungar;

extern mat4 perspectiveProjectionMatrix;

float displacementmap_depthShrungar;

// Variables For Skybox
GLuint texture_skyboxShrungar;
struct SkyboxUniform sceneSkyBoxUniformShrungar;

// Variables For Starfieldx
GLuint texture_starShrungar; 
double deltaTimeSHrungar;
struct StarfieldUniform sceneStarfieldUniformShrungar;

//Model variables
STATIC_MODEL rockModelShrungar;
STATIC_MODEL streetLightModelShrungar;

GLfloat densityFogShrungar = 0.15;
GLfloat gradientShrungar = 0.5;
GLfloat skyFogColorShrungar[] = { 0.25f, 0.25f, 0.25f, 1.0f };


// Varaiables for God Rays
struct GodraysUniform sceneGodRaysUniformShrungar;
GLfloat lightPosition_grShrungar[] = {0.0f, 10.0f, -100.0f, 1.0f};

// Camera angle for rotation
GLfloat cameraAngleShrungar = 0.0f;
GLfloat dispersalShrungar = 0.1875f;
GLfloat haloWidthShrungar = 0.45f;
GLfloat intensityShrungar = 1.5f;
GLfloat distortionShruungar[] = { 0.94f, 0.97f, 1.0f };

Camera cameraShrungar;


int initializeScene10_Shrungar(void)
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
	//initializeCamera(&cameraShrungar);

#ifdef ENABLE_VIDEO_RENDER
	initializeQuadForVideo();
    //initializeTriangle();
    //initializeSphere();
	initializeVideoEffect("res\\videos\\AMCBanner_60fps.mp4");

#else

#ifdef ENABLE_ADSLIGHT
    // Texture
	// if (LoadGLTexture(&texture_MarbleShrungar, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
	if (LoadGLTexture_UsingSOIL(&texture_MarbleShrungar, TEXTURE_DIR"marble.bmp") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_MarbleShrungar);
	}

#endif // ENABLE_ADSLIGHT

#ifdef ENABLE_SHADOW

	shadowFramebufferShrungar.textureWidth = 1024;
	shadowFramebufferShrungar.textureHeight = 1024;

	if (shadowCreateFBO(&shadowFramebufferShrungar) == FALSE) {

		LOG("shadowCreateFBO() For Shadow FAILED!!!\n");
			return(-1);

	}
	else {

		LOG("shadowCreateFBO() Successfull for Shadow!!!\n");

	}

	initializeQuad();

#endif

#ifdef ENABLE_TERRIAN
	displacementmap_depthShrungar = 15.0f;

	terrainTextureVariablesShrungar.albedoPath = TEXTURE_DIR"terrain/DiffuseMapTerrain.jpg";
	terrainTextureVariablesShrungar.displacementPath = TEXTURE_DIR"terrain/DisplacementMapTerrain.jpg";

	if (initializeTerrain(&terrainTextureVariablesShrungar) != 0) 
	{
		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}
	
#endif

#ifdef ENABLE_ATMOSPHERE

	//
	atmosVariablesShrungar.m_nSamples = 3;		// Number of sample rays to use in integral equation
	atmosVariablesShrungar.m_Kr = 0.0035f;		// Rayleigh scattering constant
	atmosVariablesShrungar.m_Kr4PI = atmosVariablesShrungar.m_Kr * 4.0f * M_PI;
	atmosVariablesShrungar.m_Km = 0.0015f;		// Mie scattering constant
	atmosVariablesShrungar.m_Km4PI = atmosVariablesShrungar.m_Km * 4.0f * M_PI;
	atmosVariablesShrungar.m_ESun = 20.0f;		// Sun brightness constant
	atmosVariablesShrungar.m_g = -0.990f;		// The Mie phase asymmetry factor
	atmosVariablesShrungar.m_fExposure = 2.0f;

	atmosVariablesShrungar.m_fInnerRadius = 10.0f;
	atmosVariablesShrungar.m_fOuterRadius = 50.0f;
	//atmosVariablesShrungar.m_fOuterRadius = atmosVariablesShrungar.m_fInnerRadius + (atmosVariablesShrungar.m_fInnerRadius * 2.5f);
	atmosVariablesShrungar.m_fScale = 1 / (atmosVariablesShrungar.m_fOuterRadius - atmosVariablesShrungar.m_fInnerRadius);

	atmosVariablesShrungar.m_fWavelength[0] = 0.650f;		// 650 nm for red
	atmosVariablesShrungar.m_fWavelength[1] = 0.570f;		// 570 nm for green
	atmosVariablesShrungar.m_fWavelength[2] = 0.475f;		// 475 nm for blue
	atmosVariablesShrungar.m_fWavelength4[0] = powf(atmosVariablesShrungar.m_fWavelength[0], 4.0f);
	atmosVariablesShrungar.m_fWavelength4[1] = powf(atmosVariablesShrungar.m_fWavelength[1], 4.0f);
	atmosVariablesShrungar.m_fWavelength4[2] = powf(atmosVariablesShrungar.m_fWavelength[2], 4.0f);

	atmosVariablesShrungar.m_fRayleighScaleDepth = 0.25f;
	atmosVariablesShrungar.m_fMieScaleDepth = 0.1f;

	atmosVariablesShrungar.m_vLight = vec3(0, 0, -350);
	atmosVariablesShrungar.m_vLightDirection = atmosVariablesShrungar.m_vLight / sqrtf(atmosVariablesShrungar.m_vLight[0] * atmosVariablesShrungar.m_vLight[0] + atmosVariablesShrungar.m_vLight[1] * atmosVariablesShrungar.m_vLight[1] + atmosVariablesShrungar.m_vLight[2] * atmosVariablesShrungar.m_vLight[2]);

	//
	initializeAtmosphere(atmosVariablesShrungar);

#endif

#ifdef ENABLE_GODRAYS
	int initializeGodRays(void);
	initializeSphere(1.0f, 60, 60);
	initializeGodRays();
	initializeQuad();
#endif // ENABLE_GODRAYS

#ifdef ENABLE_WATER

	waterTextureVariablesShrungar.displacementPath = "res/textures/water/waterDUDV.bmp";

	if (initializeWater(&waterTextureVariablesShrungar) != 0) {

		LOG("initializeWater() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("initializeWater() Successfull!!!\n");
	}

	//
	waterReflectionFrameBufferDetailsShrungar.textureWidth = 1280;
	waterReflectionFrameBufferDetailsShrungar.textureHeight = 720;

	if (waterCreateFBO(&waterReflectionFrameBufferDetailsShrungar) == FALSE) {

		LOG("waterCreateFBO() For WaterReflection FAILED!!!\n");
		return(-1);

	}
	else {

		LOG("waterCreateFBO() Successfull for WaterReflection!!!\n");

	}

	waterRefractionFrameBufferDetailsShrungar.textureWidth = 1280;
	waterRefractionFrameBufferDetailsShrungar.textureHeight = 720;

	if (waterCreateFBO(&waterRefractionFrameBufferDetailsShrungar) == FALSE) {

		LOG("waterCreateFBO() For WaterRefraction FAILED!!!\n");
		return(-1);

	}
	else {

		LOG("waterCreateFBO() Successfull for Water Refraction!!!\n");

	}

#endif

#ifdef ENABLE_SKYBOX
	if (initializeSkybox(&texture_skyboxShrungar, TEXTURE_DIR"Skybox\\") != 0)
	{

		LOG("initializeSkybox() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("initializeSkybox() Successfull!!!\n");
	}
#endif

#ifdef ENABLE_CLOUD_NOISE

	noise_textureShrungar = initializeCloud();
	if (noise_textureShrungar == 0)
	{
		LOG("initializeCloud() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeCloud() Successfull!!!\n");
	}

#endif

#ifdef ENABLE_STARFIELD
	if (initializeStarfield(&texture_starShrungar, TEXTURE_DIR"Starfield/Star.png") != 0)
	{
		LOG("initializeStarfield() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeStarfield() Successfull!!!\n");
	}
#endif // ENABLE_STARFIELD
	
#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/rock/rock.obj", &rockModelShrungar);
	loadStaticModel("res/models/streetLight/StreetLight.obj", &streetLightModelShrungar);
#endif

#ifdef ENABLE_BILLBOARDING	

    GLfloat instance_positions[NO_OF_INSTANCES * 4] = {};
    // generate positions per instance
    for(int i = 0; i < NO_OF_INSTANCES; i++)
    {
		instance_positions[(i*4)+0] = (((GLfloat)rand() / RAND_MAX) * (X_MAX - X_MIN)) + X_MIN;
		instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (Y_MAX - Y_MIN)) + Y_MIN;
		instance_positions[(i*4)+2] = (((GLfloat)rand() / RAND_MAX) * (Z_MAX - Z_MIN)) + Z_MIN;
		instance_positions[(i*4)+3] = 1.0f;
		LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
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
	if (LoadGLTextureData_UsingSOIL(&texture_grassShrungar, imagefile) == GL_FALSE)
	{
        LOG("Texture loading failed for image %s\n", imagefile);
        return (-6);
    }

	sprintf(imagefile, "%s", TEXTURE_DIR"\\billboarding\\flower.png");
	if (LoadGLTextureData_UsingSOIL(&texture_flowerSHrungar, imagefile) == GL_FALSE)
	{
        LOG("Texture loading failed for image %s\n", imagefile);
        return (-6);
    }

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_GAUSSIAN_BLUR
	initializeQuad();
	if(initializeGaussianBlur(&gaussianBlurEffectShrungar) == false)
	{
		LOG("Initialize Gaussian Blur Effect FAILED!!");
		return (-7);
	}

	fullSceneFboShrungar.textureWidth = 1920;
	fullSceneFboShrungar.textureHeight = 1080;

	if (createFBO(&fullSceneFboShrungar) == false)
	{
		LOG("Unable to create FBO for entire scene");
		return (-8);
	}
	
#endif

#endif
	return 0;
}

void displayScene10_Shrungar(void)
{
	// Function Declarations
	void displayPassesShrungar(int,bool,bool,bool, int);
	void displayGodRays(int, int);

	// Code
	// Here The Game STarts

	// set cameraa

	setCamera();
	//setCamera(&cameraShrungar);

	//rotateCamera(0.0f, 10.0f, 0.0f, 50.0f, cameraAngleShrungar);

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	viewMatrix = vmath::lookat(cameraShrungar.eye, cameraShrungar.center, cameraShrungar.up);

#ifdef ENABLE_VIDEO_RENDER
	fsqUniformShrungar = useFSQuadShader();
	displayVideoEffect(&fsqUniformShrungar);
	glUseProgram(0);
#else
		// Water Frame Buffers
		// Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionFrameBufferDetailsShrungar.frameBuffer);
		glViewport(0, 0, (GLsizei)waterReflectionFrameBufferDetailsShrungar.textureWidth, (GLsizei)waterReflectionFrameBufferDetailsShrungar.textureHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterReflectionFrameBufferDetailsShrungar.textureWidth / waterReflectionFrameBufferDetailsShrungar.textureHeight, 0.1f, 1000.0f);
		displayPassesShrungar(1, true, true, false, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Refraction
		glBindFramebuffer(GL_FRAMEBUFFER, waterRefractionFrameBufferDetailsShrungar.frameBuffer);
		glViewport(0, 0, (GLsizei)waterRefractionFrameBufferDetailsShrungar.textureWidth, (GLsizei)waterRefractionFrameBufferDetailsShrungar.textureHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterRefractionFrameBufferDetailsShrungar.textureWidth / waterRefractionFrameBufferDetailsShrungar.textureHeight, 0.1f, 1000.0f);
		displayPassesShrungar(1, true, false, false, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CLIP_DISTANCE0);

		// Shadow
		glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebufferShrungar.frameBuffer);
		glViewport(0, 0, (GLsizei)shadowFramebufferShrungar.textureWidth, (GLsizei)shadowFramebufferShrungar.textureHeight);
		glClear(GL_DEPTH_BUFFER_BIT);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)shadowFramebufferShrungar.textureWidth / shadowFramebufferShrungar.textureHeight, 0.1f, 100.0f);
		displayPassesShrungar(1, true, true, true, 1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//////////////////////////////////////////////////////////////
		/*glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight,
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsqUniformShrungar = useFSQuadShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowFramebufferShrungar.frameBufferDepthTexture);
		glUniform1i(fsqUniformShrungar.textureSamplerUniform1, 0);

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);*/

	#if !defined(ENABLE_GAUSSIAN_BLUR) && !defined(ENABLE_GODRAYS)
		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, 
			(GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPassesShrungar(1, false, false, true, 1);
	
	#elif defined(ENABLE_GAUSSIAN_BLUR)
		displayPassesShrungar(1, false, false, true, 1);
		glBindFramebuffer(GL_FRAMEBUFFER, fullSceneFboShrungar.frameBuffer);
		glViewport(0, 0, (GLsizei)fullSceneFboShrungar.textureWidth, (GLsizei)fullSceneFboShrungar.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fullSceneFboShrungar.textureWidth / fullSceneFboShrungar.textureHeight, 
		0.1f, 1000.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPassesShrungar(1, false, false, true, 1);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		displayGaussianBlur();

		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsGaussBlurQuadUniformShrungar = useFSQuadShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurEffectShrungar.verticalFBDetails.frameBufferTexture);
		glUniform1i(fsGaussBlurQuadUniformShrungar.textureSamplerUniform1, 0);
		displayQuad();
		glUseProgram(0);
    	glBindTexture(GL_TEXTURE_2D, 0);
	#else

		

		// GodRay Black pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPassShrungar.frameBuffer);
		glViewport(0, 0, (GLsizei)fboBlackPassShrungar.textureWidth, (GLsizei)fboBlackPassShrungar.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPassShrungar.textureWidth / fboBlackPassShrungar.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			displayPassesShrungar(0, false, false, true, 0);

		sceneADSUniformShrungar = useADSShader();
		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		translationMatrix = vmath::translate(lightPosition_grShrungar[0], lightPosition_grShrungar[1], lightPosition_grShrungar[2]);
		modelMatrix = translationMatrix;
		
		glUniformMatrix4fv(sceneADSUniformShrungar.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneADSUniformShrungar.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneADSUniformShrungar.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(sceneADSUniformShrungar.uniform_enable_godRays, 0);
		glUniform1i(sceneADSUniformShrungar.godrays_blackpass_sphere, 1);
		float color[3] = {1.0f, 1.0f, 1.0f};
		glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f,1.0f,1.0f));
		displaySphere(color);
		glUseProgram(0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// GodRay Color Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboColorPassShrungar.frameBuffer);
		glViewport(0, 0, (GLsizei)fboColorPassShrungar.textureWidth, (GLsizei)fboColorPassShrungar.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboColorPassShrungar.textureWidth / fboColorPassShrungar.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//displayWaterFramebuffers(1);
			displayPassesShrungar(1, false, false, true, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// God Rays Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboGodRayPassShrungar.frameBuffer);
		glViewport(0, 0, (GLsizei)fboGodRayPassShrungar.textureWidth, (GLsizei)fboGodRayPassShrungar.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboGodRayPassShrungar.textureWidth / fboGodRayPassShrungar.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sceneGodRaysUniformShrungar = useGodRaysShader();

		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		//translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix;

		glUniform4fv(sceneGodRaysUniformShrungar.lightPositionOnScreen, 1, lightPosition_grShrungar);
		glUniform1f(sceneGodRaysUniformShrungar.decay, 1.0f);
		glUniform1f(sceneGodRaysUniformShrungar.density, 0.92f);
		glUniform1f(sceneGodRaysUniformShrungar.exposure, 0.25f);
		glUniform1f(sceneGodRaysUniformShrungar.weight, 0.04f);

		glUniformMatrix4fv(sceneGodRaysUniformShrungar.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneGodRaysUniformShrungar.viewMatrix, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneGodRaysUniformShrungar.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(sceneGodRaysUniformShrungar.godrays_lfEnabled, 1);

		glUniform1f(sceneGodRaysUniformShrungar.dispersalUniform, dispersalShrungar);
		glUniform1f(sceneGodRaysUniformShrungar.haloWidthUniform, haloWidthShrungar);
		glUniform1f(sceneGodRaysUniformShrungar.intensityUniform, intensityShrungar);
		glUniform3fv(sceneGodRaysUniformShrungar.distortionUniform, 1, distortionShruungar);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboBlackPassShrungar.frameBufferTexture);
		glUniform1i(sceneGodRaysUniformShrungar.godraysampler, 0);
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
		fsqUniformShrungar = useFSQuadShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboGodRayPassShrungar.frameBufferTexture);
		glUniform1i(fsqUniformShrungar.textureSamplerUniform1, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fboColorPassShrungar.frameBufferTexture);
		glUniform1i(fsqUniformShrungar.textureSamplerUniform2, 1);
		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

	#endif

#endif
}

void displayPassesShrungar(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0) {


	// Code
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	mat4 rotateX = mat4::identity();

	viewMatrix = vmath::lookat(cameraShrungar.eye, cameraShrungar.center, cameraShrungar.up);
	setCamera();
	//setCamera(&cameraShrungar);

	mat4 finalViewMatrix = mat4::identity();

	if (actualDepthQuadScene == 0) {
	
		finalViewMatrix = viewMatrix;

	}else if(actualDepthQuadScene == 1){
	
		finalViewMatrix = viewMatrix;

		finalViewMatrix = mat4::identity();
		finalViewMatrix = lookat(vec3(lightPositionShrungar[0], lightPositionShrungar[1], lightPositionShrungar[2]), vec3(0.0f, -5.0f, -20.0f), vec3(0.0f, 1.0f, 0.0f));
		/*perspectiveProjectionDepth = mat4::identity();
		perspectiveProjectionDepth = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 100.0f);*/

		lightSpaceMatrixShrungar = mat4::identity();
		lightSpaceMatrixShrungar = perspectiveProjectionMatrix * finalViewMatrix;
	
	}


	if (recordWaterReflectionRefraction == true) {
		waterUniformShrungar = useWaterShader();

		float distance = 2.0f * (cameraEyeY - waterHeightShrungar);
		if (isReflection == true) {
			glUniform4fv(waterUniformShrungar.planeUniform, 1, planeReflectionShrungar);
			cameraEyeY -= distance;
			setCamera();
			//setCamera(&cameraShrungar);
			glUniformMatrix4fv(waterUniformShrungar.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
			cameraEyeY += distance;
			setCamera();
			//setCamera(&cameraShrungar);
		}

		if (isReflection == false) {
			glUniform4fv(waterUniformShrungar.planeUniform, 1, planeRefrationShrungar);
			glUniformMatrix4fv(waterUniformShrungar.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		}
	}

	if (actualDepthQuadScene == 0) {

		if (godRays == 1) {

#ifdef ENABLE_ATMOSPHERE

			translationMatrix = mat4::identity();
			rotationMatrix = mat4::identity();
			modelMatrix = mat4::identity();

			//glBlendFunc(GL_ONE, GL_ONE);

			atmosphereUniformShrungar = useAtmosphereShader();

			glUniform3f(atmosphereUniformShrungar.cameraPosUniform, cameraEyeX, cameraEyeY, cameraEyeZ);
			glUniform3f(atmosphereUniformShrungar.lightPosUniform, atmosVariablesShrungar.m_vLightDirection[0], atmosVariablesShrungar.m_vLightDirection[1], atmosVariablesShrungar.m_vLightDirection[2]);
			glUniform3f(atmosphereUniformShrungar.invWavelengthUniform, 1 / atmosVariablesShrungar.m_fWavelength4[0], 1 / atmosVariablesShrungar.m_fWavelength4[1], 1 / atmosVariablesShrungar.m_fWavelength4[2]);
			glUniform1f(atmosphereUniformShrungar.cameraHeightUniform, sqrtf(cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ));
			glUniform1f(atmosphereUniformShrungar.cameraHeight2Uniform, cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ);
			glUniform1f(atmosphereUniformShrungar.innerRadiusUniform, atmosVariablesShrungar.m_fInnerRadius);
			glUniform1f(atmosphereUniformShrungar.innerRadius2Uniform, atmosVariablesShrungar.m_fInnerRadius * atmosVariablesShrungar.m_fInnerRadius);
			glUniform1f(atmosphereUniformShrungar.outerRadiusUniform, atmosVariablesShrungar.m_fOuterRadius);
			glUniform1f(atmosphereUniformShrungar.outerRadius2Uniform, atmosVariablesShrungar.m_fOuterRadius * atmosVariablesShrungar.m_fOuterRadius);
			glUniform1f(atmosphereUniformShrungar.KrESunUniform, atmosVariablesShrungar.m_Kr * atmosVariablesShrungar.m_ESun);
			glUniform1f(atmosphereUniformShrungar.KmESunUniform, atmosVariablesShrungar.m_Km * atmosVariablesShrungar.m_ESun);
			glUniform1f(atmosphereUniformShrungar.Kr4PIUniform, atmosVariablesShrungar.m_Kr4PI);
			glUniform1f(atmosphereUniformShrungar.Km4PIUniform, atmosVariablesShrungar.m_Km4PI);
			glUniform1f(atmosphereUniformShrungar.scaleUniform, 1.0f / (atmosVariablesShrungar.m_fOuterRadius - atmosVariablesShrungar.m_fInnerRadius));
			glUniform1f(atmosphereUniformShrungar.scaleDepthUniform, atmosVariablesShrungar.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniformShrungar.scaleOverScaleDepthUniform, (1.0f / (atmosVariablesShrungar.m_fOuterRadius - atmosVariablesShrungar.m_fInnerRadius)) / atmosVariablesShrungar.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniformShrungar.gUniform, atmosVariablesShrungar.m_g);
			glUniform1f(atmosphereUniformShrungar.g2Uniform, atmosVariablesShrungar.m_g * atmosVariablesShrungar.m_g);

			glUniformMatrix4fv(atmosphereUniformShrungar.modelMatrix, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(atmosphereUniformShrungar.viewMatrix, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(atmosphereUniformShrungar.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

			displayAtmosphere();

			glUseProgram(0);

#endif

		}



#ifdef ENABLE_CLOUD_NOISE

		glEnable(GL_TEXTURE_3D);
		sceneCloudNoiseUniformShrungar = useCloudNoiseShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		rotateX = mat4::identity();

		//translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
		translationMatrix = vmath::translate(0.0f, 0.0f, -500.0f); // glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
		scaleMatrix = vmath::scale(800.0f, 450.0f, 1.0f);
		//rotateX = vmath::rotate(10.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotateX;

		//viewMatrix = vmath::lookat(cameraShrungar.eye, cameraShrungar.eye, cameraShrungar.up);

		glUniformMatrix4fv(sceneCloudNoiseUniformShrungar.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneCloudNoiseUniformShrungar.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneCloudNoiseUniformShrungar.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform3fv(sceneCloudNoiseUniformShrungar.laUniform, 1, lightAmbientShrungar);
		glUniform3fv(sceneCloudNoiseUniformShrungar.ldUniform, 1, lightDiffuseShrungar);
		glUniform3fv(sceneCloudNoiseUniformShrungar.lsUniform, 1, lightSpecularShrungar);
		glUniform4fv(sceneCloudNoiseUniformShrungar.lightPositionUniform, 1, lightPositionShrungar);

		glUniform3fv(sceneCloudNoiseUniformShrungar.kaUniform, 1, materialAmbientShrungar);
		glUniform3fv(sceneCloudNoiseUniformShrungar.kdUniform, 1, materialDiffuseShrungar);
		glUniform3fv(sceneCloudNoiseUniformShrungar.ksUniform, 1, materialSpecularShrungar);
		glUniform1f(sceneCloudNoiseUniformShrungar.materialShininessUniform, materialShininessShrungar);

		glUniform1f(sceneCloudNoiseUniformShrungar.scaleUniform, myScaleShrungar);
		glUniform3fv(sceneCloudNoiseUniformShrungar.skyColorUniform, 1, skyColorShrungar);
		glUniform3fv(sceneCloudNoiseUniformShrungar.cloudColorUniform, 1, cloudColorShrungar);
		glUniform1f(sceneCloudNoiseUniformShrungar.noiseScaleUniform, noiseScaleShrungar);
		glUniform1i(sceneCloudNoiseUniformShrungar.uniform_enable_godRays, godRays);
		//glUniform1f(sceneCloudNoiseUniformShrungar.alphaBlendingUniform, alphaBlending);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noise_textureShrungar);

		displayQuad();

		glUseProgram(0);

		glDisable(GL_TEXTURE_3D);

#endif

#ifdef ENABLE_STARFIELD

		sceneStarfieldUniformShrungar = useStarfieldShader();

		float time = (float)deltaTimeSHrungar;

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

		glUniformMatrix4fv(sceneStarfieldUniformShrungar.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneStarfieldUniformShrungar.viewMatrix, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneStarfieldUniformShrungar.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1i(sceneStarfieldUniformShrungar.textureSamplerUniform, 0);
		glUniform1i(sceneStarfieldUniformShrungar.uniform_enable_godRays, godRays);
		glUniform1f(sceneStarfieldUniformShrungar.timeUniform, time);

		displayStarfield(texture_starShrungar);
		glUseProgram(0);

#endif // ENABLE_STARFIELD

#ifdef ENABLE_SKYBOX

		sceneSkyBoxUniformShrungar = useSkyboxShader();

		// Transformations
		translationMatrix = vmath::translate(0.0f, 0.0f, -10.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(30.0f, 30.0f, 30.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(sceneSkyBoxUniformShrungar.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneSkyBoxUniformShrungar.viewMatrix, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneSkyBoxUniformShrungar.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

		displaySkybox(texture_skyboxShrungar);
		glUseProgram(0);
#endif

	}

#ifdef ENABLE_TERRIAN
	// Terrain
	terrainUniformShrungar = useTerrainShader();

	vmath::mat4 mv_matrix = mat4::identity();
	vmath::mat4 proj_matrix = mat4::identity();


	mv_matrix = finalViewMatrix * (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));

	proj_matrix = perspectiveProjectionMatrix;


	glUniformMatrix4fv(terrainUniformShrungar.uniform_mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(terrainUniformShrungar.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniformShrungar.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);

	glUniform1f(terrainUniformShrungar.uniform_dmap_depth, displacementmap_depthShrungar);
	//glUniform1i(terrainUniformShrungar.uniform_enable_fog, enable_fog ? 1 : 0);
	//glUniform1i(terrainUniformShrungar.uniform_enable_fog, 0);
	glUniform1i(terrainUniformShrungar.uniform_enable_godRays, godRays);

#ifdef ENABLE_FOG
	glUniform1i(terrainUniformShrungar.fogEnableUniform, 1);
	glUniform1f(terrainUniformShrungar.densityUniform, densityFogShrungar);
	glUniform1f(terrainUniformShrungar.gradientUniform, gradientShrungar);
	glUniform4fv(terrainUniformShrungar.skyFogColorUniform, 1, skyFogColorShrungar);
#endif // DEBUG

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariablesShrungar.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariablesShrungar.albedo);
	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
#endif


#ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneADSUniformShrungar = useADSShader();

	// Sending Light Related Uniforms
	glUniform1i(sceneADSUniformShrungar.lightingEnableUniform, 1);
	glUniform4fv(sceneADSUniformShrungar.laUniform, 1, lightAmbientShrungar);
	glUniform4fv(sceneADSUniformShrungar.ldUniform, 1, lightDiffuseShrungar);
	glUniform4fv(sceneADSUniformShrungar.lsUniform, 1, lightSpecularShrungar);
	glUniform4fv(sceneADSUniformShrungar.lightPositionUniform, 1, lightPositionShrungar);
	glUniform4fv(sceneADSUniformShrungar.kaUniform, 1, materialAmbientShrungar);
	glUniform4fv(sceneADSUniformShrungar.kdUniform, 1, materialDiffuseShrungar);
	glUniform4fv(sceneADSUniformShrungar.ksUniform, 1, materialSpecularShrungar);
	glUniform1f(sceneADSUniformShrungar.materialShininessUniform, materialShininessShrungar);

	glUniform1i(sceneADSUniformShrungar.fogEnableUniform, 1);
	glUniform1f(sceneADSUniformShrungar.densityUniform, densityFogShrungar);
	glUniform1f(sceneADSUniformShrungar.gradientUniform, gradientShrungar);
	glUniform4fv(sceneADSUniformShrungar.skyFogColorUniform, 1, skyFogColorShrungar);
	glUniform1i(sceneADSUniformShrungar.uniform_enable_godRays, godRays);
	glUniform1i(sceneADSUniformShrungar.godrays_blackpass_sphere, 0);

	//glUniform1i(sceneADSUniformShrungar.)
	// ------ Rock Model ------
	translationMatrix = vmath::translate(-1.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneADSUniformShrungar.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	if (actualDepthQuadScene == 1) {

		glUniform1i(sceneADSUniformShrungar.actualSceneUniform, 0);
		glUniform1i(sceneADSUniformShrungar.depthSceneUniform, 1);
		glUniform1i(sceneADSUniformShrungar.depthQuadSceneUniform, 0);
		glUniformMatrix4fv(sceneADSUniformShrungar.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrixShrungar);

	}
	else {

		glUniform1i(sceneADSUniformShrungar.actualSceneUniform, 1);
		glUniform1i(sceneADSUniformShrungar.depthSceneUniform, 0);
		glUniform1i(sceneADSUniformShrungar.depthQuadSceneUniform, 0);

		glActiveTexture(GL_TEXTURE6);
		glUniform1i(sceneADSUniformShrungar.shadowMapSamplerUniform, 6);
		glBindTexture(GL_TEXTURE_2D, shadowFramebufferShrungar.frameBufferDepthTexture);

	}

	glUniformMatrix4fv(sceneADSUniformShrungar.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneADSUniformShrungar.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);


	drawStaticModel(rockModelShrungar);

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	// ------ Streetlight Model ------
	translationMatrix = vmath::translate(1.0f, -2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneADSUniformShrungar.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneADSUniformShrungar.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneADSUniformShrungar.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);


	drawStaticModel(streetLightModelShrungar);

	if (actualDepthQuadScene == 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Un-use ShaderProgramObject
	glUseProgram(0);
#endif

if(waterDraw == true){
	#ifdef ENABLE_WATER
		waterUniformShrungar = useWaterShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.0f, 0.0f, -20.0f);

		scaleMatrix = vmath::scale(40.0f, 1.0f, 40.0f);

		modelMatrix = translationMatrix * scaleMatrix;

		glUniformMatrix4fv(waterUniformShrungar.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(waterUniformShrungar.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(waterUniformShrungar.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterReflectionFrameBufferDetailsShrungar.frameBufferTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterRefractionFrameBufferDetailsShrungar.frameBufferTexture);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterTextureVariablesShrungar.displacement);

		glUniform1f(waterUniformShrungar.moveFactorUniform, moveFactorShrungar);

		glUniform1f(waterUniformShrungar.uniform_enable_godRays, godRays);

		displayWater();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

	#endif
}

if (actualDepthQuadScene == 0) {


#ifdef ENABLE_BILLBOARDING	
	// variable declaration
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	// code
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	billboardingEffectUniformShrungar = useBillboardingShader();

	// instanced quads with grass texture
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniformShrungar.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(billboardingEffectUniformShrungar.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	/// Grass
	if (texture_grassShrungar.height > texture_grassShrungar.width)
		scaleMatrix = vmath::scale(texture_grassShrungar.width / (GLfloat)texture_grassShrungar.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_grassShrungar.height / (GLfloat)texture_grassShrungar.width, 1.0f);

	translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(billboardingEffectUniformShrungar.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniform1i(billboardingEffectUniformShrungar.textureSamplerUniform, 0);
	glUniform1i(billboardingEffectUniformShrungar.billboardingEnableUniform, 1);
	glUniform1i(billboardingEffectUniformShrungar.frameTimeUniform, frameTimeShrungar);
	glUniform1i(billboardingEffectUniformShrungar.uniform_enable_godRays, godRays);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_grassShrungar.id);
	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw
	glBindTexture(GL_TEXTURE_2D, 0);


	/// Flower
	if (texture_flowerSHrungar.height > texture_flowerSHrungar.width)
		scaleMatrix = vmath::scale(texture_flowerSHrungar.width / (GLfloat)texture_flowerSHrungar.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_flowerSHrungar.height / (GLfloat)texture_flowerSHrungar.width, 1.0f);

	translationMatrix = vmath::translate(-1.5f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniformShrungar.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_flowerSHrungar.id);
	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
	glDisable(GL_BLEND);

#endif // ENABLE_BILLBOARDING

}
	
}

//int initializeGodRays(void)
//{
//	fboBlackPassShrungar.textureWidth = FBO_WIDTH;
//	fboBlackPassShrungar.textureHeight = FBO_HEIGHT;
//
//	fboColorPassShrungar.textureWidth = FBO_WIDTH;
//	fboColorPassShrungar.textureHeight = FBO_HEIGHT;
//
//	fboGodRayPassShrungar.textureWidth = FBO_WIDTH;
//	fboGodRayPassShrungar.textureHeight = FBO_HEIGHT;
//
//	createFBO(&fboBlackPassShrungar);
//	createFBO(&fboColorPassShrungar);
//	createFBO(&fboGodRayPassShrungar);
//
//	return(0);
//}

//void resize_godRayPasses(int fboWidh, int fboHeight)
//{
//	glViewport(0, 0, (GLsizei)fboWidh, (GLsizei)fboHeight);
//}

//void displayGaussianBlur(void)
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurEffectShrungar.horrizontalFBDetails.frameBuffer);
//	glViewport(0, 0, (GLsizei)gaussianBlurEffectShrungar.horrizontalFBDetails.textureWidth, 
//	(GLsizei)gaussianBlurEffectShrungar.horrizontalFBDetails.textureHeight);
//
//    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	
//    horizontalBlurUniformShrungar = useHorrizontalBlurShader();
//
//    glUniform1f(horizontalBlurUniformShrungar.targetWidth, 960.0f);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, fullSceneFboShrungar.frameBufferTexture);
//    glUniform1i(horizontalBlurUniformShrungar.hblurTexSamplerUniform, 0);
//	displayQuad();    
//    glUseProgram(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurEffectShrungar.verticalFBDetails.frameBuffer);
//	glViewport(0, 0, (GLsizei)gaussianBlurEffectShrungar.verticalFBDetails.textureWidth, 
//	(GLsizei)gaussianBlurEffectShrungar.verticalFBDetails.textureHeight);
//	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	verticalBlurUniformShrungar = useVerticalBlurShader();
//	glUniform1f(verticalBlurUniformShrungar.targetHeight, 540.0f);
//	glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, gaussianBlurEffectShrungar.horrizontalFBDetails.frameBufferTexture);
//    glUniform1i(verticalBlurUniformShrungar.vblurTexSamplerUniform, 0);
//	displayQuad();
//	glUseProgram(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

void updateScene10_Shrungar(void)
{
	// Code
#ifdef ENABLE_ADSLIGHT
    angleCubeShrungar = angleCubeShrungar + 1.0f;
	if (angleCubeShrungar >= 360.0f)
	{
		angleCubeShrungar -= 360.0f;
	}

#endif // ENABLE_ADSLIGHT

#ifdef ENABLE_STARFIELD
	deltaTimeSHrungar = updateStarfield(deltaTimeSHrungar);
#endif

#ifdef ENABLE_CLOUD_NOISE
	// update Cloud
	updateCloud(noiseScaleIncrementShrungar, noiseScaleShrungar, 0.0001f);
#endif

#ifdef ENABLE_BILLBOARDING
	frameTimeShrungar += 1;

#endif

#ifdef ENABLE_WATER

	moveFactorShrungar += 0.0003f;
	if (moveFactorShrungar >= 360.0f)
		moveFactorShrungar -= 360.0f;
#endif

	// update cameraShrungar using lerp
	//cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	//cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);
	cameraAngleShrungar = cameraAngleShrungar + 0.5f;
	if (cameraAngleShrungar >= 360.0f)
		cameraAngleShrungar -= 360.0f;
}

void uninitializeScene10_Shrungar(void)
{
	// Code
#ifdef ENABLE_BILLBOARDING
    uninitializeInstancedQuads();

	// texture
    if(texture_flowerSHrungar.id)
    {
        glDeleteTextures(1, &texture_flowerSHrungar.id);
        texture_flowerSHrungar.id = 0;
    }
    if(texture_grassShrungar.id)
    {
        glDeleteTextures(1, &texture_grassShrungar.id);
        texture_grassShrungar.id = 0;
    }
#endif

#ifdef ENABLE_WATER
	uninitializeWater(&waterTextureVariablesShrungar);
#endif

#ifdef ENABLE_STARFIELD
	uninitializeStarfield(texture_starShrungar);
#endif // ENABLE_STARFIELD

#ifdef ENABLE_SKYBOX
	uninitialiseSkybox(texture_skyboxShrungar);
#endif // ENABLE_SKYBOX

#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariablesShrungar);
#endif

#ifdef ENABLE_ATMOSPHERE
	uninitializeAtmosphere();
#endif

#ifdef ENABLE_CLOUD_NOISE
	
	uninitializeCloud();
	if (noise_textureShrungar)
	{
		glDeleteTextures(1, &noise_textureShrungar);
		noise_textureShrungar = 0;
	}
#endif

#ifdef ENABLE_ADSLIGHT
	if (texture_MarbleShrungar)
	{
		glDeleteTextures(1, &texture_MarbleShrungar);
		texture_MarbleShrungar = NULL;
	}

	// uninitializeTerrain();
	// uninitializeSphere();
	// uninitializeTriangle();
	// uninitializeQuad();
	// uninitializePyramid();
	uninitializeCube();

#endif // ENABLE_ADSLIGHT

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&rockModelShrungar);
	unloadStaticModel(&streetLightModelShrungar);
#endif

#ifdef ENABLE_GAUSSIAN_BLUR
	uninitializeGaussianBlur(&gaussianBlurEffectShrungar);
#endif

	//uninitializeCamera(&cameraShrungar);

}
