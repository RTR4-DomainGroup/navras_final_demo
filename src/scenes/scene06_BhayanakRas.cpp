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

#ifdef ENABLE_WATER
#include "../../inc/helper/waterframebuffer.h"
#include "../../inc/effects/WaterEffect.h"
#endif // ENABLE_WATER

#ifdef ENABLE_STATIC_MODELS
#include "../../inc/effects/StaticModelLoadingEffect.h"
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_GAUSSIAN_BLUR
#include "../../inc/effects/GaussianBlurEffect.h"
#endif // ENABLE_GAUSSIAN_BLUR

#include "../../inc/scenes/scene06_BhayanakRas.h"

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern GLfloat whiteSphere[]; // = {1.0f, 1.0f, 1.0f};
extern GLuint texture_Marble;
extern TEXTURE texture_grass;
extern TEXTURE texture_flower;

extern struct ADSUniform sceneOutdoorADSStaticUniform;
extern struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;

extern struct FSQuadUniform fsqUniform;

#ifdef ENABLE_WATER
// Water Related Variables
extern struct WaterUniform waterUniform;
extern struct TextureVariables waterTextureVariables;
extern struct WaterFrameBufferDetails waterReflectionFrameBufferDetails;
extern struct WaterFrameBufferDetails waterRefractionFrameBufferDetails;
#endif // ENABLE_WATER

extern GLfloat waterHeight; // = 0.0f;
extern GLfloat moveFactor; // = 0.0f;
extern GLfloat planeReflection[]; // = { 0.0f, 1.0f, 0.0f, -waterHeight };
extern GLfloat planeRefration[]; // = { 0.0f, -1.0f, 0.0f, waterHeight };
extern struct FrameBufferDetails fboBlackPass;
extern struct FrameBufferDetails fboColorPass;
extern struct FrameBufferDetails fboGodRayPass;

extern int windowWidth;
extern int windowHeight;

extern float myScale; // = 1.0f;

extern float noiseScale; // = 2.0f;
extern bool noiseScaleIncrement; // = true;

extern mat4 viewMatrix;

extern GLfloat skyColor[]; // = { 0.0f, 0.0f, 0.8f, 0.0f };
extern GLfloat cloudColor[]; // = { 0.8f, 0.8f, 0.8f, 0.0f };

extern GLuint noise_texture;

extern GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static float displacementmap_depth;

#ifdef ENABLE_SHADOW
	// Shadow
	extern ShadowFrameBufferDetails shadowFramebuffer;
	extern mat4 viewmatrixDepth;
	extern mat4 lightSpaceMatrix;
	extern mat4 perspectiveProjectionDepth;
#endif // ENABLE_SHADOW

#ifdef ENABLE_STATIC_MODELS
//Model variables
static STATIC_MODEL rockModel;
static STATIC_MODEL roomModel;
static STATIC_MODEL roomModel2;
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
DYNAMIC_MODEL skeletonModel_06;
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_GAUSSIAN_BLUR
// Gaussian Blur related variables
extern struct GaussianBlurEffect gaussianBlurEffect;
extern struct HorrizontalBlurUniform horizontalBlurUniform;
extern struct VerticalBlurUniform verticalBlurUniform;
extern struct FrameBufferDetails fullSceneFbo;
extern struct FSQuadUniform fsGaussBlurQuadUniform;
#endif // ENABLE_GAUSSIAN_BLUR

extern GLfloat density; // = 0.15;
extern GLfloat gradient; // = 0.5;
extern GLfloat skyFogColor[]; // = { 0.25f, 0.25f, 0.25f, 1.0f };

// Camera angle for rotation
static GLfloat cameraAngle = 0.0f;
static GLfloat cameraRadius = 10.0f;
extern GLfloat dispersal; // = 0.1875f;
extern GLfloat haloWidth; // = 0.45f;
extern GLfloat intensity; // = 1.5f;
extern GLfloat distortion[]; // = { 0.94f, 0.97f, 1.0f };

