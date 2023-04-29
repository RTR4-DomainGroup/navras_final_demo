#pragma once
// This File Will Be Replaced by Scene*.cpp

#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/CloudEffect.h"
#include "../../inc/effects/WaterEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/waterframebuffer.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/geometry.h"

#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/CloudNoiseShader.h"
#include "../../inc/shaders/BillboardingShader.h"

#include "../../inc/scenes/scenePlaceHolder.h"
//#include "../../inc/Noise.h"

//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####

#define ENABLE_CLOUD_NOISE
#define ENABLE_TERRIAN
#define ENABLE_WATER
// #define ENABLE_SKYBOX
// #define ENABLE_STARFIELD

#define ENABLE_STATIC_MODELS
#define ENABLE_BILLBOARDING

GLuint texture_Marble;
TEXTURE texture_grass;
TEXTURE texture_flower;

struct ADSUniform sceneADSUniform;

struct TerrainUniform terrainUniform;

struct CloudNoiseUniform sceneCloudNoiseUniform;

struct TextureVariables terrainTextureVariables;

struct BillboardingUniform billboardingEffectUniform;

// Water Related Variables
struct WaterUniform waterUniform;
struct TextureVariables waterTextureVariables;
struct WaterFrameBufferDetails waterReflectionFrameBufferDetails;
struct WaterFrameBufferDetails waterRefractionFrameBufferDetails;
GLfloat waterHeight = 0.0f;
GLfloat moveFactor = 0.0f;
GLfloat planeReflection[] = { 0.0f, 1.0f, 0.0f, -waterHeight };
GLfloat planeRefration[] = { 0.0f, -1.0f, 0.0f, waterHeight };

extern int windowWidth;
extern int windowHeight;

extern mat4 viewMatrix;

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


float displacementmap_depth;

// Variables For Skybox
GLuint texture_skybox;
struct SkyboxUniform sceneSkyBoxUniform;

// Variables For Starfield
GLuint texture_star; 
double deltaTime;
struct StarfieldUniform sceneStarfieldUniform;

//Model variables
STATIC_MODEL rockModel;
STATIC_MODEL streetLightModel;

int initializeScene_PlaceHolder(void)
{

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


	//
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

    // initializeCube();
    // initializePyramid();
    // initializeQuad();
    // initializeTriangle();
     //initializeSphere();

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
	initializeQuad();

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

	return 0;
}

void displayScene_PlaceHolder(void)
{
	// Function Declarations
	void displayWaterFramebuffers(void);

	// Code
	// Here The Game STarts

	//2 framebuffers for water effect
	displayWaterFramebuffers();
	
	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	// scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	// rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	// rotationMatrix_y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	// rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	// rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	// modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

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

	// Call Geometry over here 
	displayCube();
	// displayTriangle();
	// displayQuad();
	// displayPyramid();
	// displaySphere();
	
	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);

#endif // ENABLE_ADSLIGHT
	
	
#ifdef ENABLE_CLOUD_NOISE

	glEnable(GL_TEXTURE_3D);

	sceneCloudNoiseUniform = useCloudNoiseShader();

	//mat4 viewMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
	translationMatrix = vmath::translate(0.0f, 0.0f, -500.0f); // glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
	scaleMatrix = vmath::scale(800.0f, 450.0f, 1.0f);
	//rotateX = vmath::rotate(10.0f, 1.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix;

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

	//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noise_texture);

	displayQuad();

	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);

#endif


#ifdef ENABLE_TERRIAN
	// Terrain

	terrainUniform = useTerrainShader();

	vmath::mat4 mv_matrix = viewMatrix * (translate(0.0f, -2.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));

	vmath::mat4 proj_matrix = perspectiveProjectionMatrix;

	glUniformMatrix4fv(terrainUniform.uniform_mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);

	glUniform1f(terrainUniform.uniform_dmap_depth, displacementmap_depth);
	//glUniform1i(terrainUniform.uniform_enable_fog, enable_fog ? 1 : 0);
	glUniform1i(terrainUniform.uniform_enable_fog, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.albedo);


	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

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

#ifdef ENABLE_STARFIELD

	sceneStarfieldUniform = useStarfieldShader();

	float time = (float)deltaTime;

	time = time * 0.05f;
	time = time - floor(time);

	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);

	glUniform1f(sceneStarfieldUniform.timeUniform, time);

	displayStarfield(texture_star);
	glUseProgram(0);

#endif // ENABLE_STARFIELD

#ifdef ENABLE_STATIC_MODELS
	//MODELS

	sceneADSUniform = useADSShader();

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


	// ------ Rock Model ------
	translationMatrix = vmath::translate(-1.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

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

	glUniformMatrix4fv(sceneADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

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

	displayWater();

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

#endif

#ifdef ENABLE_BILLBOARDING	

	billboardingEffectUniform = useBillboardingShader();

//////////////////////////////////////////

	// instanced quads with grass texture
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	if (texture_grass.height > texture_grass.width)
		scaleMatrix = vmath::scale(texture_grass.width / (GLfloat)texture_grass.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_grass.height / (GLfloat)texture_grass.width, 1.0f);

	translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform1i(billboardingEffectUniform.textureSamplerUniform, 0);

	// if(bBillboardingEnabled)
	glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 1);
	// else
	//     glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_grass.id);

	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw

	glBindTexture(GL_TEXTURE_2D, 0);

	/// Flower

	if (texture_flower.height > texture_flower.width)
		scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

	translationMatrix = vmath::translate(1.5f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_flower.id);

    displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

#endif // ENABLE_BILLBOARDING


}

