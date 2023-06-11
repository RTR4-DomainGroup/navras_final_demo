// This File Will Be Replaced by Scene*.cpp

#include "../../inc/helper/texture_loader.h"

#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/ssaoframebuffer.h"

#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/SSAOShader.h"

#include "../../inc/effects/GaussianBlurEffect.h"
#include "../../inc/effects/SSAOEffect.h"


#include "../../inc/scenes/scenePlaceHolderIndoor.h"


#include "../../inc/effects/StaticModelLoadingEffect.h"

#ifdef ENABLE_EROSION
#include "../../inc/effects/ErosionEffect.h"
#endif // ENABLE_EROSION

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

scene_types_t  getCurrentScene(void);

// SSAO variable
SSAOFrameBufferStruct ssaoFrameBufferDetails;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;


#ifdef ENABLE_GAUSSIAN_BLUR
// Gaussian Blur related variables
static struct GaussianBlurEffect gaussianBlurIndoorEffect;
static struct HorrizontalBlurUniform horizontalBlurUniform;
static struct VerticalBlurUniform verticalBlurIndoorUniform;
static struct FrameBufferDetails fullSceneIndoorFbo;
static struct FSQuadUniform fsGaussBlurIndoorQuadUniform;
#endif // ENABLE_GAUSSIAN_BLUR

// MASKS
static struct FrameBufferDetails fboMaskPass_Indoor;
static GLuint texture_mask_indoor;

static STATIC_MODEL maskModel_KarunRas;
static STATIC_MODEL maskModel_RaudraRas;
static STATIC_MODEL maskModel_HasyaRas;

extern GLfloat lightAmbient_shantRas_mask[];
extern GLfloat lightDiffuse_shantRas_mask[];
extern GLfloat lightSpecular_shantRas_mask[];
extern GLfloat lightPosition_shantRas_mask[];

extern GLfloat materialAmbient_shantRas_mask[];
extern GLfloat materialDiffuse_shantRas_mask[];
extern GLfloat materialSpecular_shantRas_mask[];
extern GLfloat materialShininess_shantRas_mask;
extern struct ErosionNoiseUniform sceneErosionNoiseUniform;
static GLuint noise_texture_eroded_indoor;
static float myScale_erosion_indoor = 2.0f;
static float noiseScale_erosion_indoor = 2.0f;
static bool offsetIncrement_indoor = false;
static bool isBlurI = false;

static GLfloat offset_ras_indoor[] = { 0.17f, 0.17f, 0.17f };

int initializeScene_PlaceHolderIndoor(void)
{
	// Function Declarations

    // Code
	#ifdef ENABLE_GAUSSIAN_BLUR
	initializeQuad();
	if(initializeGaussianBlur(&gaussianBlurIndoorEffect) == false)
	{
		LOG("Initialize Gaussian Blur Effect FAILED!!");
		return (-7);
	}

	fullSceneIndoorFbo.textureWidth = WIN_WIDTH;
	fullSceneIndoorFbo.textureHeight = WIN_HEIGHT;

	if (createFBO(&fullSceneIndoorFbo) == false)
	{
		LOG("Unable to create FBO for entire scene");
		return (-8);
	}
	
#endif // ENABLE_GAUSSIAN_BLUR


#ifdef ENABLE_SSAO


	// framebuffer related variables
	//int windowWidth;
	//int windowHeight;


	ssaoFrameBufferDetails.textureWidth = 1920;
	ssaoFrameBufferDetails.textureHeight = 1080;

	if (ssaoCreateFBO(&ssaoFrameBufferDetails) == GL_FALSE) 
	{
		LOG("ssaoCreateFBO() For Shadow FAILED!!!\n");
		return(-1);
	}
	else 
	{
		LOG("ssaoCreateFBO() Successfull for Shadow!!!\n");
	}
	initializeSSAO();

#endif // ENABLE_SSAO

	fboMaskPass_Indoor.textureWidth = 1920;
	fboMaskPass_Indoor.textureHeight = 1080;

	createFBO(&fboMaskPass_Indoor);


	loadStaticModel("res/models/masks/individualScenes/RaudraWithText.obj", &maskModel_RaudraRas);
	loadStaticModel("res/models/masks/individualScenes/KarunWithText.obj", &maskModel_KarunRas);
	loadStaticModel("res/models/masks/individualScenes/HasyaWithText.obj", &maskModel_HasyaRas);


	if (LoadGLTexture_UsingSOIL(&texture_mask_indoor, "res/models/masks/copper.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_mask_indoor FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_mask_indoor Successfull = %u!!!\n", texture_mask_indoor);
	}

	noise_texture_eroded_indoor = initializeErosion();
	if (noise_texture_eroded_indoor == 0)
	{
		LOG("initializeErosion() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeErosion() Successfull!!!\n");
	}

	return 0;
}

