#include "../../inc/scenes/scene9_VeerRas.h"

#include "../../inc/helper/common.h"

#include "../../inc/helper/camera.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/shadowframebuffer.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/texture_loader.h"

#include "../../inc/shaders/AtmosphereShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/FSQuadShader.h"

#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/AtmosphereEffect.h"
#include "../../inc/effects/CloudEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
#include "../../inc/effects/GaussianBlurEffect.h"

#define FBO_WIDTH 1920
#define FBO_HEIGHT 1080

#define ENABLE_TERRIAN
//#define ENABLE_ATMOSPHERE
//#define ENABLE_CLOUD_NOISE
//#define ENABLE_FOG
//#define ENABLE_STATIC_MODELS	
//#define ENABLE_GAUSSIAN_BLUR
//#define ENABLE_GODRAYS
//#define ENABLE_SHADOW

struct TextureVariables terrainTextureVariables_veer;

float displacementmap_depth_veer;

// Atmosphere Scattering
AtmosphereUniform atmosphereUniform_veer;
AtmosphericVariables atmosVariables_veer;

GLuint noise_texture_veer;

//Model variables
STATIC_MODEL rockModel_veer;
STATIC_MODEL streetLightModel_veer;

struct FrameBufferDetails fboBlackPass_veer;
struct FrameBufferDetails fboColorPass_veer;
struct FrameBufferDetails fboGodRayPass_veer;

extern mat4 perspectiveProjectionMatrix;

mat4 viewMatrix_veer;

struct ADSUniform sceneOutdoorADSUniform_veer;
struct FSQuadUniform fsqUniform_veer;

// Varaiables for God Rays
struct GodraysUniform sceneGodRaysUniform_veer;
GLfloat lightPosition_gr_veer[] = { 0.0f, 10.0f, -100.0f, 1.0f };

GLfloat cameraAngle_veer = 0.0f;

GLfloat dispersal_veer = 0.1875f;
GLfloat haloWidth_veer = 0.45f;
GLfloat intensity_veer = 1.5f;
GLfloat distortion_veer[] = { 0.94f, 0.97f, 1.0f };

extern int windowWidth;
extern int windowHeight;