static GLfloat lightAmbient_bhayanak[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightDiffuse_bhayanak[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightSpecular_bhayanak[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition_bhayanak[] = { 0.0f, 2.0f, 2.0f, 1.0f };

static GLfloat materialAmbient_bhayanak[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse_bhayanak[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialSpecular_bhayanak[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialShininess_bhayanak = 128.0f;

GLfloat cameraForeward = 0.0f;
GLfloat cameraLeft = 0.0f;
GLfloat cameraTop = 0.0f;

GLfloat cameraUpVector = 1.0f;

bool isInitialDisplayScene06_BhayanakRas = true;

GLuint texture_bhayanakMask;

// Time
extern time_t now;
extern time_t then;

int initializeScene06_BhayanakRas(void)
{
    // Code.

#ifdef ENABLE_MASKSQUADS
	initializeQuad();

	if (LoadGLTexture_UsingSOIL(&texture_bhayanakMask, TEXTURE_DIR"Masks\\BhayanakMask.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in Mask Bhayanak Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull Mask Bhayanak Ras = %u!!!\n", texture_bhayanakMask);
	}

#endif // ENABLE_MASKSQUADS

#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/scene06_bhayanak/boy/tempBhayanakKid2.obj", &rockModel);
	loadStaticModel("res/models/scene06_bhayanak/room/bhayanakRoom.obj", &roomModel);
	loadStaticModel("res/models/scene06_bhayanak/room/bhayanakRoomWithoutFloor.obj", &roomModel2);

#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	loadDynamicModel("res/models/man/man.fbx", &skeletonModel_06);
#endif // ENABLE_DYNAMIC_MODEL


	return 0;
}

void setCameraScene06_BhyanakRas(void)
{
	if (isInitialDisplayScene06_BhayanakRas == true)
	{
		
		setCamera(2.05f, 0.50f, 2.30f, -247.60f, 0.50f, -261.33f, 0.00f, 1.00f, 0.00f);
		isInitialDisplayScene06_BhayanakRas = false;
	}
}

void displayScene06_BhayanakRas(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0) {

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
	float distance;

	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	displayCamera();

	mat4 finalViewMatrix = mat4::identity();

	if (actualDepthQuadScene == 0) 
	{
		finalViewMatrix = viewMatrix;

	} 
	else if(actualDepthQuadScene == 1) 
	{
	
		finalViewMatrix = mat4::identity();
		finalViewMatrix = lookat(vec3(lightPosition_bhayanak[0], lightPosition_bhayanak[1], lightPosition_bhayanak[2]), vec3(0.0f, -5.0f, -20.0f), vec3(0.0f, 1.0f, 0.0f));
		//finalViewMatrix = viewMatrix;

#ifdef ENABLE_SHADOW
		lightSpaceMatrix = mat4::identity();
		lightSpaceMatrix = perspectiveProjectionMatrix * finalViewMatrix;
		
#endif // ENABLE_SHADOW
	
	}

	if (recordWaterReflectionRefraction == true) {

#ifdef ENABLE_WATER
		waterUniform = useWaterShader();

		
		if (isReflection == true) {
			distance = 2.0f * (cameraEyeY - waterHeight);
			glUniform4fv(waterUniform.planeUniform, 1, planeReflection);
			cameraEyeY -= distance;
			cameraCenterY -= distance;
			displayCamera();
			finalViewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
			glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		}

		if (isReflection == false) {
			glUniform4fv(waterUniform.planeUniform, 1, planeRefration);
			glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		}
#endif // ENABLE_WATER
	}

#ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneOutdoorADSStaticUniform = useADSShader();

	// Sending Light Related Uniforms
	glUniform1i(sceneOutdoorADSStaticUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneOutdoorADSStaticUniform.laUniform, 1, lightAmbient_bhayanak);
	glUniform4fv(sceneOutdoorADSStaticUniform.ldUniform, 1, lightDiffuse_bhayanak);
	glUniform4fv(sceneOutdoorADSStaticUniform.lsUniform, 1, lightSpecular_bhayanak);
	glUniform4fv(sceneOutdoorADSStaticUniform.lightPositionUniform, 1, lightPosition_bhayanak);
	glUniform4fv(sceneOutdoorADSStaticUniform.kaUniform, 1, materialAmbient_bhayanak);
	glUniform4fv(sceneOutdoorADSStaticUniform.kdUniform, 1, materialDiffuse_bhayanak);
	glUniform4fv(sceneOutdoorADSStaticUniform.ksUniform, 1, materialSpecular_bhayanak);
	glUniform1f(sceneOutdoorADSStaticUniform.materialShininessUniform, materialShininess_bhayanak);

	glUniform1i(sceneOutdoorADSStaticUniform.isInstanced, 0);

	glUniform1f(sceneOutdoorADSStaticUniform.colorCorrectionUniform, 0.3f);

	glUniform1f(sceneOutdoorADSStaticUniform.blackOrWhiteRoomUniform, 0.0f);
	glUniform1f(sceneOutdoorADSStaticUniform.blackOrWhiteRoomMixDeltaUniform, 0.0f);
	glUniform1f(sceneOutdoorADSStaticUniform.ssaoIntensityDeltaUniform, 0.7f);

	//normal mapping
	glUniform4fv(sceneOutdoorADSStaticUniform.viewpositionUniform, 1, camera.eye);

	glUniform1i(sceneOutdoorADSStaticUniform.uniform_enable_godRays, godRays);
	glUniform1i(sceneOutdoorADSStaticUniform.godrays_blackpass_sphere, 0);


	glUniform1i(sceneOutdoorADSStaticUniform.isInstanced, 0);

	// ------ BOy Model ------
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;
	glUniform1i(sceneOutdoorADSStaticUniform.isInstanced, 0);
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

	drawStaticModel(rockModel);

	// ------ Room Model ------
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	translationMatrix = vmath::translate(0.0f, 1.28f, 0.0f);
	scaleMatrix = vmath::scale(0.1f, 0.1f, 0.1f);

	modelMatrix = translationMatrix * scaleMatrix;

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

	if(now >= (then + 10)){

		glUniform1f(sceneOutdoorADSStaticUniform.blackOrWhiteRoomMixDeltaUniform, 0.75f);
		drawStaticModel(roomModel2);
	}
	else{
		drawStaticModel(roomModel);
	}
#ifdef ENABLE_MASKSQUADS
	// Transformations For Mask Quad
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 1.0f, -2.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_bhayanakMask);
	glUniform1i(sceneOutdoorADSStaticUniform.textureSamplerUniform_diffuse, 0);
	displayQuad();
#endif

	if (actualDepthQuadScene == 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	glUniform1f(sceneOutdoorADSStaticUniform.blackOrWhiteRoomMixDeltaUniform, 0.0f);
	// Un-use ShaderProgramObject
	glUseProgram(0);

#endif // ENABLE_WATER

#ifdef ENABLE_DYNAMIC_MODELS

#endif

#ifdef ENABLE_WATER

	if (now >= (then + 10)) {

		if(waterDraw == true){
			waterUniform = useWaterShader();

			translationMatrix = mat4::identity();
			scaleMatrix = mat4::identity();
			modelMatrix = mat4::identity();

			translationMatrix = vmath::translate(0.0f, 0.0f, -20.0f);

			scaleMatrix = vmath::scale(180.0f, 1.0f,180.0f);

			modelMatrix = translationMatrix * scaleMatrix;

			glUniformMatrix4fv(waterUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
			glUniformMatrix4fv(waterUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, waterReflectionFrameBufferDetails.frameBufferTexture);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, waterRefractionFrameBufferDetails.frameBufferTexture);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, waterTextureVariables.displacement);

			glUniform1f(waterUniform.moveFactorUniform, moveFactor);

			glUniform1f(waterUniform.uniform_waveStrength, 0.0f);
			glUniform4fv(waterUniform.uniform_watercolor, 1, vec4(0.0f, 0.0f, 0.0f, 1.0));

			glUniform1f(waterUniform.uniform_enable_godRays, godRays);

			displayWater();

			glBindTexture(GL_TEXTURE_2D, 0);
			glUseProgram(0);
		}

	}

	if (isReflection == true) {

		cameraEyeY += distance;
		cameraCenterY += distance;
		displayCamera();
		finalViewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	}

#endif // ENABLE_WATER

}

void updateScene06_BhayanakRas(void)
{
	// Code

#ifdef ENABLE_CAMERA_ANIMATION
	
	cameraEyeX = preciselerp(cameraEyeX, 0.70f, 0.002f);
	cameraEyeY = preciselerp(cameraEyeY, 0.25f, 0.002f);
	cameraEyeZ = preciselerp(cameraEyeZ, 0.95f, 0.002f);

	cameraCenterX = preciselerp(cameraCenterX, -248.95f, 0.002f);
	cameraCenterY = preciselerp(cameraCenterY, 0.25f, 0.002f);
	cameraCenterZ = preciselerp(cameraCenterZ, -262.68f, 0.002f);

#endif // ENABLE_CAMERA_ANIMATION


#ifdef ENABLE_WATER

#endif // ENABLE_WATER
}

void uninitializeScene06_BhayanakRas(void)
{
	// Code

#ifdef ENABLE_MASKSQUADS
	if (texture_bhayanakMask)
	{
		glDeleteTextures(1, &texture_bhayanakMask);
		texture_bhayanakMask = 0;
	}
#endif

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&rockModel);

#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	unloadDynamicModel(&skeletonModel_06);
#endif

}

