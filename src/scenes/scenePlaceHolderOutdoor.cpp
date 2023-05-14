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
#include "../../inc/shaders/ADSLightDynamicShader.h"
#include "../../inc/shaders/BillboardingShader.h"

#include "../../inc/effects/videoEffect.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/CloudEffect.h"
#include "../../inc/effects/WaterEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
// #include "../../inc/effects/Billboarding.h"
#include "../../inc/effects/GaussianBlurEffect.h"

#include "../../inc/scenes/scenePlaceHolderOutdoor.h"


#define FBO_WIDTH 1920
#define FBO_HEIGHT 1080
//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####

//#define ENABLE_TERRIAN
//#define ENABLE_WATER
//#define ENABLE_CLOUD_NOISE
//#define ENABLE_SKYBOX
//#define ENABLE_STARFIELD
//#define ENABLE_FOG
//#define ENABLE_STATIC_MODELS	
#define ENABLE_DYNAMIC_MODELS	
//#define ENABLE_BILLBOARDING
//#define ENABLE_VIDEO_RENDER
#define ENABLE_GAUSSIAN_BLUR
//#define ENABLE_GODRAYS

GLfloat whiteSphere[3] = {1.0f, 1.0f, 1.0f};
GLuint texture_Marble;
TEXTURE texture_grass;
TEXTURE texture_flower;

struct ADSDynamicUniform sceneADSDynamicUniform;
struct ADSUniform sceneOutdoorADSUniform;
struct FSQuadUniform fsqUniform;

struct TerrainUniform terrainUniform;

struct CloudNoiseUniform sceneCloudNoiseUniform;

struct TextureVariables terrainTextureVariables;

// variables for billboarding
struct BillboardingUniform billboardingEffectUniform;
GLuint frameTime = 0;

// Water Related Variables
struct WaterUniform waterUniform;
struct TextureVariables waterTextureVariables;
struct WaterFrameBufferDetails waterReflectionFrameBufferDetails;
struct WaterFrameBufferDetails waterRefractionFrameBufferDetails;

// Gaussian Blur related variables
struct GaussianBlurEffect gaussianBlurEffect;
struct HorrizontalBlurUniform horizontalBlurUniform;
struct VerticalBlurUniform verticalBlurUniform;
struct FrameBufferDetails fullSceneFbo;
struct FSQuadUniform fsGaussBlurQuadUniform;

GLfloat waterHeight = 0.0f;
GLfloat moveFactor = 0.0f;
GLfloat planeReflection[] = { 0.0f, 1.0f, 0.0f, -waterHeight };
GLfloat planeRefration[] = { 0.0f, -1.0f, 0.0f, waterHeight };
struct FrameBufferDetails fboBlackPass;
struct FrameBufferDetails fboColorPass;
struct FrameBufferDetails fboGodRayPass;

extern int windowWidth;
extern int windowHeight;

mat4 viewMatrix;

float myScale = 1.0f;

float noiseScale = 2.0f;
bool noiseScaleIncrement = true;

GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;

GLfloat skyColor[] = { 0.0f, 0.0f, 0.8f, 0.0f };
GLfloat cloudColor[] = { 0.8f, 0.8f, 0.8f, 0.0f };

GLuint noise_texture;

GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;
extern glm::mat4 glm_perspectiveProjectionMatrix;;

float displacementmap_depth;

// Variables For Skybox
GLuint texture_skybox;
struct SkyboxUniform sceneSkyBoxUniform;

// Variables For Starfieldx
GLuint texture_star; 
double deltaTime;
struct StarfieldUniform sceneStarfieldUniform;

//Model variables
STATIC_MODEL rockModel;
STATIC_MODEL streetLightModel;

DYNAMIC_MODEL dancingVampire;

GLfloat density = 0.15;
GLfloat gradient = 0.5;
GLfloat skyFogColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };


// Varaiables for God Rays
struct GodraysUniform sceneGodRaysUniform;
GLfloat lightPosition_gr[] = {0.0f, 10.0f, -100.0f, 1.0f};


