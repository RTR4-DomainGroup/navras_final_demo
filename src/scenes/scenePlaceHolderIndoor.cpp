// This File Will Be Replaced by Scene*.cpp

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/waterframebuffer.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/ssaoframebuffer.h"

#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/ADSLightDynamicShader.h"
#include "../../inc/shaders/BillboardingShader.h"
#include "../../inc/shaders/SSAOShader.h"


#include "../../inc/effects/videoEffect.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/CloudEffect.h"
#include "../../inc/effects/WaterEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
#include "../../inc/effects/GaussianBlurEffect.h"
#include "../../inc/effects/SSAOEffect.h"


#include "../../inc/scenes/scenePlaceHolderIndoor.h"


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT


extern int windowWidth;
extern int windowHeight;

// SSAO variable
SSAOFrameBufferStruct ssaoFrameBufferDetails;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

struct ADSUniform sceneIndoorADSUniform;
struct ADSDynamicUniform sceneIndoorADSDynamicUniform;

static GLfloat density = 0.15;
static GLfloat gradient = 0.5;
static GLfloat skyFogColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };

static GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 10.0f, 10.0f, 0.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

static mat4 viewMatrix;

// //Model variables
// static STATIC_MODEL rockModel;
// static STATIC_MODEL streetLightModel;
// static STATIC_MODEL deskModel;
// static STATIC_MODEL schoolBagModel;
// static DYNAMIC_MODEL skeletonModel;

int initializeScene_PlaceHolderIndoor(void)
{
	// Function Declarations

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


#ifdef ENABLE_SSAO

	if (ssaoCreateFBO(&ssaoFrameBufferDetails) == GL_FALSE) 
	{
		LOG("ssaoCreateFBO() For Shadow FAILED!!!\n");
		return(-1);
	}
	else 
	{
		LOG("ssaoCreateFBO() Successfull for Shadow!!!\n");
	}

#endif // ENABLE_SSAO


// #ifdef ENABLE_STATIC_MODELS
// 	//load models
// 	loadStaticModel("res/models/rock/rock.obj", &rockModel);
// 	loadStaticModel("res/models/streetLight/StreetLight.obj", &streetLightModel);
// 	loadStaticModel("res/models/desk/desk.obj", &deskModel);
// 	loadStaticModel("res/models/schoolBag/schoolBag.fbx", &schoolBagModel);
// #endif // ENABLE_STATIC_MODELS


// #ifdef ENABLE_DYNAMIC_MODELS
// 	//loadDynamicModel("res/models/skeleton/sadWalk.fbx", &skeletonModel);
// 	//loadDynamicModel("res/models/exo/Walking.dae", &skeletonModel);
// 	loadDynamicModel("res/models/man/man.fbx", &skeletonModel);
// #endif


	return 0;
}

void displayScene_PlaceHolderIndoor(SET_CAMERA setCamera, DISPLAY_PASSES_INDOOR displayPasses)
{
	// Function Declarations
	void displayGodRays(int, int);

	// Code
	// Here The Game STarts
	setCamera();

	// set camera
	displayCamera();

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	viewMatrix = mat4::identity();

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);

#ifdef ENABLE_SSAO
	
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFrameBufferDetails.render_fbo);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	displayPasses();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#endif // ENABLE_SSAO

	//Default Frame Buffer
	
	//glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
	//perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneIndoorADSUniform = useADSShader();

	// Sending Light Related Uniforms
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

	//For Normal Mapping
	glUniform4fv(sceneIndoorADSUniform.viewpositionUniform, 1, camera.eye);

	// ------ Rock Model ------
	translationMatrix = vmath::translate(-1.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// drawStaticModel(rockModel);

	// ------ Streetlight Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	
	translationMatrix = vmath::translate(1.0f, -2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// drawStaticModel(streetLightModel);

	// ------ SchoolBag Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();


	translationMatrix = vmath::translate(1.0f, 0.0f, -1.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// drawStaticModel(schoolBagModel);


	// ------ Desk Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(-1.0f, -2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// drawStaticModel(deskModel);

	// Un-use ShaderProgramObject
	glUseProgram(0);
#endif


#ifdef ENABLE_DYNAMIC_MODELS

	glm::mat4 glm_modelMatrix;
	glm::mat4 glm_translateMatrix;
	glm::mat4 glm_rotateMatrix;
	glm::mat4 glm_scaleMatrix;

	glm_modelMatrix = glm::mat4(1.0f);
	glm_translateMatrix = glm::mat4(1.0f);
	glm_rotateMatrix = glm::mat4(1.0f);
	glm_scaleMatrix = glm::mat4(1.0f);

	sceneIndoorADSDynamicUniform = useADSDynamicShader();

	// Sending Light Related Uniforms
	glUniform4fv(sceneIndoorADSDynamicUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneIndoorADSDynamicUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneIndoorADSDynamicUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneIndoorADSDynamicUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneIndoorADSDynamicUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneIndoorADSDynamicUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneIndoorADSDynamicUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneIndoorADSDynamicUniform.materialShininessUniform, materialShininess);

	glUniform1i(sceneIndoorADSDynamicUniform.fogEnableUniform, 0);
	glUniform1f(sceneIndoorADSDynamicUniform.densityUniform, density);
	glUniform1f(sceneIndoorADSDynamicUniform.gradientUniform, gradient);
	glUniform4fv(sceneIndoorADSDynamicUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneIndoorADSDynamicUniform.uniform_enable_godRays, 1);
	glUniform1i(sceneIndoorADSDynamicUniform.godrays_blackpass_sphere, 1);

	// ------ Dancing Vampire Model ------

	glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -2.0f, -2.0f));
	glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.02f, 0.02f, 0.02f));
	//glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glm_modelMatrix = glm_translateMatrix * glm_scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSDynamicUniform.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(glm_modelMatrix));
	glUniformMatrix4fv(sceneIndoorADSDynamicUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSDynamicUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// drawDynamicModel(sceneIndoorADSDynamicUniform, skeletonModel, 0.2f);

	glUseProgram(0);

#endif


}

void updateScene_PlaceHolderIndoor(void)
{
	// Code
//#ifdef ENABLE_ADSLIGHT
//    angleCube = angleCube + 1.0f;
//	if (angleCube >= 360.0f)
//	{
//		angleCube -= 360.0f;
//	}
//
//#endif // ENABLE_ADSLIGHT


	//update camera using lerp
	/*cameraEyeY = preciselerp(cameraEyeY, 25.0f, 0.01f);
	cameraCenterY = preciselerp(cameraCenterY, 25.0f, 0.01f);*/
}

void uninitializeScene_PlaceHolderIndoor(void)
{
	// Code

#ifdef ENABLE_ADSLIGHT
	if (texture_Marble)
	{
		glDeleteTextures(1, &texture_Marble);
		texture_Marble = NULL;
	}

	// uninitializeSphere();
	// uninitializeTriangle();
	// uninitializeQuad();
	// uninitializePyramid();
	uninitializeCube();

#endif // ENABLE_ADSLIGHT

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	// unloadStaticModel(&schoolBagModel);
	// unloadStaticModel(&deskModel);
	// unloadStaticModel(&streetLightModel);
	// unloadStaticModel(&rockModel);
#endif


#ifdef ENABLE_DYNAMIC_MODELS
	// unloadDynamicModel(&skeletonModel);
#endif
}


