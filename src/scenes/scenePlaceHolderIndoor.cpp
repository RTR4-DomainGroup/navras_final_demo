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


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

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


	ssaoFrameBufferDetails.textureWidth = 2560;
	ssaoFrameBufferDetails.textureHeight = 1440;

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


	return 0;
}

void displayScene_PlaceHolderIndoor(SET_CAMERA setCamera, DISPLAY_PASSES_INDOOR displayPasses, bool shouldSceneBlur)
{
	void displayBlur(void);

	// Code
	// Here The Game STarts
	setCamera();

	if (!shouldSceneBlur)
	{
#ifdef ENABLE_SSAO
		glViewport(0, 0, ssaoFrameBufferDetails.textureWidth, ssaoFrameBufferDetails.textureHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFrameBufferDetails.render_fbo);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		displayPasses();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		displaySSAO(&ssaoFrameBufferDetails);



#endif // ENABLE_SSAO
	}
	else
	{
		glDisable(GL_BLEND);
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
		glUseProgram(0);
    	glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_BLEND);



		


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

    glUniform1f(horizontalBlurUniform.targetWidth, 960.0f);
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
	glUniform1f(verticalBlurIndoorUniform.targetHeight, 540.0f);
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
}

void uninitializeScene_PlaceHolderIndoor(void)
{
	// Code


#ifdef ENABLE_DYNAMIC_MODELS
	// unloadDynamicModel(&skeletonModel);
#endif
}