int initializeScene_PlaceHolderOutdoor(void)
{
	// Function Declarations

    // Code.
#ifdef ENABLE_VIDEO_RENDER
	initializeQuadForVideo();
    //initializeTriangle();
    //initializeSphere();
	initializeVideoEffect("res\\videos\\AMCBanner_60fps.mp4");

#else

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

#ifdef ENABLE_TERRIAN
	displacementmap_depth = 15.0f;

	terrainTextureVariables.albedoPath = TEXTURE_DIR"terrain/DiffuseMapTerrain.jpg";
	terrainTextureVariables.displacementPath = TEXTURE_DIR"terrain/DisplacementMapTerrain.jpg";

	if (initializeTerrain(&terrainTextureVariables) != 0) 
	{
		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}
	
#endif

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

#endif

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
#endif

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

#endif

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
	
#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/rock/rock.obj", &rockModel);
	loadStaticModel("res/models/streetLight/StreetLight.obj", &streetLightModel);
#endif

#ifdef ENABLE_DYNAMIC_MODELS
	loadDynamicModel("res/models/dancingVampire/dancing_vampire.dae", &dancingVampire);
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
	
#endif

#endif
	return 0;
}

void displayScene_PlaceHolderOutdoor(void)
{
	// Function Declarations
	void displayPasses(int,bool,bool,bool);
	void displayGodRays(int, int);

	// Code
	// Here The Game STarts

	// set camera
	setCamera();

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);

#ifdef ENABLE_VIDEO_RENDER
	fsqUniform = useFSQuadShader();
	displayVideoEffect(&fsqUniform);
	glUseProgram(0);
#else
		// Water Frame Buffers
		// Reflection
		glEnable(GL_CLIP_DISTANCE0);
		glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionFrameBufferDetails.frameBuffer);
		glViewport(0, 0, (GLsizei)waterReflectionFrameBufferDetails.textureWidth, (GLsizei)waterReflectionFrameBufferDetails.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterReflectionFrameBufferDetails.textureWidth / waterReflectionFrameBufferDetails.textureHeight, 0.1f, 1000.0f);
		displayPasses(1, true, true, false);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Refraction
		glBindFramebuffer(GL_FRAMEBUFFER, waterRefractionFrameBufferDetails.frameBuffer);
		glViewport(0, 0, (GLsizei)waterRefractionFrameBufferDetails.textureWidth, (GLsizei)waterRefractionFrameBufferDetails.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterRefractionFrameBufferDetails.textureWidth / waterRefractionFrameBufferDetails.textureHeight, 0.1f, 1000.0f);
		displayPasses(1, true, false, false);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_CLIP_DISTANCE0);
	#if !defined(ENABLE_GAUSSIAN_BLUR) && !defined(ENABLE_GODRAYS)
		//2 framebuffers for water effect
		displayWaterFramebuffers(1);
		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, 
			(GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(1, false, false, true);
	
	#elif defined(ENABLE_GAUSSIAN_BLUR)
		displayPasses(1, false, false, true);
		glBindFramebuffer(GL_FRAMEBUFFER, fullSceneFbo.frameBuffer);
		glViewport(0, 0, (GLsizei)fullSceneFbo.textureWidth, (GLsizei)fullSceneFbo.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fullSceneFbo.textureWidth / fullSceneFbo.textureHeight, 
		0.1f, 1000.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(1, false, false, true);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		displayGaussianBlur();

		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsGaussBlurQuadUniform = useFSQuadShader();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurEffect.verticalFBDetails.frameBufferTexture);
		glUniform1i(fsGaussBlurQuadUniform.textureSamplerUniform1, 0);
		displayQuad();
		glUseProgram(0);
    	glBindTexture(GL_TEXTURE_2D, 0);
	#else

		

		// GodRay Black pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboBlackPass.textureWidth, (GLsizei)fboBlackPass.textureHeight);
			perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPass.textureWidth / fboBlackPass.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//2 framebuffers for water effect
		//displayWaterFramebuffers(0);
			displayPasses(0, false, false, true);

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
		
		displaySphere(NULL);
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
			displayPasses(1, false, false, true);
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
	#endif

#endif
}

