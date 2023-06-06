#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../inc/shaders/BillboardingShader.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
#include "../../inc/effects/GaussianBlurEffect.h"
#include "../../inc/scenes/scenePlaceHolderIndoor.h"
#include "../../inc/scenes/scene05_karun.h"
#include "../../inc/debug/debug_transformation.h"


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static struct ADSUniform sceneIndoorADSUniform;
GLuint texture_karunMask;

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

GLuint texture_kidroom_ceiling;
GLuint texture_kidroom_floor;
GLuint texture_kidroom_side;
GLuint texture_kidroom_back;

//Model variables
STATIC_MODEL tableModel;

GLuint textures_kidroom[4];

//values
scene5_variables translatevalues;

int initializeScene5_karun(void)
{

#ifdef ENABLE_MASKSQUADS
	if (LoadGLTexture_UsingSOIL(&texture_karunMask, TEXTURE_DIR"Masks\\KarunMask.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in Mask Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull Mask Karun Ras = %u!!!\n", texture_karunMask);
	}


#endif // ENABLE_MASKSQUADS

#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/kid-table/kid-tableOBJ.obj", &tableModel);

	if (LoadGLTexture_UsingSOIL(&texture_kidroom_ceiling, TEXTURE_DIR"Room/scene5Room/ceiling.png") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_ceiling);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_kidroom_floor, TEXTURE_DIR"Room/floor.jpg") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in floor Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_floor);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_kidroom_back, TEXTURE_DIR"Room/scene5Room/frontwall2.png") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in back Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_back);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_kidroom_side, TEXTURE_DIR"Room/scene5Room/sidewalls.png") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in sidewall Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_side);
	}
	//loadStaticModel("res/models/schoolBag/schoolBag.fbx", &schoolBagModel);
#endif
	initializeInvertedNormalCube();
	initializeQuad();

	textures_kidroom[0] = (GLuint)texture_kidroom_ceiling;
	textures_kidroom[1] = (GLuint)texture_kidroom_floor;
	textures_kidroom[2] = (GLuint)texture_kidroom_back;
	textures_kidroom[3] = (GLuint)texture_kidroom_side;
//	glEnable(GL_TEXTURE_2D);
	return 0;
}

void displayScene5_karun(void)
{
    // set camera
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
	glUniform1i(sceneIndoorADSUniform.instancingEnabled, 0);

	translationMatrix = vmath::translate(0.0f, 0.0f, -1.0f);
	scaleMatrix = vmath::scale(4.0f, 2.0f, 5.0f);
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	//glEnable(GL_TEXTURE_2D);	
	displayRoom(textures_kidroom);
	glBindTexture(GL_TEXTURE_2D, 0);

#ifdef ENABLE_MASKSQUADS
	// Transformations For Mask Quad
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
	scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
	//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_karunMask);
	glUniform1i(sceneIndoorADSUniform.textureSamplerUniform_diffuse, 0);
		displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
#endif // ENABLE_MASKSQUADS

    // ------ Desk Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	//translatevalues.x = 1.799999 , translatevalues.y = -1.000000, translatevalues.z = -3.299998 
	translationMatrix = vmath::translate(1.799999f, -1.000000f, -3.299998f);
	//translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	//LOG(" translatevalues.x = %f , translatevalues.y = %f, translatevalues.z = %f \n", tf_t.x, tf_t.y, tf_t.z);
	scaleMatrix = vmath::scale(0.5f, 0.4f, 0.35f);
	//rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix ;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(tableModel);

	// Un-use ShaderProgramObject
	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
    #endif 
}

void uninitializeScene5_karun(void)
{
    //UNINIT models
	unloadStaticModel(&tableModel);

#ifdef ENABLE_MASKSQUADS
	if (texture_karunMask)
	{
		glDeleteTextures(1, &texture_karunMask);
		texture_karunMask = 0;
	}
#endif

	if (texture_kidroom_ceiling)
	{
		glDeleteTextures(1, &texture_kidroom_ceiling);
		texture_kidroom_ceiling = 0;
	}
	if (texture_kidroom_floor)
	{
		glDeleteTextures(1, &texture_kidroom_floor);
		texture_kidroom_floor = 0;
	}
	if (texture_kidroom_back)
	{
		glDeleteTextures(1, &texture_kidroom_back);
		texture_kidroom_floor = 0;
	}
	if (texture_kidroom_side)
	{
		glDeleteTextures(1, &texture_kidroom_side);
		texture_kidroom_side = 0;
	}
	
}
