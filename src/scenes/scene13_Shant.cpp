#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/constants.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../inc/shaders/BillboardingShader.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"

#include "../../inc/scenes/scenePlaceHolderIndoor.h"
#include "../../inc/scenes/scene13_Shant.h"
#include "../../inc/debug/debug_transformation.h"

#define _USE_MATH_DEFINES 1
#include <math.h>		// for PI

#ifdef ENABLE_EROSION
#include "../../inc/effects/ErosionEffect.h"
#endif // ENABLE_EROSION

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static struct ADSUniform sceneIndoorADSUniform;

extern GLfloat density;
extern GLfloat gradient;
extern GLfloat skyFogColor[];

static GLfloat lightAmbient_shantRas[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse_shantRas[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular_shantRas[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition_shantRas[] = { 10.0f, 10.0f, 0.0f, 1.0f };

static GLfloat materialAmbient_shantRas[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse_shantRas[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular_shantRas[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess_shantRas = 128.0f;

static GLuint texture_ceiling;
static GLuint texture_floor;
static GLuint texture_side;
static GLuint texture_back;


//Model variables
static STATIC_MODEL deskModel;

static GLuint textures[4];

#ifdef ENABLE_STATIC_MODELS
static STATIC_MODEL shantRoomModel;
//static STATIC_MODEL maskModel;

static STATIC_MODEL maskModel_karunRas;
static STATIC_MODEL maskModel_bhayanakRas;
static STATIC_MODEL maskModel_raudraRas;
static STATIC_MODEL maskModel_bibhatsaRas;
static STATIC_MODEL maskModel_veerRas;
static STATIC_MODEL maskModel_adbhutRas;
static STATIC_MODEL maskModel_shringarRas;
static STATIC_MODEL maskModel_hasyaRas;
static STATIC_MODEL maskModel_shantRas;

#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_EROSION
struct ErosionNoiseUniform sceneErosionNoiseUniform;
GLuint noise_texture_eroded;
GLuint texture_Marble_Shant;
float myScale_erosion = 2.0f;
float noiseScale_erosion = 2.0f;
bool offsetIncrement = true;
// Mask Textures
GLuint texture_karunRas;
GLuint texture_bhayanakRas;
GLuint texture_raudraRas;
GLuint texture_bibhatsaRas;
GLuint texture_veerRas;
GLuint texture_adbhutRas;
GLuint texture_shringarRas;
GLuint texture_hasyaRas;
GLuint texture_shantRas;

GLuint textures_masks[9];

static STATIC_MODEL model_masks[9];

GLfloat offset_ras[9][3] =
{
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f },
	{ 0.48f, 0.48f, 0.48f }
};

bool isMaskQuadEnabled[9] = { true, true, true, true, true, true, true, true, true };

GLfloat maskTranslationRadii[9] =
{
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f,
	0.0f
};

GLfloat maskScales[9] =
{
	0.00125f,
	0.00125f,
	0.00125f,
	0.00125f,
	0.00125f,
	0.00125f,
	0.00125f,
	0.00125f,
	0.00125f
};

bool masksTransformationsComplete = false;

static GLfloat lightAmbient_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular_shantRas_mask[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition_shantRas_mask[] = { 10.0f, 10.0f, 10.0f, 1.0f };

static GLfloat materialAmbient_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialDiffuse_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular_shantRas_mask[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess_shantRas_mask = 128.0f;
#endif // ENABLE_EROSION


bool isInitialDisplayScene13_ShantRas = true;

int initializeScene13_Shant(void)
{
#ifdef ENABLE_STATIC_MODELS
	// function declarations
	//loadStaticModel("res/models/rock/rock.obj", &maskModel);

	loadStaticModel("res/models/masks/KarunMask.obj", &maskModel_karunRas);
	loadStaticModel("res/models/masks/BhayanakMask.obj", &maskModel_bhayanakRas);
	loadStaticModel("res/models/masks/RaudraMask1.obj", &maskModel_raudraRas);
	loadStaticModel("res/models/masks/BeebhatsaMask.obj", &maskModel_bibhatsaRas);
	loadStaticModel("res/models/masks/VeerMask.obj", &maskModel_veerRas);
	loadStaticModel("res/models/masks/AdbhutMask.obj", &maskModel_adbhutRas);
	loadStaticModel("res/models/masks/ShringarMask.obj", &maskModel_shringarRas);
	loadStaticModel("res/models/masks/HasyaMask.obj", &maskModel_hasyaRas);
	loadStaticModel("res/models/masks/ShantaMask.obj", &maskModel_shantRas);

	loadStaticModel("res/models/scene13_shanta/room/shantaRoom11.obj", &shantRoomModel);

	initializeQuad();

	//if (LoadGLTexture_UsingSOIL(&texture_bhayanakRas, TEXTURE_DIR"Masks/BhayanakMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_bhayanakRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_bhayanakRas Successfull = %u!!!\n", texture_bhayanakRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_bibhatsaRas, TEXTURE_DIR"Masks/BibhatsaMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_bibhatsaMask FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_bibhatsaMask Successfull = %u!!!\n", texture_bibhatsaRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_karunRas, TEXTURE_DIR"Masks/KarunMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_karunRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_karunRas Successfull = %u!!!\n", texture_karunRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_raudraRas, TEXTURE_DIR"Masks/RaudraMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_raudraRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_raudraRas Successfull = %u!!!\n", texture_raudraRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_veerRas, TEXTURE_DIR"Masks/VeerMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_veerRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_veerRas Successfull = %u!!!\n", texture_veerRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_adbhutRas, TEXTURE_DIR"Masks/AdbhutMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_adbhutRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_adbhutRas Successfull = %u!!!\n", texture_adbhutRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_shringarRas, TEXTURE_DIR"Masks/ShringarMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_shringarRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_shringarRas Successfull = %u!!!\n", texture_shringarRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_hasyaRas, TEXTURE_DIR"Masks/HasyaMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_hasyaRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_hasyaRas Successfull = %u!!!\n", texture_hasyaRas);
	//}

	//if (LoadGLTexture_UsingSOIL(&texture_shantRas, TEXTURE_DIR"Masks/ShantMask.jpg") == FALSE)
	//{
	//	//uninitialize();
	//	LOG("LoadGLTexture for texture_shantRas FAILED!!!\n");
	//	return(-1);
	//}
	//else
	//{
	//	LOG("LoadGLTexture texture_shantRas Successfull = %u!!!\n", texture_shantRas);
	//}

	if (LoadGLTexture_UsingSOIL(&texture_bhayanakRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_bhayanakRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_bhayanakRas Successfull = %u!!!\n", texture_bhayanakRas);
	}

	if (LoadGLTexture_UsingSOIL(&texture_bibhatsaRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_bibhatsaMask FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_bibhatsaMask Successfull = %u!!!\n", texture_bibhatsaRas);
	}

	if (LoadGLTexture_UsingSOIL(&texture_karunRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_karunRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_karunRas Successfull = %u!!!\n", texture_karunRas);
	}

	if (LoadGLTexture_UsingSOIL(&texture_raudraRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_raudraRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_raudraRas Successfull = %u!!!\n", texture_raudraRas);
	}

	if (LoadGLTexture_UsingSOIL(&texture_veerRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_veerRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_veerRas Successfull = %u!!!\n", texture_veerRas);
	}

	if (LoadGLTexture_UsingSOIL(&texture_adbhutRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_adbhutRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_adbhutRas Successfull = %u!!!\n", texture_adbhutRas);
	}

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

	if (LoadGLTexture_UsingSOIL(&texture_hasyaRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_hasyaRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_hasyaRas Successfull = %u!!!\n", texture_hasyaRas);
	}

	if (LoadGLTexture_UsingSOIL(&texture_shantRas, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_shantRas FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_shantRas Successfull = %u!!!\n", texture_shantRas);
	}

#ifdef ENABLE_EROSION
	textures_masks[0] = (GLuint)texture_karunRas;
	textures_masks[1] = (GLuint)texture_bhayanakRas;
	textures_masks[2] = (GLuint)texture_raudraRas;
	textures_masks[3] = (GLuint)texture_bibhatsaRas;
	textures_masks[4] = (GLuint)texture_veerRas;
	textures_masks[5] = (GLuint)texture_adbhutRas;
	textures_masks[6] = (GLuint)texture_shringarRas;
	textures_masks[7] = (GLuint)texture_hasyaRas;
	textures_masks[8] = (GLuint)texture_shantRas;

	LOG("Shant Ras coming here in initialize 1\n");

	model_masks[0] = maskModel_karunRas;
	model_masks[1] = maskModel_bhayanakRas;
	model_masks[2] = maskModel_raudraRas;
	model_masks[3] = maskModel_bibhatsaRas;
	model_masks[4] = maskModel_veerRas;
	model_masks[5] = maskModel_adbhutRas;
	model_masks[6] = maskModel_shringarRas;
	model_masks[7] = maskModel_hasyaRas;
	model_masks[8] = maskModel_shantRas;

	LOG("Shant Ras coming here in initialize 2\n");

#endif // ENABLE_EROSION

	//	//load models
//	if (LoadGLTexture_UsingSOIL(&texture_ceiling, TEXTURE_DIR"Shanta\\ceiling.jpg") == FALSE) {
//		uninitializeScene13_Shant();
//		LOG("LoadGLTexture FAILED in Shanta!!!\n");
//		return(-1);
//	}
//	else
//	{
//		LOG("LoadGLTexture Successfull = %u!!!\n", texture_ceiling);
//	}
//	if (LoadGLTexture_UsingSOIL(&texture_floor, TEXTURE_DIR"Shanta\\floor.jpg") == FALSE) {
//		uninitializeScene13_Shant();
//		LOG("LoadGLTexture FAILED in floor Shanta!!!\n");
//		return(-1);
//	}
//	else
//	{
//		LOG("LoadGLTexture Successfull = %u!!!\n", texture_floor);
//	}
//	if (LoadGLTexture_UsingSOIL(&texture_back, TEXTURE_DIR"Shanta\\back.jpg") == FALSE) {
//		uninitializeScene13_Shant();
//		LOG("LoadGLTexture FAILED in backwall Shanta!!!\n");
//		return(-1);
//	}
//	else
//	{
//		LOG("LoadGLTexture Successfull = %u!!!\n", texture_back);
//	}
//	if (LoadGLTexture_UsingSOIL(&texture_side, TEXTURE_DIR"Shanta\\sidewall.jpg") == FALSE) {
//		uninitializeScene13_Shant();
//		LOG("LoadGLTexture FAILED for sidewall Shanta!!!\n");
//		return(-1);
//	}
//	else
//	{
//		LOG("LoadGLTexture Successfull = %u!!!\n", texture_side);
//	}
//
#endif
//	initializeInvertedNormalCube();
//	initializeQuad();
//
//	textures[0] = (GLuint)texture_ceiling;
//	textures[1] = (GLuint)texture_floor;
//	textures[2] = (GLuint)texture_back;
//	textures[3] = (GLuint)texture_side;

	// external debugging varaible
    tf_t = {2.0f, 0.0f, -3.35f}; // tree pos 
    // tf_s = {0.75f, 0.75f, 0.75f}; // tree scale 
    // tf_r = {0.0f, 0.0f, 0.0f}; // tree rotation 
	tf_Speed = 0.05f;
//	glEnable(GL_TEXTURE_2D);

#ifdef ENABLE_EROSION

	if (LoadGLTexture_UsingSOIL(&texture_Marble_Shant, TEXTURE_DIR"marble.bmp") == GL_FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble_Shant);
	}

	noise_texture_eroded = initializeErosion();
	if (noise_texture_eroded == 0)
	{
		LOG("initializeErosion() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeErosion() Successfull!!!\n");
	}
#endif // ENABLE_EROSION


	return 0;
}

void setCameraScene13_ShantRas(void)
{
	if (isInitialDisplayScene13_ShantRas == true)
	{
		//setCamera(17.50f, 1.10f, -6.95f, -90.24f, 59.45f, -355.27f, 0.0f, 0.5f, 0.5f);
		//setCamera(-0.70, 0.00, -1.85, -0.70, 0.00, -7.85, 0.0f, 1.0f, 0.0f);
		// updated initial position
		// lookAt(-0.70f, 0.00f, -2.75f, -0.70f, 0.00f, -8.75f, 0.00f, 1.00f, 0.00f)
		setCamera(-0.70f, 0.00f, -2.75f, -0.70f, 0.00f, -8.75f, 0.00f, 1.00f, 0.00f);
		isInitialDisplayScene13_ShantRas = false;
	}
}


void displayScene13_Shant(void)
{
    // set camera
	setCameraScene13_ShantRas();

	displayCamera();

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
    #ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneIndoorADSUniform = useADSShader();

	glUniform1i(sceneIndoorADSUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneIndoorADSUniform.laUniform, 1, lightAmbient_shantRas);
	glUniform4fv(sceneIndoorADSUniform.ldUniform, 1, lightDiffuse_shantRas);
	glUniform4fv(sceneIndoorADSUniform.lsUniform, 1, lightSpecular_shantRas);
	glUniform4fv(sceneIndoorADSUniform.lightPositionUniform, 1, lightPosition_shantRas);
	glUniform4fv(sceneIndoorADSUniform.kaUniform, 1, materialAmbient_shantRas);
	glUniform4fv(sceneIndoorADSUniform.kdUniform, 1, materialDiffuse_shantRas);
	glUniform4fv(sceneIndoorADSUniform.ksUniform, 1, materialSpecular_shantRas);
	glUniform1f(sceneIndoorADSUniform.materialShininessUniform, materialShininess_shantRas);

	glUniform1i(sceneIndoorADSUniform.fogEnableUniform, 0);
	glUniform1f(sceneIndoorADSUniform.densityUniform, density);
	glUniform1f(sceneIndoorADSUniform.gradientUniform, gradient);
	glUniform4fv(sceneIndoorADSUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneIndoorADSUniform.uniform_enable_godRays, 1);
	glUniform1i(sceneIndoorADSUniform.godrays_blackpass_sphere, 0);
	glUniform1i(sceneIndoorADSUniform.actualSceneUniform, 1);
	glUniform1i(sceneIndoorADSUniform.depthSceneUniform, 0);
	glUniform1i(sceneIndoorADSUniform.depthQuadSceneUniform, 0);

	// ################################### ROOM ###################################  
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	// ------ Room Model ------
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.1f, 0.1f, 0.1f);
	rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix_y;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(shantRoomModel);

	glUseProgram(0);
	// ################################### ROOM ###################################  

	// ################################### MASK ###################################  
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

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


	// ------ Mask Model ------
	//translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	//scaleMatrix = vmath::scale(0.1f, 0.1f, 0.1f);
	//rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

	//modelMatrix = translationMatrix * scaleMatrix * rotationMatrix_y;

	//glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	//glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	//glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	//glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion);
	//glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, vec3(0.32, 0.32, 0.32));

	//drawCustomTextureStaticModel(maskModel, texture_bhayanakRas, noise_texture_eroded);
	// ################################### ROOM ###################################  

	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);
	glDisable(GL_TEXTURE_2D);
    
#endif 

#ifdef ENABLE_EROSION

	//glEnable(GL_TEXTURE_3D);
	//sceneErosionNoiseUniform = useErosionNoiseShader();

	//// ras_1
	//translationMatrix = mat4::identity();
	//modelMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -8.0f);

	//modelMatrix = translationMatrix;

	//glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	//glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	//glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	//glUniform3fv(sceneErosionNoiseUniform.laUniform, 1, lightAmbient_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.ldUniform, 1, lightDiffuse_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.lsUniform, 1, lightSpecular_shantRas);
	//glUniform4fv(sceneErosionNoiseUniform.lightPositionUniform, 1, lightPosition_shantRas);

	//glUniform3fv(sceneErosionNoiseUniform.kaUniform, 1, materialAmbient_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.kdUniform, 1, materialDiffuse_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.ksUniform, 1, materialSpecular_shantRas);
	//glUniform1f(sceneErosionNoiseUniform.materialShininessUniform, materialShininess_shantRas);

	//glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion);
	//glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras_1);
	////glUniform3fv(sceneErosionNoiseUniform.skyColorUniform, 1, skyColorForVeerRas);
	////glUniform3fv(sceneErosionNoiseUniform.cloudColorUniform, 1, cloudColorForVeerRas);
	////glUniform1f(sceneErosionNoiseUniform.noiseScaleUniform, noiseScale_erosion);
	////glUniform1i(sceneErosionNoiseUniform.uniform_enable_godRays, godRays);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture_Marble_Shant);
	//glUniform1i(sceneErosionNoiseUniform.textureSamplerUniform, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_3D, noise_texture_eroded);
	//glUniform1i(sceneErosionNoiseUniform.noiseSamplerUniform, 1);

	//displayQuad();

	//// ras_2
	//translationMatrix = mat4::identity();
	//modelMatrix = mat4::identity();

	//translationMatrix = vmath::translate(-3.0f, 0.0f, -8.0f);

	//modelMatrix = translationMatrix;

	//glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	//glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	//glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	//glUniform3fv(sceneErosionNoiseUniform.laUniform, 1, lightAmbient_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.ldUniform, 1, lightDiffuse_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.lsUniform, 1, lightSpecular_shantRas);
	//glUniform4fv(sceneErosionNoiseUniform.lightPositionUniform, 1, lightPosition_shantRas);

	//glUniform3fv(sceneErosionNoiseUniform.kaUniform, 1, materialAmbient_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.kdUniform, 1, materialDiffuse_shantRas);
	//glUniform3fv(sceneErosionNoiseUniform.ksUniform, 1, materialSpecular_shantRas);
	//glUniform1f(sceneErosionNoiseUniform.materialShininessUniform, materialShininess_shantRas);

	//glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion);
	//glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras_2);
	////glUniform3fv(sceneErosionNoiseUniform.skyColorUniform, 1, skyColorForVeerRas);
	////glUniform3fv(sceneErosionNoiseUniform.cloudColorUniform, 1, cloudColorForVeerRas);
	////glUniform1f(sceneErosionNoiseUniform.noiseScaleUniform, noiseScale_erosion);
	////glUniform1i(sceneErosionNoiseUniform.uniform_enable_godRays, godRays);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture_Marble_Shant);
	//glUniform1i(sceneErosionNoiseUniform.textureSamplerUniform, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_3D, noise_texture_eroded);
	//glUniform1i(sceneErosionNoiseUniform.noiseSamplerUniform, 1);

	//displayQuad();

	//// Unuse the shaderProgramObject
	//glUseProgram(0);

	float angle = 0.0;

	glEnable(GL_TEXTURE_3D);
	glEnable(GL_TEXTURE_2D);
	sceneErosionNoiseUniform = useErosionNoiseShader();

	for (int i = 0; i < 9; i++)
	{
		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		//rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		float xPos = maskTranslationRadii[i] * cos(angle * M_PI / 180.0);
		float yPos = maskTranslationRadii[i] * sin(angle * M_PI / 180.0);

		translationMatrix = vmath::translate(xPos - 0.765f, yPos - 0.5f, -7.0f);
		scaleMatrix = vmath::scale(maskScales[i], maskScales[i], maskScales[i]);
		modelMatrix = translationMatrix * scaleMatrix/* * rotationMatrix*/;

		//glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		//glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		//glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		/*glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform3fv(sceneErosionNoiseUniform.laUniform, 1, lightAmbient_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.ldUniform, 1, lightDiffuse_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.lsUniform, 1, lightSpecular_shantRas_mask);
		glUniform4fv(sceneErosionNoiseUniform.lightPositionUniform, 1, lightPosition_shantRas_mask);

		glUniform3fv(sceneErosionNoiseUniform.kaUniform, 1, materialAmbient_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.kdUniform, 1, materialDiffuse_shantRas_mask);
		glUniform3fv(sceneErosionNoiseUniform.ksUniform, 1, materialSpecular_shantRas_mask);
		glUniform1f(sceneErosionNoiseUniform.materialShininessUniform, materialShininess_shantRas_mask);

		glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion);
		glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras[i]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures_masks[i]);
		glUniform1i(sceneErosionNoiseUniform.textureSamplerUniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_3D, noise_texture_eroded);
		glUniform1i(sceneErosionNoiseUniform.noiseSamplerUniform, 1);

		if (isMaskQuadEnabled[i])
			displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);*/

		glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion);
		glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras[i]);

		if (isMaskQuadEnabled[i])
			drawCustomTextureStaticModel(model_masks[i], textures_masks[i], noise_texture_eroded);

		angle = angle + 22.5f;
	}
	glUseProgram(0);
	glDisable(GL_TEXTURE_3D);

#endif // ENABLE_EROSION
}

void updateScene13_ShantRas(void)
{
#ifdef ENABLE_EROSION
	// update Cloud
	//if (offset_ras_1[0] <= 0.48f)
	//	updateErosion(offsetIncrement, offset_ras_1, 0.001f);

	//if (offset_ras_1[0] <= 0.33f)
	//	updateErosion(offsetIncrement, offset_ras_2, 0.001f);

	maskTranslationRadii[0] += 0.008f;
	maskScales[0] += 0.0001f;
	for (int i = 0; i < 9; i++)
	{
		if (maskTranslationRadii[i] >= 0.433f)
		{
			maskTranslationRadii[i + 1] += 0.008f;
			maskScales[i + 1] += 0.0001f;
		}
		if (maskTranslationRadii[i] >= 1.3f)
			maskTranslationRadii[i] = 1.3;

		if (maskTranslationRadii[8] == 1.3f)
			masksTransformationsComplete = true;

		//if (maskScales[i] >= 0.009375f)
		//	maskScales[i] = 0.009375f;
		if (maskScales[i] >= 0.005f)
			maskScales[i] = 0.005f;

		/*if (maskScales[i] >= 0.1125f)
			maskScales[i + 1] += 0.01f;
		if (maskScales[i] >= 0.25f)
			maskScales[i] = 0.025f;*/
	}

	if (masksTransformationsComplete == true)
	{
		updateErosion(offsetIncrement, offset_ras[8], 0.005f);
		for (int i = 8; i > -1; i--)
		{
			if (offset_ras[i][0] <= 0.33f)
				updateErosion(offsetIncrement, offset_ras[i - 1], 0.005f);
			if (offset_ras[i][0] <= 0.17f)
				isMaskQuadEnabled[i] = false;
		}
	}
#endif // ENABLE_EROSION

}

void uninitializeScene13_Shant(void)
{
    //UNINIT models
#ifdef ENABLE_STATIC_MODELS
	unloadStaticModel(&shantRoomModel);
	//unloadStaticModel(&maskModel);

	unloadStaticModel(&maskModel_karunRas);
	unloadStaticModel(&maskModel_bhayanakRas);
	unloadStaticModel(&maskModel_raudraRas);
	unloadStaticModel(&maskModel_bibhatsaRas);
	unloadStaticModel(&maskModel_veerRas);
	unloadStaticModel(&maskModel_adbhutRas);
	unloadStaticModel(&maskModel_shringarRas);
	unloadStaticModel(&maskModel_hasyaRas);
	unloadStaticModel(&maskModel_shantRas);
#endif // ENABLE_STATIC_MODELS

	if (texture_shantRas)
	{
		glDeleteTextures(1, &texture_shantRas);
		texture_shantRas = 0;
	}
	if (texture_hasyaRas)
	{
		glDeleteTextures(1, &texture_hasyaRas);
		texture_hasyaRas = 0;
	}
	if (texture_shringarRas)
	{
		glDeleteTextures(1, &texture_shringarRas);
		texture_shringarRas = 0;
	}
	if (texture_adbhutRas)
	{
		glDeleteTextures(1, &texture_adbhutRas);
		texture_adbhutRas = 0;
	}
	if (texture_veerRas)
	{
		glDeleteTextures(1, &texture_veerRas);
		texture_veerRas = 0;
	}
	if (texture_bibhatsaRas)
	{
		glDeleteTextures(1, &texture_bibhatsaRas);
		texture_bibhatsaRas = 0;
	}
	if (texture_raudraRas)
	{
		glDeleteTextures(1, &texture_raudraRas);
		texture_raudraRas = 0;
	}
	if (texture_bhayanakRas)
	{
		glDeleteTextures(1, &texture_bhayanakRas);
		texture_bhayanakRas = 0;
	}
	if (texture_karunRas)
	{
		glDeleteTextures(1, &texture_karunRas);
		texture_karunRas = 0;
	}

	if (texture_ceiling)
	{
		glDeleteTextures(1, &texture_ceiling);
		texture_ceiling = 0;
	}
	if (texture_floor)
	{
		glDeleteTextures(1, &texture_floor);
		texture_floor = 0;
	}
	if (texture_back)
	{
		glDeleteTextures(1, &texture_back);
		texture_floor = 0;
	}
	if (texture_side)
	{
		glDeleteTextures(1, &texture_side);
		texture_side = 0;
	}
#ifdef ENABLE_EROSION
	uninitializeErosion();
	if (noise_texture_eroded)
	{
		glDeleteTextures(1, &noise_texture_eroded);
		noise_texture_eroded = 0;
	}
#endif // ENABLE_EROSION
}

