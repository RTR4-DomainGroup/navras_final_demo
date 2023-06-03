// This File Will Be Replaced by Scene*.cpp

//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/FSQuadShader.h"

#ifdef ENABLE_SHADOW
#include "../../inc/helper/shadowframebuffer.h"
#endif // ENABLE_SHADOW

#ifdef ENABLE_STARFIELD
#include "../../inc/effects/StarfieldEffect.h"
#endif // ENABLE_STARFIELD

#ifdef ENABLE_STATIC_MODELS
#include "../../inc/effects/StaticModelLoadingEffect.h"
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#endif // ENABLE_DYNAMIC_MODELS

#include "../../inc/scenes/scene08_BibhatsaRas.h"

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern GLfloat whiteSphere[]; // = {1.0f, 1.0f, 1.0f};
extern GLuint texture_Marble;
extern TEXTURE texture_grass;
extern TEXTURE texture_flower;

extern struct ADSUniform sceneOutdoorADSStaticUniform;
extern struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;

extern struct FSQuadUniform fsqUniform;

struct ADSUniform bibhatsaRasObject;

#ifdef ENABLE_SHADOW
// Shadow
extern ShadowFrameBufferDetails shadowFramebuffer;
extern mat4 viewmatrixDepth;
extern mat4 lightSpaceMatrix;
extern mat4 perspectiveProjectionDepth;
#endif // ENABLE_SHADOW

extern struct FrameBufferDetails fboBlackPass;
extern struct FrameBufferDetails fboColorPass;
extern struct FrameBufferDetails fboGodRayPass;

extern int windowWidth;
extern int windowHeight;