void displayWaterFramebuffers(void) {

	// Code

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();

	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	mat4 rotateX = mat4::identity();

	glEnable(GL_CLIP_DISTANCE0);

	glBindFramebuffer(GL_FRAMEBUFFER, waterReflectionFrameBufferDetails.frameBuffer);

	glViewport(0, 0, (GLsizei)waterReflectionFrameBufferDetails.textureWidth, (GLsizei)waterReflectionFrameBufferDetails.textureHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterReflectionFrameBufferDetails.textureWidth / waterReflectionFrameBufferDetails.textureHeight, 0.1f, 1000.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	waterUniform = useWaterShader();

	glUniform4fv(waterUniform.planeUniform, 1, planeReflection);

	float distance = 2 * (cameraEyeY - waterHeight);
	cameraEyeY -= distance;

	setCamera();

	cameraEyeY += distance;
	setCamera();

	//glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);




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
	
	//mat4 viewMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
	translationMatrix = vmath::translate(0.0f, 0.0f, -500.0f); // glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
	scaleMatrix = vmath::scale(800.0f, 450.0f, 1.0f);
	//rotateX = vmath::rotate(10.0f, 1.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotateX;

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

	//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noise_texture);

	displayQuad();

	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);

#endif





#ifdef ENABLE_TERRIAN
	// Terrain

	terrainUniform = useTerrainShader();

	vmath::mat4 mv_matrix = (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f)) * viewMatrix;

	vmath::mat4 proj_matrix = perspectiveProjectionMatrix;

	glUniformMatrix4fv(terrainUniform.uniform_mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);

	glUniform1f(terrainUniform.uniform_dmap_depth, displacementmap_depth);
	//glUniform1i(terrainUniform.uniform_enable_fog, enable_fog ? 1 : 0);
	glUniform1i(terrainUniform.uniform_enable_fog, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.albedo);


	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

#endif

#ifdef ENABLE_BILLBOARDING	

	// Code
	billboardingEffectUniform = useBillboardingShader();
//////////////////////////////////////////
	// instanced quads with grass texture

	// translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	if (texture_grass.height > texture_grass.width)
		scaleMatrix = vmath::scale(texture_grass.width / (GLfloat)texture_grass.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_grass.height / (GLfloat)texture_grass.width, 1.0f);


	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform1i(billboardingEffectUniform.textureSamplerUniform, 0);

	// if(bBillboardingEnabled)
	glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 1);
	// else
	//     glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_grass.id);

	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw


	glBindTexture(GL_TEXTURE_2D, 0);

	/// Flower

	if (texture_flower.height > texture_flower.width)
		scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

	translationMatrix = vmath::translate(1.5f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_flower.id);

	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

#endif // ENABLE_BILLBOARDING
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	glBindFramebuffer(GL_FRAMEBUFFER, waterRefractionFrameBufferDetails.frameBuffer);

	glViewport(0, 0, (GLsizei)waterRefractionFrameBufferDetails.textureWidth, (GLsizei)waterRefractionFrameBufferDetails.textureHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)waterRefractionFrameBufferDetails.textureWidth / waterRefractionFrameBufferDetails.textureHeight, 0.1f, 1000.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	waterUniform = useWaterShader();

	glUniform4fv(waterUniform.planeUniform, 1, planeRefration);



#ifdef ENABLE_CLOUD_NOISE

	glEnable(GL_TEXTURE_3D);

	sceneCloudNoiseUniform = useCloudNoiseShader();

	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	rotateX = mat4::identity();
	//mat4 viewMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
	translationMatrix = vmath::translate(0.0f, 0.0f, -500.0f); // glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
	scaleMatrix = vmath::scale(800.0f, 450.0f, 1.0f);
	//rotateX = vmath::rotate(10.0f, 1.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotateX;

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

	//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noise_texture);

	displayQuad();

	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);

#endif



#ifdef ENABLE_TERRIAN
	// Terrain

	terrainUniform = useTerrainShader();

	mv_matrix = mat4::identity();
	proj_matrix = mat4::identity();

	mv_matrix = viewMatrix * (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));

	proj_matrix = perspectiveProjectionMatrix;

	glUniformMatrix4fv(terrainUniform.uniform_mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix* mv_matrix);

	glUniform1f(terrainUniform.uniform_dmap_depth, displacementmap_depth);
	//glUniform1i(terrainUniform.uniform_enable_fog, enable_fog ? 1 : 0);
	glUniform1i(terrainUniform.uniform_enable_fog, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.albedo);


	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

#endif

#ifdef ENABLE_BILLBOARDING	

	// Code
	billboardingEffectUniform = useBillboardingShader();
//////////////////////////////////////////
	// instanced quads with grass texture

	// translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	if (texture_grass.height > texture_grass.width)
		scaleMatrix = vmath::scale(texture_grass.width / (GLfloat)texture_grass.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_grass.height / (GLfloat)texture_grass.width, 1.0f);


	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform1i(billboardingEffectUniform.textureSamplerUniform, 0);

	// if(bBillboardingEnabled)
	glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 1);
	// else
	//     glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_grass.id);

	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw


	glBindTexture(GL_TEXTURE_2D, 0);

	/// Flower

	if (texture_flower.height > texture_flower.width)
		scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

	translationMatrix = vmath::translate(1.5f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_flower.id);

	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

#endif // ENABLE_BILLBOARDING

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_CLIP_DISTANCE0);

}

void updateScene_PlaceHolder(void)
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
#endif

#ifdef ENABLE_WATER

	moveFactor += 0.0003f;
	if (moveFactor >= 360.0f)
		moveFactor -= 360.0f;
#endif

}

void uninitializeScene_PlaceHolder(void)
{
	// Code

	
#ifdef ENABLE_BILLBOARDING
    uninitializeQuad();
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

}

