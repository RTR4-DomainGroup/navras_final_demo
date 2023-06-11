// This File Will Be Replaced by Scene*.cpp

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/StarfieldShader.h"
#include "../../inc/shaders/ADSLightDynamicShader.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/scenes/scenePlaceHolderOutdoor.h"
#include "../../inc/Navras.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"


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

#ifdef ENABLE_EROSION
#include "../../inc/effects/ErosionEffect.h"
#endif // ENABLE_EROSION

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

#define FBO_EARTHANDSPACE_WIDTH (WIN_WIDTH * 3)  
#define FBO_EARTHANDSPACE_HEIGHT (WIN_HEIGHT * 3)

GLfloat whiteSphere[3] = {1.0f, 1.0f, 1.0f};
GLuint texture_Marble;
TEXTURE texture_grass;
TEXTURE texture_flower;

struct ADSUniform sceneOutdoorADSStaticUniform;
struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;


struct FSQuadUniform fsqUniform;

#ifdef ENABLE_TERRIAN
struct TerrainUniform terrainUniform;
#endif // ENABLE_TERRIAN

//#ifdef ENABLE_CLOUD_NOISE
//struct CloudNoiseUniform sceneCloudNoiseUniform;
// float myScale = 1.0f;
// float noiseScale = 2.0f;
// bool noiseScaleIncrement = true;
// GLfloat skyColor[] = { 0.0f, 0.0f, 0.8f, 0.0f };
// GLfloat cloudColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
//#endif // ENABLE_CLOUD_NOISE

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
AtmosphereUniform atmosphereUniform_outdoor;
AtmosphericVariables atmosVariables_outdoor;
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

struct FrameBufferDetails fboEarthAndSpace;

#ifdef ENABLE_MASKS
// Masks
static struct FrameBufferDetails fboMaskPass_Outdoor;
static GLuint texture_shringarRas;

static STATIC_MODEL maskModel_BhayanakRas;
static STATIC_MODEL maskModel_BibhastaRas;
static STATIC_MODEL maskModel_VeerRas;
static STATIC_MODEL maskModel_AdbhutRas;
static STATIC_MODEL maskModel_ShringarRas;