GLfloat lightAmbient_bibhatsa[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse_bibhatsa[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular_bibhatsa[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition_bibhatsa[] = { 10.0f, 10.0f, 0.0f, 1.0f };

GLfloat materialAmbient_bibhatsa[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse_bibhatsa[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular_bibhatsa[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess_bibhatsa = 128.0f;

extern mat4 viewMatrix;

extern mat4 perspectiveProjectionMatrix;

#ifdef ENABLE_STARFIELD
// Variables For Starfieldx
extern GLuint texture_star; 
extern double deltaTime;
extern struct StarfieldUniform sceneStarfieldUniform;
#endif // ENABLE_STARFIELD

#ifdef ENABLE_STATIC_MODELS
//Model variables
STATIC_MODEL manModel;
static STATIC_MODEL streetLightModel;
STATIC_MODEL buildingOneModel;
STATIC_MODEL buildingTwoModel;
STATIC_MODEL buildingThreeModel;
STATIC_MODEL buildingFourModel;
STATIC_MODEL buildingFiveModel;
STATIC_MODEL trashOneModel;

#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
static DYNAMIC_MODEL skeletonModel;
#endif // ENABLE_STATIC_MODELS

GLuint texture_road;
GLuint texture_footpath;
GLuint texture_wall;
GLuint texture_bibhatsaMask;

bool isInitialDisplayScene08_BibhatsaRas = true;

int initializeScene08_BibhatsaRas(void)
{
    // Code.
	// initializeCamera(&camera);

#ifdef ENABLE_STARFIELD
	initializeQuad();
	initializeCube();
	initializeCubeWithTilingTexcoords();

	if (LoadGLTexture_UsingSOIL(&texture_bibhatsaMask, TEXTURE_DIR"Masks\\BibhatsaMask.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_bibhatsaMask FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_bibhatsaMask Successfull = %u!!!\n", texture_bibhatsaMask);
	}

	if (LoadGLTexture_UsingSOIL(&texture_road, TEXTURE_DIR"Road\\road.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_road FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_road Successfull = %u!!!\n", texture_road);
	}

	if (LoadGLTexture_UsingSOIL(&texture_footpath, TEXTURE_DIR"Road\\foot.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_footpath FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_footpath Successfull = %u!!!\n", texture_footpath);
	}

	if (LoadGLTexture_UsingSOIL(&texture_wall, TEXTURE_DIR"Bibhatsa\\wall_side.png") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_wall FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_wall Successfull = %u!!!\n", texture_wall);
	}
#endif
	
#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/scene08-beebhatsa/man/tempBeebhatsaMan.obj", &manModel);
	loadStaticModel("res/models/streetLight/StreetLight.obj", &streetLightModel);

	// Buildings Model Loading
	loadStaticModel("res/models/scene08-beebhatsa/bld1/building1.obj", &buildingOneModel);
	loadStaticModel("res/models/scene08-beebhatsa/bld2/Building2.obj", &buildingTwoModel);
	loadStaticModel("res/models/scene08-beebhatsa/bld3/Building3.obj", &buildingThreeModel);
	loadStaticModel("res/models/scene08-beebhatsa/bld4/Building4_new.obj", &buildingFourModel);
	loadStaticModel("res/models/scene08-beebhatsa/bld5/Building5.obj", &buildingFiveModel);
	loadStaticModel("res/models/scene08-beebhatsa/trash1/trashCan1.obj", &trashOneModel);
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	//loadDynamicModel("res/models/skeleton/sadWalk.fbx", &skeletonModel);
	//loadDynamicModel("res/models/exo/Walking.dae", &skeletonModel);
	loadDynamicModel("res/models/man/man.fbx", &skeletonModel);
#endif // ENABLE_DYNAMIC_MODELS

	return 0;
}

void setCameraScene08(void)
{
	if (isInitialDisplayScene08_BibhatsaRas == true)
	{
		setCamera(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		isInitialDisplayScene08_BibhatsaRas = false;
	}
}

void displayScene08_Passes(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0) 
{
	// Function Declaration
	void depthQuadSceneCalls(int);

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
	displayCamera();
	//setCamera(&camera);

	mat4 finalViewMatrix = mat4::identity();

	if (actualDepthQuadScene == 0) 
	{
		finalViewMatrix = viewMatrix;
	} else if(actualDepthQuadScene == 1) 
	{
		finalViewMatrix = mat4::identity();
		finalViewMatrix = lookat(vec3(lightPosition_bibhatsa[0], lightPosition_bibhatsa[1], lightPosition_bibhatsa[2]), vec3(0.0f, -5.0f, -20.0f), vec3(0.0f, 1.0f, 0.0f));
		//finalViewMatrix = viewMatrix;

#ifdef ENABLE_SHADOW
		lightSpaceMatrix = mat4::identity();
		lightSpaceMatrix = perspectiveProjectionMatrix * finalViewMatrix;
		//lightSpaceMatrix = ortho(-30.0f, 30.0f, -30.0f, 30.0f, -30.0f, 30.0f);
		//lightSpaceMatrix = lightSpaceMatrix * finalViewMatrix;
#endif // ENABLE_SHADOW
	
	}

	if (actualDepthQuadScene == 0)
	{

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

		translationMatrix = vmath::translate(0.0f, 0.0f, -80.0f);					// glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

		//glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);
		glUniform1i(sceneStarfieldUniform.uniform_enable_godRays, godRays);
		glUniform1f(sceneStarfieldUniform.timeUniform, time);

		displayStarfield(texture_star);
		glUseProgram(0);

		// Road Drawing
		bibhatsaRasObject = useADSShader();

		// Transformations
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(-10.0f, -5.0f, -40.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(20.0f, 20.0f, 75.0f);
		rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(bibhatsaRasObject.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		depthQuadSceneCalls(actualDepthQuadScene);
		glUniformMatrix4fv(bibhatsaRasObject.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(bibhatsaRasObject.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1i(bibhatsaRasObject.uniform_enable_godRays, godRays);

		glUniformMatrix4fv(bibhatsaRasObject.laUniform, 1, GL_FALSE, lightAmbient_bibhatsa);
		glUniformMatrix4fv(bibhatsaRasObject.ldUniform, 1, GL_FALSE, lightDiffuse_bibhatsa);
		glUniformMatrix4fv(bibhatsaRasObject.lsUniform, 1, GL_FALSE, lightSpecular_bibhatsa);
		glUniformMatrix4fv(bibhatsaRasObject.lightPositionUniform, 1, GL_FALSE, lightPosition_bibhatsa);
		
		glUniformMatrix4fv(bibhatsaRasObject.kaUniform, 1, GL_FALSE, materialAmbient_bibhatsa);
		glUniformMatrix4fv(bibhatsaRasObject.kdUniform, 1, GL_FALSE, materialDiffuse_bibhatsa);
		glUniformMatrix4fv(bibhatsaRasObject.ksUniform, 1, GL_FALSE, materialSpecular_bibhatsa);
		glUniform1f(bibhatsaRasObject.materialShininessUniform, materialShininess_bibhatsa);		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_road);
		glUniform1i(bibhatsaRasObject.textureSamplerUniform_diffuse, 0);

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);

		// Transformations - Wall At The End Of The Road
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(-50.0f, 0.0f, -78.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(100.0f, 10.0f, 1.0f);
		//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(bibhatsaRasObject.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		depthQuadSceneCalls(actualDepthQuadScene);
		glUniformMatrix4fv(bibhatsaRasObject.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(bibhatsaRasObject.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_wall);
		glUniform1i(bibhatsaRasObject.textureSamplerUniform_diffuse, 0);
			displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);

		// Transformations - Quad For Mask
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(2.0f, 0.0f, -5.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
		//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(bibhatsaRasObject.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		depthQuadSceneCalls(actualDepthQuadScene);
		glUniformMatrix4fv(bibhatsaRasObject.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(bibhatsaRasObject.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_bibhatsaMask);
		glUniform1i(bibhatsaRasObject.textureSamplerUniform_diffuse, 0);
			displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);

		// Transformations - To Draw Footpath Left Side
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(-50.0f, -5.0f, -40.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(40.0f, 0.1f, 75.0f);
		rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(bibhatsaRasObject.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		depthQuadSceneCalls(actualDepthQuadScene);
		glUniformMatrix4fv(bibhatsaRasObject.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(bibhatsaRasObject.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1i(bibhatsaRasObject.uniform_enable_godRays, godRays);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_footpath);
		glUniform1i(bibhatsaRasObject.textureSamplerUniform_diffuse, 0);
			displayCubeWithTilingTexcoords();
		glBindTexture(GL_TEXTURE_2D, 0);

		// Transformations - To Draw Footpath On Right Side
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(10.0f, -5.0f, -40.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(40.0f, 0.1f, 75.0f);
		rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(bibhatsaRasObject.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		depthQuadSceneCalls(actualDepthQuadScene);
		glUniformMatrix4fv(bibhatsaRasObject.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(bibhatsaRasObject.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1i(bibhatsaRasObject.uniform_enable_godRays, godRays);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_footpath);
		glUniform1i(bibhatsaRasObject.textureSamplerUniform_diffuse, 0);
			displayCubeWithTilingTexcoords();
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);

#endif // ENABLE_STARFIELD
	}

#ifdef ENABLE_STATIC_MODELS
		//MODELS
		sceneOutdoorADSStaticUniform = useADSShader();

		// Sending Light Related Uniforms
		glUniform1i(sceneOutdoorADSStaticUniform.lightingEnableUniform, 1);
		glUniform4fv(sceneOutdoorADSStaticUniform.laUniform, 1, lightAmbient_bibhatsa);
		glUniform4fv(sceneOutdoorADSStaticUniform.ldUniform, 1, lightDiffuse_bibhatsa);
		glUniform4fv(sceneOutdoorADSStaticUniform.lsUniform, 1, lightSpecular_bibhatsa);
		glUniform4fv(sceneOutdoorADSStaticUniform.lightPositionUniform, 1, lightPosition_bibhatsa);
		glUniform4fv(sceneOutdoorADSStaticUniform.kaUniform, 1, materialAmbient_bibhatsa);
		glUniform4fv(sceneOutdoorADSStaticUniform.kdUniform, 1, materialDiffuse_bibhatsa);
		glUniform4fv(sceneOutdoorADSStaticUniform.ksUniform, 1, materialSpecular_bibhatsa);
		glUniform1f(sceneOutdoorADSStaticUniform.materialShininessUniform, materialShininess_bibhatsa);

		//normal mapping
		glUniform4fv(sceneOutdoorADSStaticUniform.viewpositionUniform, 1, camera.eye);

		glUniform1i(sceneOutdoorADSStaticUniform.uniform_enable_godRays, godRays);
		glUniform1i(sceneOutdoorADSStaticUniform.godrays_blackpass_sphere, 0);

		//glUniform1i(sceneOutdoorADSStaticUniform.)
		// ------ Man Model ------
		translationMatrix = vmath::translate(0.0f, -2.0f, 0.0f);
		scaleMatrix = vmath::scale(0.03f, 0.03f, 0.03f);
		rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix *  rotationMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		if (actualDepthQuadScene == 1)
		{
			glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 0);
			glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 1);
			glUniformMatrix4fv(sceneOutdoorADSStaticUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
		}
		else
		{
			glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 1);
			glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
		}

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(manModel);

		//translationMatrix = mat4::identity();
		//rotationMatrix = mat4::identity();
		//modelMatrix = mat4::identity();
		//scaleMatrix = mat4::identity();
		//rotationMatrix_x = mat4::identity();
		//rotationMatrix_y = mat4::identity();
		//rotationMatrix_z = mat4::identity();

		//// ------ Streetlight Model ------
		//translationMatrix = vmath::translate(4.0f, 0.0f, -6.0f);
		//scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

		//modelMatrix = translationMatrix * scaleMatrix;

		//glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		//glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		//glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		//drawStaticModel(streetLightModel);

		// ################################### BUILDING ONE ###################################  
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		// ------ Streetlight Model ------
		translationMatrix = vmath::translate(22.0f, 2.7f, -30.0f);
		scaleMatrix = vmath::scale(1.25f, 1.25f, 1.25f);
		rotationMatrix = vmath::rotate(-90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(buildingOneModel);
		// ################################### BUILDING ONE ###################################  

		// ################################### BUILDING TWO ###################################  
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		// ------ Streetlight Model ------
		translationMatrix = vmath::translate(-40.0f, -5.0f, -70.0f);
		scaleMatrix = vmath::scale(0.03f, 0.03f, 0.03f);
		rotationMatrix = vmath::rotate(-90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(buildingTwoModel);
		// ################################### BUILDING TWO ###################################  

		// ################################### BUILDING THREE ###################################  
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		// ------ Streetlight Model ------
		translationMatrix = vmath::translate(-15.0f, 2.0f, -50.0f);
		scaleMatrix = vmath::scale(0.07f, 0.07f, 0.07f);
		rotationMatrix = vmath::rotate(-90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(buildingThreeModel);
		// ################################### BUILDING THREE ###################################  

		// ################################### BUILDING FOUR ###################################  
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		// ------ Streetlight Model ------
		translationMatrix = vmath::translate(26.0f, 6.0f, -50.0f);
		scaleMatrix = vmath::scale(0.15f, 0.15f, 0.15f);
		rotationMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(buildingFourModel);
		// ################################### BUILDING FOUR ###################################  

		// ################################### BUILDING FIVE ###################################  
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		// ------ Streetlight Model ------
		translationMatrix = vmath::translate(25.0f, 5.0f, -65.0f);
		scaleMatrix = vmath::scale(0.05f, 0.05f, 0.05f);
		rotationMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(buildingFiveModel);
		// ################################### BUILDING FIVE ###################################  

		// ################################### BUILDING SIX ###################################  
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();

		// ------ Streetlight Model ------
		translationMatrix = vmath::translate(-10.0f, -4.0f, -25.0f);
		scaleMatrix = vmath::scale(0.1f, 0.1f, 0.1f);

		modelMatrix = translationMatrix * scaleMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(trashOneModel);
		// ################################### BUILDING SIX ###################################  

		// ################################### BUILDING SEVEN ###################################  
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		// ------ Streetlight Model ------
		translationMatrix = vmath::translate(-25.0f, 5.0f, -30.0f);
		scaleMatrix = vmath::scale(0.05f, 0.05f, 0.05f);
		rotationMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawStaticModel(buildingFiveModel);
		// ################################### BUILDING SEVEN ###################################  

		if (actualDepthQuadScene == 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// Un-use ShaderProgramObject
		glUseProgram(0);
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS

		glm::mat4 glm_modelMatrix;
		glm::mat4 glm_translateMatrix;
		glm::mat4 glm_rotateMatrix;
		glm::mat4 glm_scaleMatrix;

		glm_modelMatrix = glm::mat4(1.0f);
		glm_translateMatrix = glm::mat4(1.0f);
		glm_rotateMatrix = glm::mat4(1.0f);
		glm_scaleMatrix = glm::mat4(1.0f);

		sceneOutdoorADSDynamicUniform = useADSDynamicShader();

		// Sending Light Related Uniforms
		glUniform4fv(sceneOutdoorADSDynamicUniform.laUniform, 1, lightAmbient_bibhatsa);
		glUniform4fv(sceneOutdoorADSDynamicUniform.ldUniform, 1, lightDiffuse_bibhatsa);
		glUniform4fv(sceneOutdoorADSDynamicUniform.lsUniform, 1, lightSpecular_bibhatsa);
		glUniform4fv(sceneOutdoorADSDynamicUniform.lightPositionUniform, 1, lightPosition_bibhatsa);
		glUniform4fv(sceneOutdoorADSDynamicUniform.kaUniform, 1, materialAmbient_bibhatsa);
		glUniform4fv(sceneOutdoorADSDynamicUniform.kdUniform, 1, materialDiffuse_bibhatsa);
		glUniform4fv(sceneOutdoorADSDynamicUniform.ksUniform, 1, materialSpecular_bibhatsa);
		glUniform1f(sceneOutdoorADSDynamicUniform.materialShininessUniform, materialShininess_bibhatsa);

		glUniform1i(sceneOutdoorADSDynamicUniform.uniform_enable_godRays, godRays);
		glUniform1i(sceneOutdoorADSDynamicUniform.godrays_blackpass_sphere, 0);

		// ------ Dancing Vampire Model ------

		glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, -2.0f));
		glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.008f, 0.008f, 0.008f));
		//glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

		glm_modelMatrix = glm_translateMatrix * glm_scaleMatrix;

		glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(glm_modelMatrix));
		if (actualDepthQuadScene == 1)
		{
			glUniform1i(sceneOutdoorADSDynamicUniform.actualSceneUniform, 0);
			glUniform1i(sceneOutdoorADSDynamicUniform.depthSceneUniform, 1);
			glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
		}
		else
		{
			glUniform1i(sceneOutdoorADSDynamicUniform.actualSceneUniform, 1);
			glUniform1i(sceneOutdoorADSDynamicUniform.depthSceneUniform, 0);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
		}

		glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		drawDynamicModel(sceneOutdoorADSDynamicUniform, skeletonModel, 1.0f);

		glUseProgram(0);

#endif
}

void depthQuadSceneCalls(int actualDepthQuadScene)
{
	if (actualDepthQuadScene == 1)
	{
		glUniform1i(bibhatsaRasObject.actualSceneUniform, 0);
		glUniform1i(bibhatsaRasObject.depthSceneUniform, 1);
		glUniformMatrix4fv(bibhatsaRasObject.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
	}
	else
	{
		glUniform1i(bibhatsaRasObject.actualSceneUniform, 1);
		glUniform1i(bibhatsaRasObject.depthSceneUniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
	}
}

void updateScene08_BibhatsaRas(void)
{
	// Code
#ifdef ENABLE_CAMERA_ANIMATION
	cameraEyeZ = preciselerp(cameraEyeZ, -35.50f, 0.0002f);
	cameraCenterZ = preciselerp(cameraCenterZ, -41.00f, 0.002f);
#endif // ENABLE_CAMERA_ANIMATION

}

void uninitializeScene08_BibhatsaRas(void)
{
	if (texture_wall)
	{
		glDeleteTextures(1, &texture_wall);
		texture_wall = 0;
	}

	if(texture_footpath)
	{
		glDeleteTextures(1, &texture_footpath);
		texture_footpath = 0;
	}

	if(texture_road)
	{
		glDeleteTextures(1, &texture_road);
		texture_road = 0;
	}

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&manModel);
	unloadStaticModel(&streetLightModel);

	// Unloading Building Models
	unloadStaticModel(&trashOneModel);
	unloadStaticModel(&buildingFiveModel);
	unloadStaticModel(&buildingFourModel);
	unloadStaticModel(&buildingThreeModel);
	unloadStaticModel(&buildingTwoModel);
	unloadStaticModel(&buildingOneModel);

#endif // ENABLE_STATIC_MODELS


#ifdef ENABLE_DYNAMIC_MODELS
	unloadDynamicModel(&skeletonModel);
#endif
	//uninitializeCamera(&camera);

}

