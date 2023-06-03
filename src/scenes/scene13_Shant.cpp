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

static GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 10.0f, 10.0f, 0.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

static GLuint texture_ceiling;
static GLuint texture_floor;
static GLuint texture_side;
static GLuint texture_back;


//Model variables
static STATIC_MODEL deskModel;

static GLuint textures[4];

#ifdef ENABLE_STATIC_MODELS
static STATIC_MODEL shantRoomModel;
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_EROSION
struct ErosionNoiseUniform sceneErosionNoiseUniform;
GLuint noise_texture_eroded;
GLuint texture_Marble_Shant;
float myScale_erosion = 0.5f;
float noiseScale_erosion = 2.0f;
bool offsetIncrement = true;
GLfloat offset_ras_1[] = { 0.48f, 0.48f, 0.48f };
GLfloat offset_ras_2[] = { 0.48f, 0.48f, 0.48f };
#endif // ENABLE_EROSION


bool isInitialDisplayScene13_ShantRas = true;

int initializeScene13_Shant(void)
{
#ifdef ENABLE_STATIC_MODELS
	// function declarations

	loadStaticModel("res/models/scene13_shanta/room/shantaRoom11.obj", &shantRoomModel);

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
		setCamera(-0.70, 0.00, -1.85, -0.70, 0.00, -7.85, 0.0f, 1.0f, 0.0f);
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
	glUniform1i(sceneIndoorADSUniform.actualSceneUniform, 1);
	glUniform1i(sceneIndoorADSUniform.depthSceneUniform, 0);
	glUniform1i(sceneIndoorADSUniform.depthQuadSceneUniform, 0);
	

	//translationMatrix = vmath::translate(0.0f, 0.0f, -1.0f);
	//scaleMatrix = vmath::scale(4.0f, 2.0f, 5.0f);
	//modelMatrix = translationMatrix * scaleMatrix;

	//glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	//glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	//glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	//glEnable(GL_TEXTURE_2D);
	
	//displayRoom(textures);

	// ################################### BUILDING ONE ###################################  
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	// ------ Streetlight Model ------
	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.1f, 0.1f, 0.1f);
	rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix_y;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(shantRoomModel);
	// ################################### BUILDING ONE ###################################  

	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
    #endif 

#ifdef ENABLE_EROSION

	glEnable(GL_TEXTURE_3D);
	sceneErosionNoiseUniform = useErosionNoiseShader();

	// ras_1
	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -8.0f);

	modelMatrix = translationMatrix;

	glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform3fv(sceneErosionNoiseUniform.laUniform, 1, lightAmbient);
	glUniform3fv(sceneErosionNoiseUniform.ldUniform, 1, lightDiffuse);
	glUniform3fv(sceneErosionNoiseUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneErosionNoiseUniform.lightPositionUniform, 1, lightPosition);

	glUniform3fv(sceneErosionNoiseUniform.kaUniform, 1, materialAmbient);
	glUniform3fv(sceneErosionNoiseUniform.kdUniform, 1, materialDiffuse);
	glUniform3fv(sceneErosionNoiseUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneErosionNoiseUniform.materialShininessUniform, materialShininess);

	glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion);
	glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras_1);
	//glUniform3fv(sceneErosionNoiseUniform.skyColorUniform, 1, skyColorForVeerRas);
	//glUniform3fv(sceneErosionNoiseUniform.cloudColorUniform, 1, cloudColorForVeerRas);
	//glUniform1f(sceneErosionNoiseUniform.noiseScaleUniform, noiseScale_erosion);
	//glUniform1i(sceneErosionNoiseUniform.uniform_enable_godRays, godRays);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_Marble_Shant);
	glUniform1i(sceneErosionNoiseUniform.textureSamplerUniform, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, noise_texture_eroded);
	glUniform1i(sceneErosionNoiseUniform.noiseSamplerUniform, 1);

	displayQuad();

	// ras_2
	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(-3.0f, 0.0f, -8.0f);

	modelMatrix = translationMatrix;

	glUniformMatrix4fv(sceneErosionNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneErosionNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneErosionNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform3fv(sceneErosionNoiseUniform.laUniform, 1, lightAmbient);
	glUniform3fv(sceneErosionNoiseUniform.ldUniform, 1, lightDiffuse);
	glUniform3fv(sceneErosionNoiseUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneErosionNoiseUniform.lightPositionUniform, 1, lightPosition);

	glUniform3fv(sceneErosionNoiseUniform.kaUniform, 1, materialAmbient);
	glUniform3fv(sceneErosionNoiseUniform.kdUniform, 1, materialDiffuse);
	glUniform3fv(sceneErosionNoiseUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneErosionNoiseUniform.materialShininessUniform, materialShininess);

	glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion);
	glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras_2);
	//glUniform3fv(sceneErosionNoiseUniform.skyColorUniform, 1, skyColorForVeerRas);
	//glUniform3fv(sceneErosionNoiseUniform.cloudColorUniform, 1, cloudColorForVeerRas);
	//glUniform1f(sceneErosionNoiseUniform.noiseScaleUniform, noiseScale_erosion);
	//glUniform1i(sceneErosionNoiseUniform.uniform_enable_godRays, godRays);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_Marble_Shant);
	glUniform1i(sceneErosionNoiseUniform.textureSamplerUniform, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, noise_texture_eroded);
	glUniform1i(sceneErosionNoiseUniform.noiseSamplerUniform, 1);

	displayQuad();

	// Unuse the shaderProgramObject
	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);
#endif // ENABLE_EROSION
}

void updateScene13_ShantRas(void)
{
#ifdef ENABLE_EROSION
	// update Cloud
	if (offset_ras_1[0] <= 0.48f)
		updateErosion(offsetIncrement, offset_ras_1, 0.001f);

	if (offset_ras_1[0] <= 0.33f)
		updateErosion(offsetIncrement, offset_ras_2, 0.001f);

	/*if (offsetIncrement == true)
	{
		offset[0] = offset[0] + 0.001f;
		offset[1] = offset[1] + 0.001f;
		offset[2] = offset[2] + 0.001f;
		if (offset[2] > 0.48f)
		{
			offsetIncrement = false;
		}
	}
	else
	{
		offset[0] = offset[0] - 0.001f;
		offset[1] = offset[1] - 0.001f;
		offset[2] = offset[2] - 0.001f;
		if (offset[2] < 0.17f)
		{
			offsetIncrement = true;
		}
	}*/

#endif // ENABLE_EROSION

}

void uninitializeScene13_Shant(void)
{
    //UNINIT models
	unloadStaticModel(&shantRoomModel);
	//unloadStaticModel(&deskModel);
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