GLfloat lightAmbient_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse_shantRas_mask[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat lightSpecular_shantRas_mask[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition_shantRas_mask[] = { 0.0f, 10.0f, 10.0f, 1.0f };

GLfloat materialAmbient_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialDiffuse_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular_shantRas_mask[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat materialShininess_shantRas_mask = 128.0f;
extern struct ErosionNoiseUniform sceneErosionNoiseUniform;
static GLuint noise_texture_eroded_outdoor;
static float myScale_erosion_outdoor = 2.0f;
static float noiseScale_erosion_outdoor = 2.0f;
static bool offsetIncrement_outdoor = false;

static GLfloat offset_ras_outdoor[] = { 0.17f, 0.17f, 0.17f };
#endif // ENABLE_MASKS
static bool isBlur = false;

extern int windowWidth;
extern int windowHeight;

mat4 viewMatrix;

//GLuint noise_texture;

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
GLfloat lightPosition_gr[] = {0.0f, 4.0f, -60.0f, 1.0f};
#endif // ENABLE_GODRAYS

// Camera angle for rotation
//GLfloat cameraAngle = 100.0f;
GLfloat dispersal = 0.1875f;
GLfloat haloWidth = 0.45f;
GLfloat intensity = 1.5f;
GLfloat distortion[] = { 0.94f, 0.97f, 1.0f };

typedef void(*SET_CAMERA) (void);

typedef void (* DISPLAY_PASSES) (int,bool,bool,bool,int);


int initializeScene_PlaceHolderOutdoor(void)
{
    // Code.
	// initializeCamera(&camera);

#ifdef ENABLE_ADSLIGHT
	// Texture
	// if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == GL_FALSE) {
	if (LoadGLTexture_UsingSOIL(&texture_Marble, TEXTURE_DIR"marble.bmp") == GL_FALSE) {
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

	shadowFramebuffer.textureWidth = FBO_WIDTH;
	shadowFramebuffer.textureHeight = FBO_HEIGHT;

	if (shadowCreateFBO(&shadowFramebuffer) == GL_FALSE) {

		LOG("shadowCreateFBO() For Shadow FAILED!!!\n");
			return(-1);

	}
	else {

		LOG("shadowCreateFBO() Successfull for Shadow!!!\n");

	}

	initializeQuad();

#endif // ENABLE_SHADOW


#ifdef ENABLE_ATMOSPHERE

	//
	atmosVariables_outdoor.m_nSamples = 3;		// Number of sample rays to use in integral equation
	atmosVariables_outdoor.m_Kr = 0.0035f;		// Rayleigh scattering constant
	atmosVariables_outdoor.m_Kr4PI = atmosVariables_outdoor.m_Kr * 4.0f * M_PI;
	atmosVariables_outdoor.m_Km = 0.0015f;		// Mie scattering constant
	atmosVariables_outdoor.m_Km4PI = atmosVariables_outdoor.m_Km * 4.0f * M_PI;
	atmosVariables_outdoor.m_ESun = 20.0f;		// Sun brightness constant
	atmosVariables_outdoor.m_g = -0.990f;		// The Mie phase asymmetry factor
	atmosVariables_outdoor.m_fExposure = 2.0f;

	atmosVariables_outdoor.m_fInnerRadius = 10.0f;
	atmosVariables_outdoor.m_fOuterRadius = 50.0f;
	//atmosVariables_outdoor.m_fOuterRadius = atmosVariables_outdoor.m_fInnerRadius + (atmosVariables_outdoor.m_fInnerRadius * 2.5f);
	atmosVariables_outdoor.m_fScale = 1 / (atmosVariables_outdoor.m_fOuterRadius - atmosVariables_outdoor.m_fInnerRadius);

	atmosVariables_outdoor.m_fWavelength[0] = 0.650f;		// 650 nm for red
	atmosVariables_outdoor.m_fWavelength[1] = 0.570f;		// 570 nm for green
	atmosVariables_outdoor.m_fWavelength[2] = 0.475f;		// 475 nm for blue
	atmosVariables_outdoor.m_fWavelength4[0] = powf(atmosVariables_outdoor.m_fWavelength[0], 4.0f);
	atmosVariables_outdoor.m_fWavelength4[1] = powf(atmosVariables_outdoor.m_fWavelength[1], 4.0f);
	atmosVariables_outdoor.m_fWavelength4[2] = powf(atmosVariables_outdoor.m_fWavelength[2], 4.0f);

	atmosVariables_outdoor.m_fRayleighScaleDepth = 0.25f;
	atmosVariables_outdoor.m_fMieScaleDepth = 0.1f;

	atmosVariables_outdoor.m_vLight = vec3(0, 0, -350);
	atmosVariables_outdoor.m_vLightDirection = atmosVariables_outdoor.m_vLight / sqrtf(atmosVariables_outdoor.m_vLight[0] * atmosVariables_outdoor.m_vLight[0] + atmosVariables_outdoor.m_vLight[1] * atmosVariables_outdoor.m_vLight[1] + atmosVariables_outdoor.m_vLight[2] * atmosVariables_outdoor.m_vLight[2]);

	//
	initializeAtmosphere(atmosVariables_outdoor);

#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_GODRAYS
	int initializeGodRays(void);
	initializeSphere(2.5f, 60, 60);
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
	waterReflectionFrameBufferDetails.textureWidth = 1920;
	waterReflectionFrameBufferDetails.textureHeight = 1080;

	if (waterCreateFBO(&waterReflectionFrameBufferDetails) == GL_FALSE) {

		LOG("waterCreateFBO() For WaterReflection FAILED!!!\n");
		return(-1);

	}
	else {

		LOG("waterCreateFBO() Successfull for WaterReflection!!!\n");

	}

	waterRefractionFrameBufferDetails.textureWidth = 1920;
	waterRefractionFrameBufferDetails.textureHeight = 1080;

	if (waterCreateFBO(&waterRefractionFrameBufferDetails) == GL_FALSE) {

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

//#ifdef ENABLE_CLOUD_NOISE
//
//	noise_texture = initializeCloud();
//	if (noise_texture == 0)
//	{
//		LOG("initializeCloud() FAILED!!!\n");
//		return(-1);
//	}
//	else
//	{
//		LOG("initializeCloud() Successfull!!!\n");
//	}
//
//#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_STARFIELD

	fboEarthAndSpace.textureWidth = FBO_EARTHANDSPACE_WIDTH;
	fboEarthAndSpace.textureHeight = FBO_EARTHANDSPACE_HEIGHT;

	createFBO(&fboEarthAndSpace);

	if (initializeStarfield(&texture_star, TEXTURE_DIR"Starfield\\Star.png") != 0)
	{
		LOG("initializeStarfield() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeStarfield() Successfull!!!\n");
	}
#endif // ENABLE_STARFIELD


#ifdef ENABLE_GAUSSIAN_BLUR
	initializeQuad();
	if(initializeGaussianBlur(&gaussianBlurEffect) == false)
	{
		LOG("Initialize Gaussian Blur Effect FAILED!!");
		return (-7);
	}

	fullSceneFbo.textureWidth = WIN_WIDTH;
	fullSceneFbo.textureHeight = WIN_HEIGHT;

	if (createFBO(&fullSceneFbo) == false)
	{
		LOG("Unable to create FBO for entire scene");
		return (-8);
	}
	
#endif // ENABLE_GAUSSIAN_BLUR

#ifdef ENABLE_BILLBOARDING
	char imagefile[64] = {};
	// sprintf(imagefile, "%s", TEXTURE_DIR"/billboarding/flower3.png");
	// if (LoadGLTextureData_UsingSOIL(&texture_grass, imagefile) == GL_FALSE)
	// {
	// 	LOG("Texture loading failed for image %s\n", imagefile);
	// 	return (-6);
	// }

	sprintf(imagefile, "%s", TEXTURE_DIR"/billboarding/flower5.png");
	if (LoadGLTextureData_UsingSOIL(&texture_flower, imagefile) == GL_FALSE)
	{
		LOG("Texture loading failed for image %s\n", imagefile);
		return (-6);
	}

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_MASKS
	fboMaskPass_Outdoor.textureWidth = 1920;
	fboMaskPass_Outdoor.textureHeight = 1080;

	createFBO(&fboMaskPass_Outdoor);

	loadStaticModel("res/models/masks/individualScenes/AdbhutWithText.obj", &maskModel_AdbhutRas);
	loadStaticModel("res/models/masks/individualScenes/BhayanakWithText.obj", &maskModel_BhayanakRas);
	loadStaticModel("res/models/masks/individualScenes/ShringarWithText.obj", &maskModel_ShringarRas);
	loadStaticModel("res/models/masks/individualScenes/VeerWithText.obj", &maskModel_VeerRas);
	loadStaticModel("res/models/masks/individualScenes/BeebhatsaWithText.obj", &maskModel_BibhastaRas);



	if (LoadGLTexture_UsingSOIL(&texture_shringarRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_shringarRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_shringarRas Successfull = %u!!!\n", texture_shringarRas);
	}

	noise_texture_eroded_outdoor = initializeErosion();
	if (noise_texture_eroded_outdoor == 0)
	{
		LOG("initializeErosion() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeErosion() Successfull!!!\n");
	}
#endif // ENABLE_MASKS

	return 0;
}

void displayScene_PlaceHolderOutdoor(SET_CAMERA setCamera, DISPLAY_PASSES displayPasses, bool isGodRequired, bool isWaterRequired, bool isGaussianBlurRequired)
{
	// Function Declarations
	void displayGodRays(int, int);

	// Code
	// Here The Game STarts
	// set cameraa
	setCamera();

	isBlur = isGaussianBlurRequired;

	displayCamera();
	//setCamera(&camera);

	//rotateCamera(0.0f, 10.0f, 0.0f, 50.0f, cameraAngle);

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);

	if(isWaterRequired) 
	{
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

#ifdef ENABLE_STARFIELD

	glBindFramebuffer(GL_FRAMEBUFFER, fboEarthAndSpace.frameBuffer);

	glViewport(0, 0, (GLsizei)fboEarthAndSpace.textureWidth, (GLsizei)fboEarthAndSpace.textureHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboEarthAndSpace.textureWidth / fboEarthAndSpace.textureHeight, 0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneStarfieldUniform = useStarfieldShader();

	float time = (float)deltaTime;

	time = time * 0.05f;
	time = time - floor(time);

	// Transformations
	translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -56.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);
	glUniform1f(sceneStarfieldUniform.timeUniform, time);

	displayStarfield(texture_star);
	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#endif // ENABLE_STARFIELD

#ifdef ENABLE_SHADOW

	// Shadow
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer.frameBuffer);
	glViewport(0, 0, (GLsizei)shadowFramebuffer.textureWidth, (GLsizei)shadowFramebuffer.textureHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	perspectiveProjectionMatrix = vmath::perspective(90.0f, (GLfloat)shadowFramebuffer.textureWidth / shadowFramebuffer.textureHeight, 0.1f, 100.0f);
	displayPasses(1, true, true, false, 1);
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

#ifdef ENABLE_MASKS
	// Masks
	glBindFramebuffer(GL_FRAMEBUFFER, fboMaskPass_Outdoor.frameBuffer);
		glViewport(0, 0, (GLsizei)fboMaskPass_Outdoor.textureWidth, (GLsizei)fboMaskPass_Outdoor.textureHeight);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboMaskPass_Outdoor.textureWidth / fboMaskPass_Outdoor.textureHeight, 0.1f, 100.0f);

		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		mat4 rotationMatrix_x = mat4::identity();
		mat4 rotationMatrix_y = mat4::identity();
		mat4 rotationMatrix_z = mat4::identity();

		//float angle = 0.0;

		glEnable(GL_TEXTURE_3D);
		glEnable(GL_TEXTURE_2D);
		sceneErosionNoiseUniform = useErosionNoiseShader();

		glUniform3fv(sceneErosionNoiseUniform.laUniform, 1, lightAmbient_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.ldUniform, 1, lightDiffuse_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.lsUniform, 1, lightSpecular_shantRas_mask);
		glUniform4fv(sceneErosionNoiseUniform.lightPositionUniform, 1, lightPosition_shantRas_mask);

		glUniform3fv(sceneErosionNoiseUniform.kaUniform, 1, materialAmbient_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.kdUniform, 1, materialDiffuse_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.ksUniform, 1, materialSpecular_shantRas_mask);
		glUniform1f(sceneErosionNoiseUniform.materialShininessUniform, materialShininess_shantRas_mask);

		//glDisable(GL_BLEND);
		// ------ Mask Model ------
		translationMatrix = vmath::translate(0.0f, -1.0f, -6.0f);
		scaleMatrix = vmath::scale(0.025f, 0.025f, 0.025f);
		//rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix_y;

		glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, mat4::identity());
		glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion_outdoor);
		//glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, vec3(0.32, 0.32, 0.32));
		glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras_outdoor);

		if(getCurrentScene() == SCENE06_BHAYANK_RAS)
		{

			drawCustomTextureStaticModel(maskModel_BhayanakRas, texture_shringarRas, noise_texture_eroded_outdoor);

		}
		else if(getCurrentScene() == SCENE08_BIBHATSA_RAS)
		{

			drawCustomTextureStaticModel(maskModel_BibhastaRas, texture_shringarRas, noise_texture_eroded_outdoor);

		}
		else if(getCurrentScene() == SCENE09_VEER_RAS)
		{

			drawCustomTextureStaticModel(maskModel_VeerRas, texture_shringarRas, noise_texture_eroded_outdoor);

		}
		else if(getCurrentScene() == SCENE10_ADBHUT_RAS)
		{

			drawCustomTextureStaticModel(maskModel_AdbhutRas, texture_shringarRas, noise_texture_eroded_outdoor);

		}
		else if(getCurrentScene() == SCENE11_SHRINGAR_RAS)
		{

			drawCustomTextureStaticModel(maskModel_ShringarRas, texture_shringarRas, noise_texture_eroded_outdoor);

		}


		//drawCustomTextureStaticModel(maskModel_shringarRas, texture_shringarRas, noise_texture_eroded_outdoor);
		// ################################### ROOM ###################################  

		glUseProgram(0);

		glDisable(GL_TEXTURE_3D);
		glDisable(GL_TEXTURE_2D);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif // ENABLE_MASKS


	//
	if(!isGaussianBlurRequired && !isGodRequired) 
	{
		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, 
			(GLfloat)windowWidth / windowHeight, 0.01f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(1, false, false, isWaterRequired, 0);
	}
	else if(isGaussianBlurRequired && !isGodRequired)
	{
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
		glUniform1i(fsGaussBlurQuadUniform.singleTexture, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurEffect.verticalFBDetails.frameBufferTexture);
		glUniform1i(fsGaussBlurQuadUniform.textureSamplerUniform1, 0);
		displayQuad();
    	glBindTexture(GL_TEXTURE_2D, 0);
		

		////
#ifdef ENABLE_MASKS
		glUniform1i(fsGaussBlurQuadUniform.singleTexture, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboMaskPass_Outdoor.frameBufferTexture);
		glUniform1i(fsGaussBlurQuadUniform.textureSamplerUniform1, 0);

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);
#endif


		glUseProgram(0);

	}
	else if(isGodRequired && !isGaussianBlurRequired)
	{
		// GodRay Black pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboBlackPass.textureWidth, (GLsizei)fboBlackPass.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPass.textureWidth / fboBlackPass.textureHeight, 
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(0, false, false, isWaterRequired, 0);

		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		
		translationMatrix = vmath::translate(lightPosition_gr[0], lightPosition_gr[1], lightPosition_gr[2]);
		scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
		modelMatrix = translationMatrix * scaleMatrix;
		
		sceneOutdoorADSStaticUniform = useADSShader();
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(sceneOutdoorADSStaticUniform.uniform_enable_godRays, 0);
		glUniform1i(sceneOutdoorADSStaticUniform.godrays_blackpass_sphere, 1);
		float color[3] = {1.0f, 1.0f, 1.0f};
		if(getCurrentScene() == SCENE02_EARTH_AND_SPACE)
		{
			color[0] = 1.0f;
			color[1] = 0.65f;
			color[2] = 0.01f;
		}
		//glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f,1.0f,1.0f));

		displaySphere(color);
		glUseProgram(0);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// GodRay Color Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboColorPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboColorPass.textureWidth, (GLsizei)fboColorPass.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboColorPass.textureWidth / fboColorPass.textureHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(1, false, false, isWaterRequired, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// God Rays Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboGodRayPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboGodRayPass.textureWidth, (GLsizei)fboGodRayPass.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboGodRayPass.textureWidth / fboGodRayPass.textureHeight, 0.1f, 1000.0f);
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
		if(getCurrentScene() == SCENE02_EARTH_AND_SPACE) {
			glUniform1i(sceneGodRaysUniform.godrays_lfEnabled, 0);
		}
		// else
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
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		fsqUniform = useFSQuadShader();
		//glEnable(GL_BLEND);

		glUniform1i(fsqUniform.singleTexture, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboGodRayPass.frameBufferTexture);
		glUniform1i(fsqUniform.textureSamplerUniform1, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fboColorPass.frameBufferTexture);
		glUniform1i(fsqUniform.textureSamplerUniform2, 1);

		/*glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, fboMaskPass_Outdoor.frameBufferTexture);
		glUniform1i(fsqUniform.textureSamplerUniform3, 2);*/

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);

		//glDisable(GL_BLEND);

		//
		/*glUniform1i(fsGaussBlurQuadUniform.singleTexture, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboMaskPass_Outdoor.frameBufferTexture);
		glUniform1i(fsGaussBlurQuadUniform.textureSamplerUniform1, 0);

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);*/
		glUseProgram(0);
	
	}
	else if (isGodRequired && isGaussianBlurRequired)
	{
		
		// GodRay Black pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboBlackPass.textureWidth, (GLsizei)fboBlackPass.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPass.textureWidth / fboBlackPass.textureHeight,
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(0, false, false, isWaterRequired, 0);

		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(lightPosition_gr[0], lightPosition_gr[1], lightPosition_gr[2]);
		scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
		modelMatrix = translationMatrix * scaleMatrix;

		sceneOutdoorADSStaticUniform = useADSShader();
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(sceneOutdoorADSStaticUniform.uniform_enable_godRays, 0);
		glUniform1i(sceneOutdoorADSStaticUniform.godrays_blackpass_sphere, 1);
		float color[3] = { 1.0f, 1.0f, 1.0f };
		if (getCurrentScene() == SCENE02_EARTH_AND_SPACE)
		{
			color[0] = 1.0f;
			color[1] = 0.65f;
			color[2] = 0.01f;
		}
		//glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f,1.0f,1.0f));

		displaySphere(color);
		glUseProgram(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// GodRay Color Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboColorPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboColorPass.textureWidth, (GLsizei)fboColorPass.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboColorPass.textureWidth / fboColorPass.textureHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		displayPasses(1, false, false, isWaterRequired, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// God Rays Pass
		glBindFramebuffer(GL_FRAMEBUFFER, fboGodRayPass.frameBuffer);
		glViewport(0, 0, (GLsizei)fboGodRayPass.textureWidth, (GLsizei)fboGodRayPass.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboGodRayPass.textureWidth / fboGodRayPass.textureHeight, 0.1f, 1000.0f);
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
		if (getCurrentScene() == SCENE02_EARTH_AND_SPACE) {
			glUniform1i(sceneGodRaysUniform.godrays_lfEnabled, 0);
		}
		// else
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
		//displayPasses(1, false, false, isWaterRequired, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, fullSceneFbo.frameBuffer);
		glViewport(0, 0, (GLsizei)fullSceneFbo.textureWidth, (GLsizei)fullSceneFbo.textureHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fullSceneFbo.textureWidth / fullSceneFbo.textureHeight,
			0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsqUniform = useFSQuadShader();
		//glEnable(GL_BLEND);

		glUniform1i(fsqUniform.singleTexture, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboGodRayPass.frameBufferTexture);
		glUniform1i(fsqUniform.textureSamplerUniform1, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fboColorPass.frameBufferTexture);
		glUniform1i(fsqUniform.textureSamplerUniform2, 1);

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Gausian Blur
		displayGaussianBlur();

		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsGaussBlurQuadUniform = useFSQuadShader();
		glUniform1i(fsGaussBlurQuadUniform.singleTexture, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurEffect.verticalFBDetails.frameBufferTexture);
		glUniform1i(fsGaussBlurQuadUniform.textureSamplerUniform1, 0);
		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);

		//
#ifdef ENABLE_MASKS
		if (getCurrentScene() != SCENE02_EARTH_AND_SPACE) {

			glUniform1i(fsGaussBlurQuadUniform.singleTexture, 1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, fboMaskPass_Outdoor.frameBufferTexture);
			glUniform1i(fsGaussBlurQuadUniform.textureSamplerUniform1, 0);

			displayQuad();
			glBindTexture(GL_TEXTURE_2D, 0);

		}
#endif
		glUseProgram(0);
	}


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

#ifdef ENABLE_GAUSSIAN_BLUR
void displayGaussianBlur(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurEffect.horrizontalFBDetails.frameBuffer);
	glViewport(0, 0, (GLsizei)gaussianBlurEffect.horrizontalFBDetails.textureWidth, 
	(GLsizei)gaussianBlurEffect.horrizontalFBDetails.textureHeight);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    horizontalBlurUniform = useHorrizontalBlurShader();

    glUniform1f(horizontalBlurUniform.targetWidth, 540.0f);
	glUniform1f(horizontalBlurUniform.blurFactor, 1.0f);
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
	glUniform1f(verticalBlurUniform.targetHeight, 270.0f);
	glUniform1f(verticalBlurUniform.blurFactor, 1.0f);
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

//#ifdef ENABLE_CLOUD_NOISE
//	// update Cloud
//	updateCloud(noiseScaleIncrement, noiseScale, 0.0001f);
//#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_WATER

	moveFactor += 0.0003f;
	if (moveFactor >= 360.0f)
		moveFactor -= 360.0f;
#endif // ENABLE_WATER

#ifdef ENABLE_MASKS

	// update camera using lerp
	//cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	//cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);
	/*cameraAngle = cameraAngle + 0.5f;
	if (cameraAngle >= 360.0f)
		cameraAngle -= 360.0f;*/
	// updateErosion(offsetIncrement_outdoor, offset_ras_outdoor[8], 0.002f);
	if (isBlur == false)
	{
		offset_ras_outdoor[0] = 0.17f;
		offset_ras_outdoor[1] = 0.17f;
		offset_ras_outdoor[2] = 0.17f;
	}

	if (((getCurrentScene() == SCENE06_BHAYANK_RAS) ||
		(getCurrentScene() == SCENE08_BIBHATSA_RAS) ||
		(getCurrentScene() == SCENE09_VEER_RAS) ||
		(getCurrentScene() == SCENE10_ADBHUT_RAS) ||
		(getCurrentScene() == SCENE11_SHRINGAR_RAS)) && 
		isBlur == true)
	{
		offset_ras_outdoor[0] = offset_ras_outdoor[0] + 0.002f;
		offset_ras_outdoor[1] = offset_ras_outdoor[1] + 0.002f;
		offset_ras_outdoor[2] = offset_ras_outdoor[2] + 0.002f;
		if (offset_ras_outdoor[2] > 0.48f)
		{
			offset_ras_outdoor[0] = 0.48f;
			offset_ras_outdoor[1] = 0.48f;
			offset_ras_outdoor[2] = 0.48f;
		}
	}
#endif // ENABLE_MASKS

}

void uninitializeScene_PlaceHolderOutdoor(void)
{
	// Code
#ifdef ENABLE_MASKS
	unloadStaticModel(&maskModel_AdbhutRas);
	unloadStaticModel(&maskModel_BhayanakRas);
	unloadStaticModel(&maskModel_BibhastaRas);
	unloadStaticModel(&maskModel_VeerRas);
	unloadStaticModel(&maskModel_ShringarRas);
#endif // ENABLE_MASKS
	

#ifdef ENABLE_BILLBOARDING

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

#ifdef ENABLE_ATMOSPHERE
	uninitializeAtmosphere();
#endif // ENABLE_ATMOSPHERE

//#ifdef ENABLE_CLOUD_NOISE
//	
//	uninitializeCloud();
//	if (noise_texture)
//	{
//		glDeleteTextures(1, &noise_texture);
//		noise_texture = 0;
//	}
//#endif // ENABLE_CLOUD_NOISE

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

