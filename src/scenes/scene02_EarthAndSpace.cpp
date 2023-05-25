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

#include "../../inc/scenes/scene02_EarthAndSpace.h"

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT


extern GLfloat whiteSphere[]; // = {1.0f, 1.0f, 1.0f};
extern GLuint texture_Marble;
GLuint texture_earth;
GLuint texture_sun;

extern struct ADSUniform sceneOutdoorADSStaticUniform;
extern struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;
extern struct FSQuadUniform fsqUniform;

#ifdef ENABLE_SHADOW
// Shadow
extern ShadowFrameBufferDetails shadowFramebuffer;
extern mat4 viewmatrixDepth;
extern mat4 lightSpaceMatrix;
extern mat4 perspectiveProjectionDepth;
#endif // ENABLE_SHADOW

//extern GLfloat waterHeight; // = 0.0f;
//extern GLfloat moveFactor; // = 0.0f;
//extern GLfloat planeReflection[]; // = { 0.0f, 1.0f, 0.0f, -waterHeight };
//extern GLfloat planeRefration[]; // = { 0.0f, -1.0f, 0.0f, waterHeight };

//extern struct FrameBufferDetails fboBlackPass;
//extern struct FrameBufferDetails fboColorPass;
//extern struct FrameBufferDetails fboGodRayPass;

extern struct FrameBufferDetails fboEarthAndSpace;

extern int windowWidth;
extern int windowHeight;

//extern float myScale; // = 1.0f;
//
//extern float noiseScale; // = 2.0f;
//extern bool noiseScaleIncrement; // = true;

extern mat4 viewMatrix;

extern GLfloat skyColor[]; // = { 0.0f, 0.0f, 0.8f, 0.0f };
extern GLfloat cloudColor[]; // = { 0.8f, 0.8f, 0.8f, 0.0f };

//extern GLuint noise_texture;

//extern GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;


#ifdef ENABLE_STARFIELD
// Variables For Starfieldx
extern GLuint texture_star; 
extern double deltaTime;
extern struct StarfieldUniform sceneStarfieldUniform;
#endif // ENABLE_STARFIELD

struct ADSUniform adsEarthAndSpaceUniform;

extern GLfloat density; // = 0.15;
extern GLfloat gradient; // = 0.5;
extern GLfloat skyFogColor[]; // = { 0.25f, 0.25f, 0.25f, 1.0f };

// Camera angle for rotation
extern GLfloat cameraAngle; // = 0.0f;
extern GLfloat dispersal; // = 0.1875f;
extern GLfloat haloWidth; // = 0.45f;
extern GLfloat intensity; // = 1.5f;
extern GLfloat distortion[]; // = { 0.94f, 0.97f, 1.0f };


#ifdef ENABLE_STATIC_MODELS
//Model variables
static STATIC_MODEL rockModel;
static STATIC_MODEL treeModel;
static DYNAMIC_MODEL skeletonModel;

#endif // ENABLE_STATIC_MODELS

//static struct TextureVariables terrainTextureVariables;
//static float displacementmap_depth;

static GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 4.0f, 3.0f, 3.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

//float distance10;

int initializeScene02_EarthAndSpace(void)
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
	// initializeCamera(&camera);

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

#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/rock/rock.obj", &rockModel);
	loadStaticModel("res/models/tree_adbhut/tree.fbx", &treeModel);
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	//loadDynamicModel("res/models/skeleton/sadWalk.fbx", &skeletonModel);
	//loadDynamicModel("res/models/exo/Walking.dae", &skeletonModel);
	loadDynamicModel("res/models/man/man.fbx", &skeletonModel);
#endif // ENABLE_DYNAMIC_MODELS

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

	if (LoadGLTexture_UsingSOIL(&texture_earth, TEXTURE_DIR"marble.bmp") == FALSE)
	{
		//uninitialize();
		LOG("Starfield LoadGLTexture for Earth FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture for Earth is Successful = %u!!!\n", texture_earth);
	}

	if (LoadGLTexture_UsingSOIL(&texture_sun, TEXTURE_DIR"Starfield/Sun.png") == FALSE)
	{
		//uninitialize();
		LOG("Starfield LoadGLTexture for Sun FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture for Sun is Successful = %u!!!\n", texture_sun);
	}

	initializeQuad();
