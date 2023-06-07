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
GLfloat earthAngle = 90.0f;
#ifdef ENABLE_SHADOW
// Shadow
extern ShadowFrameBufferDetails shadowFramebuffer;
extern mat4 viewmatrixDepth;
extern mat4 lightSpaceMatrix;
extern mat4 perspectiveProjectionDepth;
#endif // ENABLE_SHADOW

extern struct FrameBufferDetails fboEarthAndSpace;

extern int windowWidth;
extern int windowHeight;

extern mat4 viewMatrix;

extern GLfloat skyColor[]; // = { 0.0f, 0.0f, 0.8f, 0.0f };
extern GLfloat cloudColor[]; // = { 0.8f, 0.8f, 0.8f, 0.0f };

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

GLfloat angleSphere = 90.0f;

#ifdef ENABLE_DYNAMIC_MODELS
static DYNAMIC_MODEL skeletonModel;
#endif // ENABLE_STATIC_MODELS

//static struct TextureVariables terrainTextureVariables;
//static float displacementmap_depth;

static GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightPosition[] = { 0.0f, 0.0f, -15.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

bool isInitialDisplayScene02_EarthAndSpace = true;

GLfloat cameraRadiusEarthAndSpace = 4.0f;
GLfloat cameraAngleEarthAndSpace = 90.0f;

//float distance10;

int initializeScene02_EarthAndSpace(void)
{
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

	if (LoadGLTexture_UsingSOIL(&texture_earth, TEXTURE_DIR"Earth/Earth.jpg") == FALSE)
	{
		LOG("Starfield LoadGLTexture for Earth FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture for Earth is Successful = %u!!!\n", texture_earth);
	}

	if (LoadGLTexture_UsingSOIL(&texture_sun, TEXTURE_DIR"Sun/Sun.jpg") == FALSE)
	{
		LOG("Starfield LoadGLTexture for Sun FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture for Sun is Successful = %u!!!\n", texture_sun);
	}

	initializeCube();
	initializeQuad();

#endif // ENABLE_STARFIELD

	return 0;
}

void setCameraScene02_EarthAndSpace(void)
{
	if (isInitialDisplayScene02_EarthAndSpace == true)
	{
		setCamera(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		isInitialDisplayScene02_EarthAndSpace = false;
	}
}

float anglez = 15.0f;
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

	//rotateCamera(0.0f, 0.0f, -6.0f, cameraRadiusEarthAndSpace, cameraAngleEarthAndSpace);
	displayCamera();
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	//setCamera(&camera);

	mat4 finalViewMatrix = mat4::identity();

	if (actualDepthQuadScene == 0)
	{
		finalViewMatrix = viewMatrix;
	}
	else if (actualDepthQuadScene == 1)
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

		glUniform4fv(adsEarthAndSpaceUniform.laUniform, 1, lightAmbient);
		glUniform4fv(adsEarthAndSpaceUniform.ldUniform, 1, lightDiffuse);
		glUniform4fv(adsEarthAndSpaceUniform.lsUniform, 1, lightSpecular);
		glUniform4f(adsEarthAndSpaceUniform.lightPositionUniform, 0.0f, 0.0f, anglez, 1.0f);
		anglez -= 0.5f;
		//LOG("%f\n", anglez);
		//glUniform4fv(adsEarthAndSpaceUniform.lightPositionUniform, 1, lightPosition);

		glUniform4fv(adsEarthAndSpaceUniform.kaUniform, 1, materialAmbient);
		glUniform4fv(adsEarthAndSpaceUniform.kdUniform, 1, materialDiffuse);
		glUniform4fv(adsEarthAndSpaceUniform.ksUniform, 1, materialSpecular);
		glUniform1f(adsEarthAndSpaceUniform.materialShininessUniform, materialShininess);

		glUniform1i(adsEarthAndSpaceUniform.uniform_enable_godRays, godRays);

		glUniform1f(adsEarthAndSpaceUniform.gradientUniform, gradient);
		glUniform1f(adsEarthAndSpaceUniform.densityUniform, density);

		glUniform1i(adsEarthAndSpaceUniform.fogEnableUniform, 0);
		glUniform4fv(adsEarthAndSpaceUniform.skyFogColorUniform, 1, skyFogColor);
		glUniform1i(adsEarthAndSpaceUniform.godrays_blackpass_sphere, 0);
		glUniform1i(adsEarthAndSpaceUniform.isInstanced, 0);

		// Cube
		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();

		translationMatrix = vmath::translate(-200.0f, 0.0f, -10.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(400.0f, 400.0f, 400.0f);
		modelMatrix = translationMatrix * scaleMatrix;									// ORDER IS VERY IMPORTANT

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
		glBindTexture(GL_TEXTURE_2D, fboEarthAndSpace.frameBufferTexture);
		//glUniform1i(adsEarthAndSpaceUniform.textureSamplerUniform_diffuse, 0);

		displayCube();
		glBindTexture(GL_TEXTURE_2D, 0);

		// Earth
		translationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
		rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
		rotationMatrix = vmath::rotate(angleSphere, 0.0f, 1.0f, 0.0f);
		rotationMatrix = rotationMatrix * rotationMatrix_x;
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

		//// Sun
		//translationMatrix = mat4::identity();
		//modelMatrix = mat4::identity();
		//rotationMatrix = mat4::identity();
		//scaleMatrix = mat4::identity();

		//translationMatrix = vmath::translate(200.0f, 20.0f, -50.0f);					// glTranslatef() is replaced by this line.
		////scaleMatrix = vmath::scale(0.0f, 10.0f, -100.0f);
		//modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		//glUniformMatrix4fv(adsEarthAndSpaceUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		//if (actualDepthQuadScene == 1)
		//{
		//	glUniform1i(adsEarthAndSpaceUniform.actualSceneUniform, 0);
		//	glUniform1i(adsEarthAndSpaceUniform.depthSceneUniform, 1);
		//	glUniformMatrix4fv(adsEarthAndSpaceUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
		//}
		//else
		//{
		//	glUniform1i(adsEarthAndSpaceUniform.actualSceneUniform, 1);
		//	glUniform1i(adsEarthAndSpaceUniform.depthSceneUniform, 0);
		//	glActiveTexture(GL_TEXTURE3);
		//	glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
		//}

		//glUniformMatrix4fv(adsEarthAndSpaceUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		//glUniformMatrix4fv(adsEarthAndSpaceUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture_sun);
		//glUniform1i(adsEarthAndSpaceUniform.textureSamplerUniform_diffuse, 0);

		////displaySphere(color);
		//glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);

#endif // ENABLE_STARFIELD

	}

}

void updateScene02_EarthAndSpace(void)
{
#ifdef ENABLE_CAMERA_ANIMATION
	cameraAngleEarthAndSpace += 0.005f;
	if (cameraAngleEarthAndSpace > 120.0f)
		cameraAngleEarthAndSpace = 120.0f;
#endif // ENABLE_CAMERA_ANIMATION

	// Code
	angleSphere = angleSphere + 0.2f;
	if (angleSphere >= 360.0f)
		angleSphere = 0.0f;

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
		texture_sun = 0;
	}

	if (texture_earth)
	{
		glDeleteTextures(1, &texture_earth);
		texture_earth = 0;
	}

	uninitializeStarfield(texture_star);
#endif // ENABLE_STARFIELD

}