GLfloat lightAmbient_veer[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse_veer[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular_veer[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition_veer[] = { 10.0f, 10.0f, 0.0f, 1.0f };

GLfloat materialAmbient_veer[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse_veer[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular_veer[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess_veer = 128.0f;

GLfloat skyColor_veer[] = { 0.0f, 0.0f, 0.8f, 0.0f };
GLfloat cloudColor_veer[] = { 0.8f, 0.8f, 0.8f, 0.0f };

// Shadow
ShadowFrameBufferDetails shadowFramebuffer_veer;
mat4 viewmatrixDepth_veer;
mat4 lightSpaceMatrix_veer;
mat4 perspectiveProjectionDepth_veer;

struct CloudNoiseUniform sceneCloudNoiseUniform_veer;

float myScale_veer = 1.0f;

float noiseScale_veer = 2.0f;
bool noiseScaleIncrement_veer = true;

struct TerrainUniform terrainUniform_veer;

GLfloat density_veer = 0.15;
GLfloat gradient_veer = 0.5;
GLfloat skyFogColor_veer[] = { 0.25f, 0.25f, 0.25f, 1.0f };

// Gaussian Blur related variables
struct GaussianBlurEffect gaussianBlurEffect_veer;
struct HorrizontalBlurUniform horizontalBlurUniform_veer;
struct VerticalBlurUniform verticalBlurUniform_veer;
struct FrameBufferDetails fullSceneFbo_veer;
struct FSQuadUniform fsGaussBlurQuadUniform_veer;

extern Camera camera;

int initializeScene9_VeerRas(void)
{
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
	
#ifdef ENABLE_SHADOW
	shadowFramebuffer_veer.textureWidth = 1024;
	shadowFramebuffer_veer.textureHeight = 1024;

	if (shadowCreateFBO(&shadowFramebuffer_veer) == FALSE) 
	{

		LOG("shadowCreateFBO() For Shadow FAILED!!!\n");
		return(-1);

	}
	else
	{

		LOG("shadowCreateFBO() Successfull for Shadow!!!\n");

	}

	initializeQuad();

#endif

#ifdef ENABLE_TERRIAN
	displacementmap_depth_veer = 15.0f;

	terrainTextureVariables_veer.albedoPath = TEXTURE_DIR"terrain/DiffuseMapTerrain.jpg";
	terrainTextureVariables_veer.displacementPath = TEXTURE_DIR"terrain/DisplacementMapTerrain.jpg";
	terrainTextureVariables_veer.normalPath = TEXTURE_DIR"terrain/NormalMapTerrain.jpg";

	if (initializeTerrain(&terrainTextureVariables_veer) != 0)
	{
		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}

#endif

#ifdef ENABLE_ATMOSPHERE
	//
	atmosVariables_veer.m_nSamples = 3;		// Number of sample rays to use in integral equation
	atmosVariables_veer.m_Kr = 0.0035f;		// Rayleigh scattering constant
	atmosVariables_veer.m_Kr4PI = atmosVariables_veer.m_Kr * 4.0f * M_PI;
	atmosVariables_veer.m_Km = 0.0015f;		// Mie scattering constant
	atmosVariables_veer.m_Km4PI = atmosVariables_veer.m_Km * 4.0f * M_PI;
	atmosVariables_veer.m_ESun = 20.0f;		// Sun brightness constant
	atmosVariables_veer.m_g = -0.990f;		// The Mie phase asymmetry factor
	atmosVariables_veer.m_fExposure = 2.0f;

	atmosVariables_veer.m_fInnerRadius = 10.0f;
	atmosVariables_veer.m_fOuterRadius = 50.0f;
	//atmosVariables_veer.m_fOuterRadius = atmosVariables_veer.m_fInnerRadius + (atmosVariables_veer.m_fInnerRadius * 2.5f);
	atmosVariables_veer.m_fScale = 1 / (atmosVariables_veer.m_fOuterRadius - atmosVariables_veer.m_fInnerRadius);

	atmosVariables_veer.m_fWavelength[0] = 0.650f;		// 650 nm for red
	atmosVariables_veer.m_fWavelength[1] = 0.570f;		// 570 nm for green
	atmosVariables_veer.m_fWavelength[2] = 0.475f;		// 475 nm for blue
	atmosVariables_veer.m_fWavelength4[0] = powf(atmosVariables_veer.m_fWavelength[0], 4.0f);
	atmosVariables_veer.m_fWavelength4[1] = powf(atmosVariables_veer.m_fWavelength[1], 4.0f);
	atmosVariables_veer.m_fWavelength4[2] = powf(atmosVariables_veer.m_fWavelength[2], 4.0f);

	atmosVariables_veer.m_fRayleighScaleDepth = 0.25f;
	atmosVariables_veer.m_fMieScaleDepth = 0.1f;

	atmosVariables_veer.m_vLight = vec3(0, 0, -350);
	atmosVariables_veer.m_vLightDirection = atmosVariables_veer.m_vLight / sqrtf(atmosVariables_veer.m_vLight[0] * atmosVariables_veer.m_vLight[0] + atmosVariables_veer.m_vLight[1] * atmosVariables_veer.m_vLight[1] + atmosVariables_veer.m_vLight[2] * atmosVariables_veer.m_vLight[2]);

	//
	initializeAtmosphere(atmosVariables_veer);

#endif

#ifdef ENABLE_GODRAYS

	int initializeGodRays_veer(void);
	initializeSphere(1.0f, 60, 60);
	initializeGodRays_veer();
	initializeQuad();
#endif // ENABLE_GODRAYS

#ifdef ENABLE_CLOUD_NOISE

	noise_texture_veer = initializeCloud();
	if (noise_texture_veer == 0)
	{
		LOG("initializeCloud() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeCloud() Successfull!!!\n");
	}
#endif

#ifdef ENABLE_STATIC_MODELS
	// static model initialization
	loadStaticModel("res/models/rock/rock.obj", &rockModel_veer);
	loadStaticModel("res/models/streetLight/StreetLight.obj", &streetLightModel_veer);
#endif

#ifdef ENABLE_GAUSSIAN_BLUR
	initializeQuad();
	if (initializeGaussianBlur(&gaussianBlurEffect_veer) == false)
	{
		//LOG("Initialize Gaussian Blur Effect FAILED!!");
		return (-7);
	}

	fullSceneFbo_veer.textureWidth = 1920;
	fullSceneFbo_veer.textureHeight = 1080;

	if (createFBO(&fullSceneFbo_veer) == false)
	{
		//LOG("Unable to create FBO for entire scene");
		return (-8);
	}
#endif
	return(0);
}

void displayScene9_VeerRas(void)
{
	// Function Declarations
	void displayPasses_veer(int, bool, bool, bool, int);
	void displayGodRays(int, int);
	void displayGaussianBlur_veer(void);

	setCamera();

	mat4 translationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	viewMatrix_veer = mat4::identity();
	viewMatrix_veer = vmath::lookat(camera.eye, camera.center, camera.up);

	// shadow
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer_veer.frameBuffer);
	glViewport(0, 0, (GLsizei)shadowFramebuffer_veer.textureWidth, (GLsizei)shadowFramebuffer_veer.textureHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)shadowFramebuffer_veer.textureWidth / shadowFramebuffer_veer.textureHeight, 0.1f, 100.0f);
	displayPasses_veer(1, true, true, true, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#ifdef ENABLE_GAUSSIAN_BLUR
	displayPasses_veer(1, false, false, true, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, fullSceneFbo_veer.frameBuffer);
	glViewport(0, 0, (GLsizei)fullSceneFbo_veer.textureWidth, (GLsizei)fullSceneFbo_veer.textureHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fullSceneFbo_veer.textureWidth / fullSceneFbo_veer.textureHeight, 0.1f, 1000.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	displayPasses_veer(1, false, false, true, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	displayGaussianBlur_veer();

	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 1000.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fsGaussBlurQuadUniform_veer = useFSQuadShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gaussianBlurEffect_veer.verticalFBDetails.frameBufferTexture);
	glUniform1i(fsGaussBlurQuadUniform_veer.textureSamplerUniform1, 0);
	displayQuad();
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
#else

	// display god rays related
	// GodRay Black pass
	glBindFramebuffer(GL_FRAMEBUFFER, fboBlackPass_veer.frameBuffer);
	glViewport(0, 0, (GLsizei)fboBlackPass_veer.textureWidth, (GLsizei)fboBlackPass_veer.textureHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboBlackPass_veer.textureWidth / fboBlackPass_veer.textureHeight,
		0.1f, 1000.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	displayPasses_veer(0, false, false, true, 0);

	sceneOutdoorADSUniform_veer = useADSShader();
	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	translationMatrix = vmath::translate(lightPosition_gr_veer[0], lightPosition_gr_veer[1], lightPosition_gr_veer[2]);
	modelMatrix = translationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.viewMatrixUniform, 1, GL_FALSE, viewMatrix_veer);
	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniform1i(sceneOutdoorADSUniform_veer.uniform_enable_godRays, 0);
	glUniform1i(sceneOutdoorADSUniform_veer.godrays_blackpass_sphere, 1);
	float color[3] = { 1.0f, 1.0f, 1.0f };
	glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f, 1.0f, 1.0f));
	displaySphere(color);
	glUseProgram(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// GodRay Color Pass
	glBindFramebuffer(GL_FRAMEBUFFER, fboColorPass_veer.frameBuffer);
	glViewport(0, 0, (GLsizei)fboColorPass_veer.textureWidth, (GLsizei)fboColorPass_veer.textureHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboColorPass_veer.textureWidth / fboColorPass_veer.textureHeight,
		0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//displayWaterFramebuffers(1);
	displayPasses_veer(1, false, false, true, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// God Rays Pass
	glBindFramebuffer(GL_FRAMEBUFFER, fboGodRayPass_veer.frameBuffer);
	glViewport(0, 0, (GLsizei)fboGodRayPass_veer.textureWidth, (GLsizei)fboGodRayPass_veer.textureHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)fboGodRayPass_veer.textureWidth / fboGodRayPass_veer.textureHeight,
		0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneGodRaysUniform_veer = useGodRaysShader();

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	//translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix;

	glUniform4fv(sceneGodRaysUniform_veer.lightPositionOnScreen, 1, lightPosition_gr_veer);
	glUniform1f(sceneGodRaysUniform_veer.decay, 1.0f);
	glUniform1f(sceneGodRaysUniform_veer.density, 0.92f);
	glUniform1f(sceneGodRaysUniform_veer.exposure, 0.25f);
	glUniform1f(sceneGodRaysUniform_veer.weight, 0.04f);

	glUniformMatrix4fv(sceneGodRaysUniform_veer.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneGodRaysUniform_veer.viewMatrix, 1, GL_FALSE, viewMatrix_veer);
	glUniformMatrix4fv(sceneGodRaysUniform_veer.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniform1i(sceneGodRaysUniform_veer.godrays_lfEnabled, 1);

	glUniform1f(sceneGodRaysUniform_veer.dispersalUniform, dispersal_veer);
	glUniform1f(sceneGodRaysUniform_veer.haloWidthUniform, haloWidth_veer);
	glUniform1f(sceneGodRaysUniform_veer.intensityUniform, intensity_veer);
	glUniform3fv(sceneGodRaysUniform_veer.distortionUniform, 1, distortion_veer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboBlackPass_veer.frameBufferTexture);
	glUniform1i(sceneGodRaysUniform_veer.godraysampler, 0);
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Godrays Default Frame Buffer
	glViewport(0, 0, (GLsizei)windowWidth, (GLsizei)windowHeight);
	perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight,
		0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	fsqUniform_veer = useFSQuadShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fboGodRayPass_veer.frameBufferTexture);
	glUniform1i(fsqUniform_veer.textureSamplerUniform1, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fboColorPass_veer.frameBufferTexture);
	glUniform1i(fsqUniform_veer.textureSamplerUniform2, 1);
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
#endif
}

void displayPasses_veer(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0)
{
	// Code+
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
	setCamera();
	//setCamera(&camera);

	mat4 finalViewMatrix_veer = mat4::identity();

	if (actualDepthQuadScene == 0) {

		finalViewMatrix_veer = viewMatrix;

	}
	else if (actualDepthQuadScene == 1) {

		finalViewMatrix_veer = viewMatrix;

		finalViewMatrix_veer = mat4::identity();
		finalViewMatrix_veer = lookat(vec3(lightPosition_veer[0], lightPosition_veer[1], lightPosition_veer[2]), vec3(0.0f, -5.0f, -20.0f), vec3(0.0f, 1.0f, 0.0f));
		/*perspectiveProjectionDepth_veer = mat4::identity();
		perspectiveProjectionDepth_veer = vmath::perspective(45.0f, (GLfloat)windowWidth / windowHeight, 0.1f, 100.0f);*/

		lightSpaceMatrix_veer = mat4::identity();
		lightSpaceMatrix_veer = perspectiveProjectionMatrix * finalViewMatrix_veer;

	}

	if (actualDepthQuadScene == 0) {

		if (godRays == 1) {

#ifdef ENABLE_ATMOSPHERE

			translationMatrix = mat4::identity();
			rotationMatrix = mat4::identity();
			modelMatrix = mat4::identity();

			//glBlendFunc(GL_ONE, GL_ONE);

			atmosphereUniform_veer = useAtmosphereShader();

			glUniform3f(atmosphereUniform_veer.cameraPosUniform, cameraEyeX, cameraEyeY, cameraEyeZ);
			glUniform3f(atmosphereUniform_veer.lightPosUniform, atmosVariables_veer.m_vLightDirection[0], atmosVariables_veer.m_vLightDirection[1], atmosVariables_veer.m_vLightDirection[2]);
			glUniform3f(atmosphereUniform_veer.invWavelengthUniform, 1 / atmosVariables_veer.m_fWavelength4[0], 1 / atmosVariables_veer.m_fWavelength4[1], 1 / atmosVariables_veer.m_fWavelength4[2]);
			glUniform1f(atmosphereUniform_veer.cameraHeightUniform, sqrtf(cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ));
			glUniform1f(atmosphereUniform_veer.cameraHeight2Uniform, cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ);
			glUniform1f(atmosphereUniform_veer.innerRadiusUniform, atmosVariables_veer.m_fInnerRadius);
			glUniform1f(atmosphereUniform_veer.innerRadius2Uniform, atmosVariables_veer.m_fInnerRadius * atmosVariables_veer.m_fInnerRadius);
			glUniform1f(atmosphereUniform_veer.outerRadiusUniform, atmosVariables_veer.m_fOuterRadius);
			glUniform1f(atmosphereUniform_veer.outerRadius2Uniform, atmosVariables_veer.m_fOuterRadius * atmosVariables_veer.m_fOuterRadius);
			glUniform1f(atmosphereUniform_veer.KrESunUniform, atmosVariables_veer.m_Kr * atmosVariables_veer.m_ESun);
			glUniform1f(atmosphereUniform_veer.KmESunUniform, atmosVariables_veer.m_Km * atmosVariables_veer.m_ESun);
			glUniform1f(atmosphereUniform_veer.Kr4PIUniform, atmosVariables_veer.m_Kr4PI);
			glUniform1f(atmosphereUniform_veer.Km4PIUniform, atmosVariables_veer.m_Km4PI);
			glUniform1f(atmosphereUniform_veer.scaleUniform, 1.0f / (atmosVariables_veer.m_fOuterRadius - atmosVariables_veer.m_fInnerRadius));
			glUniform1f(atmosphereUniform_veer.scaleDepthUniform, atmosVariables_veer.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniform_veer.scaleOverScaleDepthUniform, (1.0f / (atmosVariables_veer.m_fOuterRadius - atmosVariables_veer.m_fInnerRadius)) / atmosVariables_veer.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniform_veer.gUniform, atmosVariables_veer.m_g);
			glUniform1f(atmosphereUniform_veer.g2Uniform, atmosVariables_veer.m_g * atmosVariables_veer.m_g);

			glUniformMatrix4fv(atmosphereUniform_veer.modelMatrix, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(atmosphereUniform_veer.viewMatrix, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(atmosphereUniform_veer.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

			displayAtmosphere();

			glUseProgram(0);

#endif

		}

#ifdef ENABLE_CLOUD_NOISE

		glEnable(GL_TEXTURE_3D);
		sceneCloudNoiseUniform_veer = useCloudNoiseShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		rotateX = mat4::identity();

		//translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
		translationMatrix = vmath::translate(0.0f, 0.0f, -500.0f); // glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
		scaleMatrix = vmath::scale(800.0f, 450.0f, 1.0f);
		//rotateX = vmath::rotate(10.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotateX;

		//viewMatrix_veer = vmath::lookat(camera.eye, camera.eye, camera.up);

		glUniformMatrix4fv(sceneCloudNoiseUniform_veer.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneCloudNoiseUniform_veer.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneCloudNoiseUniform_veer.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform3fv(sceneCloudNoiseUniform_veer.laUniform, 1, lightAmbient_veer);
		glUniform3fv(sceneCloudNoiseUniform_veer.ldUniform, 1, lightDiffuse_veer);
		glUniform3fv(sceneCloudNoiseUniform_veer.lsUniform, 1, lightSpecular_veer);
		glUniform4fv(sceneCloudNoiseUniform_veer.lightPositionUniform, 1, lightPosition_veer);

		glUniform3fv(sceneCloudNoiseUniform_veer.kaUniform, 1, materialAmbient_veer);
		glUniform3fv(sceneCloudNoiseUniform_veer.kdUniform, 1, materialDiffuse_veer);
		glUniform3fv(sceneCloudNoiseUniform_veer.ksUniform, 1, materialSpecular_veer);
		glUniform1f(sceneCloudNoiseUniform_veer.materialShininessUniform, materialShininess_veer);

		glUniform1f(sceneCloudNoiseUniform_veer.scaleUniform, myScale_veer);
		glUniform3fv(sceneCloudNoiseUniform_veer.skyColorUniform, 1, skyColor_veer);
		glUniform3fv(sceneCloudNoiseUniform_veer.cloudColorUniform, 1, cloudColor_veer);
		glUniform1f(sceneCloudNoiseUniform_veer.noiseScaleUniform, noiseScale_veer);
		glUniform1i(sceneCloudNoiseUniform_veer.uniform_enable_godRays, godRays);
		//glUniform1f(sceneCloudNoiseUniform_veer.alphaBlendingUniform, alphaBlending);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noise_texture_veer);

		displayQuad();

		glUseProgram(0);

		glDisable(GL_TEXTURE_3D);

#endif
	}

#ifdef ENABLE_TERRIAN
	terrainUniform_veer = useTerrainShader();

	vmath::mat4 mv_matrix_veer = mat4::identity();
	vmath::mat4 proj_matrix = mat4::identity();


	mv_matrix_veer = finalViewMatrix_veer * (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));

	proj_matrix = perspectiveProjectionMatrix;

	//normal mapping
	vmath::mat4 m_matrix = (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));
	vmath::mat4 v_matrix = viewMatrix;

	glUniformMatrix4fv(terrainUniform_veer.modelMatrixUniform, 1, GL_FALSE, m_matrix);
	glUniformMatrix4fv(terrainUniform_veer.viewMatrixUniform, 1, GL_FALSE, v_matrix);
	glUniformMatrix4fv(terrainUniform_veer.projectionMatrixUniform, 1, GL_FALSE, proj_matrix);

	glUniform3fv(terrainUniform_veer.laUniform, 1, lightAmbient_veer);
	glUniform3fv(terrainUniform_veer.ldUniform, 1, lightDiffuse_veer);
	glUniform3fv(terrainUniform_veer.lsUniform, 1, lightSpecular_veer);
	glUniform4fv(terrainUniform_veer.lightPositionUniform, 1, lightPosition_veer);

	glUniform3fv(terrainUniform_veer.kaUniform, 1, materialAmbient_veer);
	glUniform3fv(terrainUniform_veer.ksUniform, 1, materialSpecular_veer);
	glUniform1f(terrainUniform_veer.materialShininessUniform, materialShininess_veer);


	glUniformMatrix4fv(terrainUniform_veer.uniform_mv_matrix, 1, GL_FALSE, mv_matrix_veer);
	glUniformMatrix4fv(terrainUniform_veer.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniform_veer.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix_veer);

	glUniform1f(terrainUniform_veer.uniform_dmap_depth, displacementmap_depth_veer);
	//glUniform1i(terrainUniform_veer.uniform_enable_fog, enable_fog ? 1 : 0);
	//glUniform1i(terrainUniform_veer.uniform_enable_fog, 0);
	glUniform1i(terrainUniform_veer.uniform_enable_godRays, godRays);

#ifdef ENABLE_FOG
	glUniform1i(terrainUniform_veer.fogEnableUniform, 1);
	glUniform1f(terrainUniform_veer.densityUniform, density_veer);
	glUniform1f(terrainUniform_veer.gradientUniform, gradient_veer);
	glUniform4fv(terrainUniform_veer.skyFogColorUniform, 1, skyFogColor_veer);
#endif // DEBUG

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables_veer.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables_veer.albedo);
	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
#endif

#ifdef ENABLE_STATIC_MODELS
	sceneOutdoorADSUniform_veer = useADSShader();

	// Sending Light Related Uniforms
	glUniform1i(sceneOutdoorADSUniform_veer.lightingEnableUniform, 1);
	glUniform4fv(sceneOutdoorADSUniform_veer.laUniform, 1, lightAmbient_veer);
	glUniform4fv(sceneOutdoorADSUniform_veer.ldUniform, 1, lightDiffuse_veer);
	glUniform4fv(sceneOutdoorADSUniform_veer.lsUniform, 1, lightSpecular_veer);
	glUniform4fv(sceneOutdoorADSUniform_veer.lightPositionUniform, 1, lightPosition_veer);
	glUniform4fv(sceneOutdoorADSUniform_veer.kaUniform, 1, materialAmbient_veer);
	glUniform4fv(sceneOutdoorADSUniform_veer.kdUniform, 1, materialDiffuse_veer);
	glUniform4fv(sceneOutdoorADSUniform_veer.ksUniform, 1, materialSpecular_veer);
	glUniform1f(sceneOutdoorADSUniform_veer.materialShininessUniform, materialShininess_veer);

	glUniform1i(sceneOutdoorADSUniform_veer.fogEnableUniform, 1);
	glUniform1f(sceneOutdoorADSUniform_veer.densityUniform, density_veer);
	glUniform1f(sceneOutdoorADSUniform_veer.gradientUniform, gradient_veer);
	glUniform4fv(sceneOutdoorADSUniform_veer.skyFogColorUniform, 1, skyFogColor_veer);
	glUniform1i(sceneOutdoorADSUniform_veer.uniform_enable_godRays, godRays);
	glUniform1i(sceneOutdoorADSUniform_veer.godrays_blackpass_sphere, 0);

	//glUniform1i(sceneOutdoorADSUniform_veer.)
	// ------ Rock Model ------
	translationMatrix = vmath::translate(-1.0f, 0.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	if (actualDepthQuadScene == 1) 
	{

		glUniform1i(sceneOutdoorADSUniform_veer.actualSceneUniform, 0);
		glUniform1i(sceneOutdoorADSUniform_veer.depthSceneUniform, 1);
		glUniform1i(sceneOutdoorADSUniform_veer.depthQuadSceneUniform, 0);
		glUniformMatrix4fv(sceneOutdoorADSUniform_veer.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix_veer);

	}
	else 
	{

		glUniform1i(sceneOutdoorADSUniform_veer.actualSceneUniform, 1);
		glUniform1i(sceneOutdoorADSUniform_veer.depthSceneUniform, 0);
		glUniform1i(sceneOutdoorADSUniform_veer.depthQuadSceneUniform, 0);

		glActiveTexture(GL_TEXTURE6);
		glUniform1i(sceneOutdoorADSUniform_veer.shadowMapSamplerUniform, 6);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer_veer.frameBufferDepthTexture);

	}

	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix_veer);
	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);


	drawStaticModel(rockModel_veer);

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	// ------ Streetlight Model ------
	translationMatrix = vmath::translate(1.0f, -2.0f, -6.0f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix_veer);
	glUniformMatrix4fv(sceneOutdoorADSUniform_veer.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);


	drawStaticModel(streetLightModel_veer);

	if (actualDepthQuadScene == 0) 
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Un-use ShaderProgramObject
	glUseProgram(0);
#endif
}

int initializeGodRays_veer(void)
{
	fboBlackPass_veer.textureWidth = FBO_WIDTH;
	fboBlackPass_veer.textureHeight = FBO_HEIGHT;

	fboColorPass_veer.textureWidth = FBO_WIDTH;
	fboColorPass_veer.textureHeight = FBO_HEIGHT;

	fboGodRayPass_veer.textureWidth = FBO_WIDTH;
	fboGodRayPass_veer.textureHeight = FBO_HEIGHT;

	createFBO(&fboBlackPass_veer);
	createFBO(&fboColorPass_veer);
	createFBO(&fboGodRayPass_veer);

	return(0);
}

void resize_godRayPasses_veer(int fboWidh, int fboHeight)
{
	glViewport(0, 0, (GLsizei)fboWidh, (GLsizei)fboHeight);
}

void displayGaussianBlur_veer(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurEffect_veer.horrizontalFBDetails.frameBuffer);
	glViewport(0, 0, (GLsizei)gaussianBlurEffect_veer.horrizontalFBDetails.textureWidth,
		(GLsizei)gaussianBlurEffect_veer.horrizontalFBDetails.textureHeight);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	horizontalBlurUniform_veer = useHorrizontalBlurShader();

	glUniform1f(horizontalBlurUniform_veer.targetWidth, 960.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fullSceneFbo_veer.frameBufferTexture);
	glUniform1i(horizontalBlurUniform_veer.hblurTexSamplerUniform, 0);
	displayQuad();
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurEffect_veer.verticalFBDetails.frameBuffer);
	glViewport(0, 0, (GLsizei)gaussianBlurEffect_veer.verticalFBDetails.textureWidth,
		(GLsizei)gaussianBlurEffect_veer.verticalFBDetails.textureHeight);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	verticalBlurUniform_veer = useVerticalBlurShader();
	glUniform1f(verticalBlurUniform_veer.targetHeight, 540.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gaussianBlurEffect_veer.horrizontalFBDetails.frameBufferTexture);
	glUniform1i(verticalBlurUniform_veer.vblurTexSamplerUniform, 0);
	displayQuad();
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void updateScene9_VeerRas(void)
{
#ifdef ENABLE_CLOUD_NOISE
	// update Cloud
	updateCloud(noiseScaleIncrement_veer, noiseScale_veer, 0.0001f);
#endif
	cameraAngle_veer = cameraAngle_veer + 0.5f;
	if (cameraAngle_veer >= 360.0f)
		cameraAngle_veer -= 360.0f;
}

void uninitializeScene9_VeerRas(void)
{
#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariables_veer);
#endif

#ifdef ENABLE_ATMOSPHERE
	uninitializeAtmosphere();
#endif

#ifdef ENABLE_CLOUD_NOISE

	uninitializeCloud();
	if (noise_texture_veer)
	{
		glDeleteTextures(1, &noise_texture_veer);
		noise_texture_veer = 0;
	}
#endif

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&rockModel_veer);
	unloadStaticModel(&streetLightModel_veer);
#endif

#ifdef ENABLE_GAUSSIAN_BLUR
	uninitializeGaussianBlur(&gaussianBlurEffect_veer);
#endif
}
