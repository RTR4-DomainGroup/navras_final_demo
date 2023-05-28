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


int initializeScene13_Shant(void)
{
#ifdef ENABLE_STATIC_MODELS
	// function declarations

	loadStaticModel("res/models/scene13_shanta/room/shantaRoom2.obj", &shantRoomModel);
	
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
	return 0;
}


void displayScene13_Shant(void)
{
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
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(shantRoomModel);
	// ################################### BUILDING ONE ###################################  


	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
    #endif 
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
}