void displayPasses(int godRays = 1, bool recordWaterReflectionRefraction = false ,bool isReflection = false, bool waterDraw = false) {
	
	
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

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	setCamera();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (recordWaterReflectionRefraction == true) {
		waterUniform = useWaterShader();

			float distance = 2.0f * (cameraEyeY - waterHeight);
			if(isReflection == true){
				glUniform4fv(waterUniform.planeUniform, 1, planeReflection);
				cameraEyeY -= distance;
				setCamera();
				glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
				cameraEyeY += distance;
				setCamera();
			}

			if (isReflection == false) {
				glUniform4fv(waterUniform.planeUniform, 1, planeRefration);
				glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
			}
	}

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

#endif // ENABLE_STARFIELD

#ifdef ENABLE_ADSLIGHT
    sceneADSUniform = useADSShader();
	glUniformMatrix4fv(sceneADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_Marble);
	
	// Sending texture Related Uniforms
    glUniform1i(sceneADSUniform.textureSamplerUniform, 0);

	// Sending Light Related Uniforms
	glUniform1i(sceneADSUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneADSUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneADSUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneADSUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneADSUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneADSUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneADSUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneADSUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneADSUniform.materialShininessUniform, materialShininess);
	glUniform1i(sceneADSUniform.uniform_enable_godRays, godRays);

	//glUniform1i(sceneADSUniform.fogEnableUniform, 1);
	//glUniform1f(sceneADSUniform.densityUniform, density);
	//glUniform1f(sceneADSUniform.gradientUniform, gradient);
	//glUniform4fv(sceneADSUniform.skyFogColorUniform, 1, skyFogColor);

	// Call Geometry over here 
	displayCube();
	// displayTriangle();
	// displayQuad();
	// displayPyramid();
	// displaySphere();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	
#endif
	
#ifdef ENABLE_CLOUD_NOISE

	glEnable(GL_TEXTURE_3D);
	sceneCloudNoiseUniform = useCloudNoiseShader();

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

	//viewMatrix = vmath::lookat(camera.eye, camera.eye, camera.up);

	glUniformMatrix4fv(sceneCloudNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneCloudNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneCloudNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform3fv(sceneCloudNoiseUniform.laUniform, 1, lightAmbient);
	glUniform3fv(sceneCloudNoiseUniform.ldUniform, 1, lightDiffuse);
	glUniform3fv(sceneCloudNoiseUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneCloudNoiseUniform.lightPositionUniform, 1, lightPosition);

	glUniform3fv(sceneCloudNoiseUniform.kaUniform, 1, materialAmbient);
	glUniform3fv(sceneCloudNoiseUniform.kdUniform, 1, materialDiffuse);
	glUniform3fv(sceneCloudNoiseUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneCloudNoiseUniform.materialShininessUniform, materialShininess);

	glUniform1f(sceneCloudNoiseUniform.scaleUniform, myScale);
	glUniform3fv(sceneCloudNoiseUniform.skyColorUniform, 1, skyColor);
	glUniform3fv(sceneCloudNoiseUniform.cloudColorUniform, 1, cloudColor);
	glUniform1f(sceneCloudNoiseUniform.noiseScaleUniform, noiseScale);
	glUniform1i(sceneCloudNoiseUniform.uniform_enable_godRays, godRays);
	//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noise_texture);

	displayQuad();

	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);

#endif

#ifdef ENABLE_TERRIAN
	// Terrain
	displayTerraineScene(godRays);
	if (godRays == 0)
	{
		sceneADSUniform = useADSShader();
		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		translationMatrix = vmath::translate(0.0f, 10.0f, -35.0f);
		modelMatrix = translationMatrix;
		glUniformMatrix4fv(sceneADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(sceneADSUniform.lightingEnableUniform, 0);
		glUniform1i(sceneADSUniform.uniform_enable_godRays, 0);
		glUniform1i(sceneADSUniform.godrays_blackpass_sphere, 1);
		
		displaySphere(NULL);
		glUseProgram(0);
	}
	
#endif

#ifdef ENABLE_SKYBOX

	sceneSkyBoxUniform = useSkyboxShader();

	// Transformations
	translationMatrix = vmath::translate(0.0f, 0.0f, -10.0f);					// glTranslatef() is replaced by this line.
	scaleMatrix = vmath::scale(30.0f, 30.0f, 30.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneSkyBoxUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneSkyBoxUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneSkyBoxUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

	displaySkybox(texture_skybox);
	glUseProgram(0);
#endif

#ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneOutdoorADSUniform = useADSShader();

	// Sending Light Related Uniforms
	glUniform1i(sceneADSUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneADSUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneADSUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneADSUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneADSUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneADSUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneADSUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneADSUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneADSUniform.materialShininessUniform, materialShininess);

	glUniform1i(sceneOutdoorADSUniform.fogEnableUniform, 1);
	glUniform1f(sceneOutdoorADSUniform.densityUniform, density);
	glUniform1f(sceneOutdoorADSUniform.gradientUniform, gradient);
	glUniform4fv(sceneOutdoorADSUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneOutdoorADSUniform.uniform_enable_godRays, godRays);
	glUniform1i(sceneOutdoorADSUniform.godrays_blackpass_sphere, 0);
	//glUniform1i(sceneOutdoorADSUniform.)
	// ------ Rock Model ------
	translationMatrix = vmath::translate(-1.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneOutdoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(rockModel);

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

	glUniformMatrix4fv(sceneOutdoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(streetLightModel);

	// Un-use ShaderProgramObject
	glUseProgram(0);
#endif

#ifdef ENABLE_WATER
	waterUniform = useWaterShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.0f, 0.0f, -20.0f);

		scaleMatrix = vmath::scale(40.0f, 1.0f, 40.0f);

		modelMatrix = translationMatrix * scaleMatrix;

		glUniformMatrix4fv(waterUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(waterUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterReflectionFrameBufferDetails.frameBufferTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterRefractionFrameBufferDetails.frameBufferTexture);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, waterTextureVariables.displacement);

		glUniform1f(waterUniform.moveFactorUniform, moveFactor);

		glUniform1f(waterUniform.uniform_enable_godRays, godRays);

		displayWater();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

	#endif
}

#ifdef ENABLE_BILLBOARDING	
	// variable declaration
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

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

#endif // ENABLE_BILLBOARDING
	
}

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
#endif

#ifdef ENABLE_CLOUD_NOISE
	// update Cloud
	updateCloud(noiseScaleIncrement, noiseScale, 0.0001f);
#endif

#ifdef ENABLE_BILLBOARDING
	frameTime += 1;

#endif

#ifdef ENABLE_WATER

	moveFactor += 0.0003f;
	if (moveFactor >= 360.0f)
		moveFactor -= 360.0f;
#endif

	// update camera using lerp
	//cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	//cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);
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
#endif

#ifdef ENABLE_WATER
	uninitializeWater(&waterTextureVariables);
#endif

#ifdef ENABLE_STARFIELD
	uninitializeStarfield(texture_star);
#endif // ENABLE_STARFIELD

#ifdef ENABLE_SKYBOX
	uninitialiseSkybox(texture_skybox);
#endif // ENABLE_SKYBOX

#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariables);
#endif

#ifdef ENABLE_CLOUD_NOISE
	
	uninitializeCloud();
	if (noise_texture)
	{
		glDeleteTextures(1, &noise_texture);
		noise_texture = 0;
	}
#endif

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

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&rockModel);
	unloadStaticModel(&streetLightModel);
#endif

#ifdef ENABLE_DYNAMIC_MODELS
	unloadDynamicModel(&dancingVampire);
#endif

#ifdef ENABLE_GAUSSIAN_BLUR
	uninitializeGaussianBlur(&gaussianBlurEffect);
#endif

}