#endif // ENABLE_STARFIELD

	return 0;
}

void displayScene02_EarthAndSpace(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0)
{
	// Code
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	setCamera();
	//setCamera(&camera);

	mat4 finalViewMatrix = mat4::identity();

	if (actualDepthQuadScene == 0) 
	{
		finalViewMatrix = viewMatrix;
	} 
	else if(actualDepthQuadScene == 1) 
	{

		finalViewMatrix = mat4::identity();
		finalViewMatrix = lookat(vec3(lightPosition[0], lightPosition[1], lightPosition[2]), vec3(0.0f, -5.0f, -20.0f), vec3(0.0f, 1.0f, 0.0f));
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

		adsEarthAndSpaceUniform = useADSShader();

		glUniformMatrix4fv(adsEarthAndSpaceUniform.laUniform, 1, GL_FALSE, lightAmbient);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.ldUniform, 1, GL_FALSE, lightDiffuse);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.lsUniform, 1, GL_FALSE, lightSpecular);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.lightPositionUniform, 1, GL_FALSE, lightPosition);

		glUniformMatrix4fv(adsEarthAndSpaceUniform.kaUniform, 1, GL_FALSE, materialAmbient);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.kdUniform, 1, GL_FALSE, materialDiffuse);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.ksUniform, 1, GL_FALSE, materialSpecular);
		glUniform1f(adsEarthAndSpaceUniform.materialShininessUniform, materialShininess);

		glUniform1i(adsEarthAndSpaceUniform.uniform_enable_godRays, godRays);

		glUniform1f(adsEarthAndSpaceUniform.gradientUniform, gradient);
		glUniform1f(adsEarthAndSpaceUniform.densityUniform, density);

		glUniform1i(adsEarthAndSpaceUniform.fogEnableUniform, 0);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.skyFogColorUniform, 1, GL_FALSE, skyFogColor);
		glUniform1i(adsEarthAndSpaceUniform.godrays_blackpass_sphere, 0);

		// Earth
		translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);					// glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(adsEarthAndSpaceUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_earth);
		//glUniform1i(fboEarthAndSpace.textureSamplerUniform_diffuse, 0);

		float color[3] = { 0.0f, 0.0f, 0.0f };
		if (godRays == 1)
		{
			color[0] = 1.0f;
			color[1] = 1.0f;
			color[2] = 1.0f;
		}
		displaySphere(color);

		glBindTexture(GL_TEXTURE_2D, 0);

		// Cube
		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(200.0f, 200.0f, 200.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(adsEarthAndSpaceUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboEarthAndSpace.frameBufferTexture);
		glUniform1i(adsEarthAndSpaceUniform.textureSamplerUniform_diffuse, 0);

		displayCube();
		glBindTexture(GL_TEXTURE_2D, 0);

		// Sun
	// Sun
		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.0f, 20.0f, -100.0f);					// glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(0.0f, 10.0f, -100.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(adsEarthAndSpaceUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		if (actualDepthQuadScene == 1)
		{
			glUniform1i(adsEarthAndSpaceUniform.actualSceneUniform, 0);
			glUniform1i(adsEarthAndSpaceUniform.depthSceneUniform, 1);
			glUniformMatrix4fv(adsEarthAndSpaceUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
		}
		else
		{
			glUniform1i(adsEarthAndSpaceUniform.actualSceneUniform, 1);
			glUniform1i(adsEarthAndSpaceUniform.depthSceneUniform, 0);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
		}

		glUniformMatrix4fv(adsEarthAndSpaceUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(adsEarthAndSpaceUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_sun);
		glUniform1i(adsEarthAndSpaceUniform.textureSamplerUniform_diffuse, 0);

		displayCube();
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);


#endif // ENABLE_STARFIELD

	}


#ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneOutdoorADSStaticUniform = useADSShader();

	// Sending Light Related Uniforms
	glUniform1i(sceneOutdoorADSStaticUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneOutdoorADSStaticUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneOutdoorADSStaticUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneOutdoorADSStaticUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneOutdoorADSStaticUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneOutdoorADSStaticUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneOutdoorADSStaticUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneOutdoorADSStaticUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneOutdoorADSStaticUniform.materialShininessUniform, materialShininess);

	//normal mapping
	glUniform4fv(sceneOutdoorADSStaticUniform.viewpositionUniform, 1, camera.eye);

	glUniform1i(sceneOutdoorADSStaticUniform.fogEnableUniform, 1);
	glUniform1f(sceneOutdoorADSStaticUniform.densityUniform, density);
	glUniform1f(sceneOutdoorADSStaticUniform.gradientUniform, gradient);
	glUniform4fv(sceneOutdoorADSStaticUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneOutdoorADSStaticUniform.uniform_enable_godRays, godRays);
	glUniform1i(sceneOutdoorADSStaticUniform.godrays_blackpass_sphere, 0);

	//glUniform1i(sceneOutdoorADSStaticUniform.)
	// ------ Rock Model ------
	translationMatrix = vmath::translate(2.0f, 2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	if (actualDepthQuadScene == 1) {

		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 0);
		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 1);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);

	} else {

		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 1);
		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);

	}

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(rockModel);

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	// ------ Streetlight Model ------
	translationMatrix = vmath::translate(-8.0f, -3.60f, -17.00f);
	scaleMatrix = vmath::scale(0.31f, 0.31f, 0.31f);

	// update_transformations(translationMatrix, scaleMatrix, rotationMatrix) ;
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);


	drawStaticModel(treeModel);

	if (actualDepthQuadScene == 0) {
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
	glUniform4fv(sceneOutdoorADSDynamicUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneOutdoorADSDynamicUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneOutdoorADSDynamicUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneOutdoorADSDynamicUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneOutdoorADSDynamicUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneOutdoorADSDynamicUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneOutdoorADSDynamicUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneOutdoorADSDynamicUniform.materialShininessUniform, materialShininess);

	glUniform1i(sceneOutdoorADSDynamicUniform.fogEnableUniform, 0);
	glUniform1f(sceneOutdoorADSDynamicUniform.densityUniform, density);
	glUniform1f(sceneOutdoorADSDynamicUniform.gradientUniform, gradient);
	glUniform4fv(sceneOutdoorADSDynamicUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneOutdoorADSDynamicUniform.uniform_enable_godRays, godRays);
	glUniform1i(sceneOutdoorADSDynamicUniform.godrays_blackpass_sphere, 0);


	glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 1.0f, -2.0f));
	glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.008f, 0.008f, 0.008f));
	//glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));

	glm_modelMatrix = glm_translateMatrix * glm_scaleMatrix;

	glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(glm_modelMatrix));
	if (actualDepthQuadScene == 1) {

		glUniform1i(sceneOutdoorADSDynamicUniform.actualSceneUniform, 0);
		glUniform1i(sceneOutdoorADSDynamicUniform.depthSceneUniform, 1);
		glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);

	}
	else {

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

void updateScene02_EarthAndSpace(void)
{
	// Code
#ifdef ENABLE_STARFIELD
	deltaTime = updateStarfield(deltaTime);
#endif

}

void uninitializeScene02_EarthAndSpace(void)
{
	// Code
#ifdef ENABLE_STARFIELD
	if (texture_sun)
	{
		glDeleteTextures(1, &texture_sun);
		texture_sun = NULL;
	}

	if (texture_earth)
	{
		glDeleteTextures(1, &texture_earth);
		texture_earth = NULL;
	}

	uninitializeStarfield(texture_star);
#endif // ENABLE_STARFIELD

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&rockModel);
	unloadStaticModel(&treeModel);
#endif // ENABLE_STATIC_MODELS


#ifdef ENABLE_DYNAMIC_MODELS
	unloadDynamicModel(&skeletonModel);
#endif
	//uninitializeCamera(&camera);

}