void displayScene_PlaceHolderIndoor(SET_CAMERA setCamera, DISPLAY_PASSES_INDOOR displayPasses, bool shouldSceneBlur)
{
	// Function declarations
	void displayBlur(void);
	scene_types_t  getCurrentScene(void);

	// Code
	isBlurI = shouldSceneBlur;

	// Code
	// Here The Game STarts
	setCamera();

	// Masks
	glBindFramebuffer(GL_FRAMEBUFFER, fboMaskPass_Indoor.frameBuffer);
		glViewport(0, 0, (GLsizei)fboMaskPass_Indoor.textureWidth, (GLsizei)fboMaskPass_Indoor.textureHeight);
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboMaskPass_Indoor.textureWidth / fboMaskPass_Indoor.textureHeight, 0.1f, 100.0f);

		mat4 translationMatrix = mat4::identity();
		mat4 rotationMatrix = mat4::identity();
		mat4 modelMatrix = mat4::identity();
		mat4 scaleMatrix = mat4::identity();
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

		glUniform1f(sceneErosionNoiseUniform.scaleUniform, myScale_erosion_indoor);
		//glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, vec3(0.32, 0.32, 0.32));
		glUniform3fv(sceneErosionNoiseUniform.offsetUniform, 1, offset_ras_indoor);

		if (getCurrentScene() == SCENE05_KARUN_RAS)
		{

			drawCustomTextureStaticModel(maskModel_KarunRas, texture_mask_indoor, noise_texture_eroded_indoor);

		}
		else if (getCurrentScene() == SCENE07_RAUDRA_RAS)
		{

			drawCustomTextureStaticModel(maskModel_RaudraRas, texture_mask_indoor, noise_texture_eroded_indoor);

		}
		else if (getCurrentScene() == SCENE12_HASYA_RAS)
		{

			drawCustomTextureStaticModel(maskModel_HasyaRas, texture_mask_indoor, noise_texture_eroded_indoor);

		}


		//drawCustomTextureStaticModel(maskModel_shringarRas, texture_mask_indoor, noise_texture_eroded_indoor);
		// ################################### ROOM ###################################  

		glUseProgram(0);

		glDisable(GL_TEXTURE_3D);
		glDisable(GL_TEXTURE_2D);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (!shouldSceneBlur)
	{
#ifdef ENABLE_SSAO
		
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFrameBufferDetails.render_fbo);
		glViewport(0, 0, ssaoFrameBufferDetails.textureWidth, ssaoFrameBufferDetails.textureHeight);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		displayPasses();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		displaySSAO(&ssaoFrameBufferDetails);



#endif // ENABLE_SSAO
	}
	else
	{
		//glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, fullSceneIndoorFbo.frameBuffer);
		glViewport(0, 0, (GLsizei)fullSceneIndoorFbo.textureWidth, (GLsizei)fullSceneIndoorFbo.textureHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fullSceneIndoorFbo.textureWidth / fullSceneIndoorFbo.textureHeight,
			0.1f, 1000.0f);
		
		displayPasses();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		displayBlur();

		glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
		perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		fsGaussBlurIndoorQuadUniform = useFSQuadShader();
		glUniform1i(fsGaussBlurIndoorQuadUniform.singleTexture, 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurIndoorEffect.verticalFBDetails.frameBufferTexture);
		glUniform1i(fsGaussBlurIndoorQuadUniform.textureSamplerUniform1, 0);
		displayQuad();
    	glBindTexture(GL_TEXTURE_2D, 0);

		////
		glUniform1i(fsGaussBlurIndoorQuadUniform.singleTexture, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fboMaskPass_Indoor.frameBufferTexture);
		glUniform1i(fsGaussBlurIndoorQuadUniform.textureSamplerUniform1, 0);

		displayQuad();
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);
		//glEnable(GL_BLEND);

	}
}

void displayBlur(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurIndoorEffect.horrizontalFBDetails.frameBuffer);
	glViewport(0, 0, (GLsizei)gaussianBlurIndoorEffect.horrizontalFBDetails.textureWidth, 
	(GLsizei)gaussianBlurIndoorEffect.horrizontalFBDetails.textureHeight);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)gaussianBlurIndoorEffect.horrizontalFBDetails.textureWidth / gaussianBlurIndoorEffect.horrizontalFBDetails.textureHeight, 0.1f, 1000.0f);

    horizontalBlurUniform = useHorrizontalBlurShader();

    glUniform1f(horizontalBlurUniform.targetWidth, 540.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fullSceneIndoorFbo.frameBufferTexture);
    glUniform1i(horizontalBlurUniform.hblurTexSamplerUniform, 0);
	displayQuad();    
    glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurIndoorEffect.verticalFBDetails.frameBuffer);
	glViewport(0, 0, (GLsizei)gaussianBlurIndoorEffect.verticalFBDetails.textureWidth, 
	(GLsizei)gaussianBlurIndoorEffect.verticalFBDetails.textureHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	verticalBlurIndoorUniform = useVerticalBlurShader();
	glUniform1f(verticalBlurIndoorUniform.targetHeight, 270.0f);
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gaussianBlurIndoorEffect.horrizontalFBDetails.frameBufferTexture);
    glUniform1i(verticalBlurIndoorUniform.vblurTexSamplerUniform, 0);
	displayQuad();
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

	if (isBlurI == false)
	{
		offset_ras_indoor[0] = 0.17f;
		offset_ras_indoor[1] = 0.17f;
		offset_ras_indoor[2] = 0.17f;
	}

	if (((getCurrentScene() == SCENE05_KARUN_RAS) ||
		(getCurrentScene() == SCENE07_RAUDRA_RAS) ||
		(getCurrentScene() == SCENE12_HASYA_RAS)) &&
		isBlurI == true)
	{
		offset_ras_indoor[0] = offset_ras_indoor[0] + 0.002f;
		offset_ras_indoor[1] = offset_ras_indoor[1] + 0.002f;
		offset_ras_indoor[2] = offset_ras_indoor[2] + 0.002f;
		if (offset_ras_indoor[2] > 0.48f)
		{
			offset_ras_indoor[0] = 0.48f;
			offset_ras_indoor[1] = 0.48f;
			offset_ras_indoor[2] = 0.48f;
		}
	}
}

void uninitializeScene_PlaceHolderIndoor(void)
{
	// Code
	unloadStaticModel(&maskModel_KarunRas);
	unloadStaticModel(&maskModel_HasyaRas);
	unloadStaticModel(&maskModel_RaudraRas);

#ifdef ENABLE_DYNAMIC_MODELS
	// unloadDynamicModel(&skeletonModel);
#endif
}


