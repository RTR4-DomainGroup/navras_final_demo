
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

//#ifdef ENABLE_ATMOSPHERE
//#include "../../inc/shaders/AtmosphereShader.h"
//#include "../../inc/effects/AtmosphereEffect.h"
//#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_TERRIAN
#include "../../inc/effects/TerrainEffect.h"
#endif // ENABLE_TERRIAN

#ifdef ENABLE_CLOUD_NOISE
#include "../../inc/effects/CloudEffect.h"
#endif // ENABLE_CLOUD_NOISE

//#ifdef ENABLE_STATIC_MODELS
//#include "../../inc/effects/StaticModelLoadingEffect.h"
//#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_GAUSSIAN_BLUR
#include "../../inc/effects/GaussianBlurEffect.h"
#endif // ENABLE_GAUSSIAN_BLUR

#include "../../inc/scenes/scene09_VeerRas.h"

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern GLfloat whiteSphere[]; // = {1.0f, 1.0f, 1.0f};
extern GLuint texture_Marble;
extern TEXTURE texture_grass;
extern TEXTURE texture_flower;

extern struct ADSUniform sceneOutdoorADSStaticUniform;
extern struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;

extern struct FSQuadUniform fsqUniform;

#ifdef ENABLE_TERRIAN
extern struct TerrainUniform terrainUniform;
#endif // ENABLE_TERRIAN

#ifdef ENABLE_CLOUD_NOISE
struct CloudNoiseUniform sceneVeerRasCloudNoiseUniform;
float cloudMyScaleVeerRas = 1.0f;
float cloudNoiseScaleVeerRas = 2.0f;
bool cloudNoiseScaleIncrementVeerRas = true;
GLuint noise_texture_veer_ras;
GLfloat skyColorForVeerRas[] = { 0.3f, 0.3f, 0.5f, 0.0f };
GLfloat cloudColorForVeerRas[] = { 0.8f, 0.8f, 0.8f, 1.0f };
#endif // ENABLE_CLOUD_NOISE

struct TextureVariables terrainTextureVariables;

#ifdef ENABLE_GAUSSIAN_BLUR
// Gaussian Blur related variables
extern struct GaussianBlurEffect gaussianBlurEffect;
extern struct HorrizontalBlurUniform horizontalBlurUniform;
extern struct VerticalBlurUniform verticalBlurUniform;
extern struct FrameBufferDetails fullSceneFbo;
extern struct FSQuadUniform fsGaussBlurQuadUniform;
#endif // ENABLE_GAUSSIAN_BLUR

//#ifdef ENABLE_ATMOSPHERE
//// Atmosphere Scattering
//static AtmosphereUniform atmosphereUniform_09;
//static AtmosphericVariables atmosVariables_09;
//#endif // ENABLE_ATMOSPHERE

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

static GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 100.0f, 100.0f, 0.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

extern mat4 viewMatrix;

extern GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static float displacementmap_depth;

//#ifdef ENABLE_STATIC_MODELS
////Model variables
//STATIC_MODEL rockModel;
//STATIC_MODEL streetLightModel;
//
//#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
DYNAMIC_MODEL skeletonModel;
GLfloat model_y = 4.5f;
#endif // ENABLE_STATIC_MODELS

extern GLfloat density; // = 0.15;
extern GLfloat gradient; // = 0.5;
extern GLfloat skyFogColor[]; // = { 0.25f, 0.25f, 0.25f, 1.0f };


// Camera angle for rotation
GLfloat cameraAngle = 86.0f;
GLfloat cameraRadius;

extern GLfloat dispersal; // = 0.1875f;
extern GLfloat haloWidth; // = 0.45f;
extern GLfloat intensity; // = 1.5f;
extern GLfloat distortion[]; // = { 0.94f, 0.97f, 1.0f };

bool isInitialDisplayScene09_VeerRas = true;
bool isCameraRotation = false;
bool continueCameraRotation = true;
bool stopCameraRotation = false;
int cameraRotationCount = 0;

GLuint texture_veerMask;

int initializeScene09_VeerRas(void)
{
	// code

#ifdef ENABLE_MASKSQUADS
	initializeQuad();

	if (LoadGLTexture_UsingSOIL(&texture_veerMask, TEXTURE_DIR"Masks\\VeerMask.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_veerMask FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_veerMask Successfull = %u!!!\n", texture_veerMask);
	}
#endif // ENABLE_MASKSQUADS

//#ifdef ENABLE_STATIC_MODELS
//	//load models
//	loadStaticModel("res/models/scene09_veer/man/tempVeerMan.obj", &rockModel);
//	loadStaticModel("res/models/streetLight/StreetLight.obj", &streetLightModel);
//#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	//loadDynamicModel("res/models/skeleton/sadWalk.fbx", &skeletonModel);
	//loadDynamicModel("res/models/exo/Walking.dae", &skeletonModel);
	//loadDynamicModel("res/models/man/man.fbx", &skeletonModel);
	loadDynamicModel("res/models/scene09_veer/man/veerManAnim.fbx", &skeletonModel);
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_TERRIAN
	 displacementmap_depth = 150.0f;
	//displacementmap_depth = 1.5f;

	//terrainTextureVariables.albedoPath = TEXTURE_DIR"terrain/Scene09_Veer/dm_s11_02.jpg";
	terrainTextureVariables.albedoPath = TEXTURE_DIR"terrain/Scene09_Veer/snow_02_diff_8k.jpg";
	terrainTextureVariables.displacementPath = TEXTURE_DIR"terrain/Scene09_Veer/render1.png";
	terrainTextureVariables.normalPath = TEXTURE_DIR"terrain/Scene09_Veer/snow_02_nor_gl_8k.jpg";

	if (initializeTerrain(&terrainTextureVariables) != 0)
	{
		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}

#endif // ENABLE_TERRIAN

	displacementmap_depth = 15;

#ifdef ENABLE_CLOUD_NOISE
	noise_texture_veer_ras = initializeCloud();
	if (noise_texture_veer_ras == 0)
	{
		LOG("initializeCloud() - noise_texture_veer_ras FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeCloud() - noise_texture_veer_ras Successfull!!!\n");
	}
#endif // ENABLE_CLOUD_NOISE

	return 0;
}

void setCameraScene09_VeerRas(void)
{
	if (isInitialDisplayScene09_VeerRas == true)
	{
		//setCamera(0.0f, 0.0f, 6.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // Reset camera position for debugging
		setCamera(17.50f, 3.35f, -4.70f, 15.40f, 5.00f, -19.70f, 0.0f, 1.0f, 0.0f); // Initial postion for camera animation
		//setCamera(15.75f, 5.10f, -17.20f, -21.01f, -3.03f, -359.39f, 0.0f, 1.0f, 0.0f); // static camera position
		isInitialDisplayScene09_VeerRas = false;
	}
}

void displayScene09_VeerRas(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0)
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

	mat4 rotateX = mat4::identity();

	//displayCamera();

	if (isCameraRotation == false || stopCameraRotation == true)
		displayCamera();
	else
	{
		rotateCamera(15.40f, 4.99f, -19.70f, cameraRadius, cameraAngle, false);
	}

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
		finalViewMatrix = lookat(vec3(15.34f, 5.25f, -19.79f), vec3(-89.07f, -220.27f, 348.81f), vec3(0.0f, 1.0f, 0.0f));
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

		if (godRays == 1)
		{

//#ifdef ENABLE_ATMOSPHERE
//
//			translationMatrix = mat4::identity();
//			rotationMatrix = mat4::identity();
//			modelMatrix = mat4::identity();
//
//			//glBlendFunc(GL_ONE, GL_ONE);
//
//			atmosphereUniform_09 = useAtmosphereShader();
//
//			glUniform3f(atmosphereUniform_09.cameraPosUniform, cameraEyeX, cameraEyeY, cameraEyeZ);
//			glUniform3f(atmosphereUniform_09.lightPosUniform, atmosVariables_09.m_vLightDirection[0], atmosVariables_09.m_vLightDirection[1], atmosVariables_09.m_vLightDirection[2]);
//			glUniform3f(atmosphereUniform_09.invWavelengthUniform, 1 / atmosVariables_09.m_fWavelength4[0], 1 / atmosVariables_09.m_fWavelength4[1], 1 / atmosVariables_09.m_fWavelength4[2]);
//			glUniform1f(atmosphereUniform_09.cameraHeightUniform, sqrtf(cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ));
//			glUniform1f(atmosphereUniform_09.cameraHeight2Uniform, cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ);
//			glUniform1f(atmosphereUniform_09.innerRadiusUniform, atmosVariables_09.m_fInnerRadius);
//			glUniform1f(atmosphereUniform_09.innerRadius2Uniform, atmosVariables_09.m_fInnerRadius * atmosVariables_09.m_fInnerRadius);
//			glUniform1f(atmosphereUniform_09.outerRadiusUniform, atmosVariables_09.m_fOuterRadius);
//			glUniform1f(atmosphereUniform_09.outerRadius2Uniform, atmosVariables_09.m_fOuterRadius * atmosVariables_09.m_fOuterRadius);
//			glUniform1f(atmosphereUniform_09.KrESunUniform, atmosVariables_09.m_Kr * atmosVariables_09.m_ESun);
//			glUniform1f(atmosphereUniform_09.KmESunUniform, atmosVariables_09.m_Km * atmosVariables_09.m_ESun);
//			glUniform1f(atmosphereUniform_09.Kr4PIUniform, atmosVariables_09.m_Kr4PI);
//			glUniform1f(atmosphereUniform_09.Km4PIUniform, atmosVariables_09.m_Km4PI);
//			glUniform1f(atmosphereUniform_09.scaleUniform, 1.0f / (atmosVariables_09.m_fOuterRadius - atmosVariables_09.m_fInnerRadius));
//			glUniform1f(atmosphereUniform_09.scaleDepthUniform, atmosVariables_09.m_fRayleighScaleDepth);
//			glUniform1f(atmosphereUniform_09.scaleOverScaleDepthUniform, (1.0f / (atmosVariables_09.m_fOuterRadius - atmosVariables_09.m_fInnerRadius)) / atmosVariables_09.m_fRayleighScaleDepth);
//			glUniform1f(atmosphereUniform_09.gUniform, atmosVariables_09.m_g);
//			glUniform1f(atmosphereUniform_09.g2Uniform, atmosVariables_09.m_g * atmosVariables_09.m_g);
//
//			glUniformMatrix4fv(atmosphereUniform_09.modelMatrix, 1, GL_FALSE, modelMatrix);
//			glUniformMatrix4fv(atmosphereUniform_09.viewMatrix, 1, GL_FALSE, viewMatrix);
//			glUniformMatrix4fv(atmosphereUniform_09.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
//
//			displayAtmosphere();
//
//			glUseProgram(0);
//
//#endif // ENABLE_ATMOSPHERE

		} // if(godRays == 1)

#ifdef ENABLE_CLOUD_NOISE

		glEnable(GL_TEXTURE_3D);
		sceneVeerRasCloudNoiseUniform = useCloudNoiseShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		rotateX = mat4::identity();

		//translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
		translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f); // glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
		scaleMatrix = vmath::scale(100.0f, 100.0f, 100.0f);
		//rotateX = vmath::rotate(10.0f, 1.0f, 0.0f, 0.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotateX;

		//viewMatrix = vmath::lookat(camera.eye, camera.eye, camera.up);

		glUniformMatrix4fv(sceneVeerRasCloudNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneVeerRasCloudNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneVeerRasCloudNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform3fv(sceneVeerRasCloudNoiseUniform.laUniform, 1, lightAmbient);
		glUniform3fv(sceneVeerRasCloudNoiseUniform.ldUniform, 1, lightDiffuse);
		glUniform3fv(sceneVeerRasCloudNoiseUniform.lsUniform, 1, lightSpecular);
		glUniform4fv(sceneVeerRasCloudNoiseUniform.lightPositionUniform, 1, lightPosition);

		glUniform3fv(sceneVeerRasCloudNoiseUniform.kaUniform, 1, materialAmbient);
		glUniform3fv(sceneVeerRasCloudNoiseUniform.kdUniform, 1, materialDiffuse);
		glUniform3fv(sceneVeerRasCloudNoiseUniform.ksUniform, 1, materialSpecular);
		glUniform1f(sceneVeerRasCloudNoiseUniform.materialShininessUniform, materialShininess);

		glUniform1f(sceneVeerRasCloudNoiseUniform.scaleUniform, cloudMyScaleVeerRas);
		glUniform3fv(sceneVeerRasCloudNoiseUniform.skyColorUniform, 1, skyColorForVeerRas);
		glUniform3fv(sceneVeerRasCloudNoiseUniform.cloudColorUniform, 1, cloudColorForVeerRas);
		glUniform1f(sceneVeerRasCloudNoiseUniform.noiseScaleUniform, cloudNoiseScaleVeerRas);
		glUniform1i(sceneVeerRasCloudNoiseUniform.uniform_enable_godRays, godRays);
		//glUniform1f(sceneVeerRasCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noise_texture_veer_ras);

		float color[3] = { 1.0f, 1.0f, 1.0f };
		glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f, 1.0f, 1.0f));
		displaySphere(color);

		glUseProgram(0);

		glDisable(GL_TEXTURE_3D);

#endif // ENABLE_CLOUD_NOISE
	}

#ifdef ENABLE_TERRIAN
	// Terrain
	terrainUniform = useTerrainShader();

	vmath::mat4 mv_matrix = mat4::identity();
	vmath::mat4 proj_matrix = mat4::identity();

	//normal mapping
	vmath::mat4 m_matrix = (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));
	vmath::mat4 v_matrix = finalViewMatrix;

	mv_matrix = finalViewMatrix * m_matrix;

	proj_matrix = perspectiveProjectionMatrix;

	glUniformMatrix4fv(terrainUniform.modelMatrixUniform, 1, GL_FALSE, m_matrix);
	glUniformMatrix4fv(terrainUniform.viewMatrixUniform, 1, GL_FALSE, v_matrix);
	glUniformMatrix4fv(terrainUniform.projectionMatrixUniform, 1, GL_FALSE, proj_matrix);

	glUniform3fv(terrainUniform.laUniform, 1, lightAmbient);
	glUniform3fv(terrainUniform.ldUniform, 1, lightDiffuse);
	glUniform3fv(terrainUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(terrainUniform.lightPositionUniform, 1, lightPosition);

	glUniform3fv(terrainUniform.kaUniform, 1, materialAmbient);
	glUniform3fv(terrainUniform.ksUniform, 1, materialSpecular);
	glUniform1f(terrainUniform.materialShininessUniform, materialShininess);

	glUniformMatrix4fv(terrainUniform.uniform_mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);

	glUniform1f(terrainUniform.uniform_dmap_depth, 10);
	//glUniform1i(terrainUniform.uniform_enable_fog, enable_fog ? 1 : 0);
	//glUniform1i(terrainUniform.uniform_enable_fog, 0);
	glUniform1i(terrainUniform.uniform_enable_godRays, godRays);

	if (actualDepthQuadScene == 1)
	{

		glUniform1i(terrainUniform.actualSceneUniform, 0);
		glUniform1i(terrainUniform.depthSceneUniform, 1);
		glUniformMatrix4fv(terrainUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);

	}
	else
	{
		glUniform1i(terrainUniform.actualSceneUniform, 1);
		glUniform1i(terrainUniform.depthSceneUniform, 0);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
	}

#ifdef ENABLE_FOG
	glUniform1i(terrainUniform.fogEnableUniform, 1);
	glUniform1f(terrainUniform.densityUniform, density);
	glUniform1f(terrainUniform.gradientUniform, gradient);
	glUniform4fv(terrainUniform.skyFogColorUniform, 1, skyFogColor);
#endif // DEBUG - ENABLE_FOG

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.albedo);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.normal);
	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
#endif // ENABLE_TERRIAN


//#ifdef ENABLE_STATIC_MODELS
//	//MODELS
//	sceneOutdoorADSStaticUniform = useADSShader();
//
//	// Sending Light Related Uniforms
//	glUniform1i(sceneOutdoorADSStaticUniform.lightingEnableUniform, 1);
//	glUniform4fv(sceneOutdoorADSStaticUniform.laUniform, 1, lightAmbient);
//	glUniform4fv(sceneOutdoorADSStaticUniform.ldUniform, 1, lightDiffuse);
//	glUniform4fv(sceneOutdoorADSStaticUniform.lsUniform, 1, lightSpecular);
//	glUniform4fv(sceneOutdoorADSStaticUniform.lightPositionUniform, 1, lightPosition);
//	glUniform4fv(sceneOutdoorADSStaticUniform.kaUniform, 1, materialAmbient);
//	glUniform4fv(sceneOutdoorADSStaticUniform.kdUniform, 1, materialDiffuse);
//	glUniform4fv(sceneOutdoorADSStaticUniform.ksUniform, 1, materialSpecular);
//	glUniform1f(sceneOutdoorADSStaticUniform.materialShininessUniform, materialShininess);
//
//	glUniform1f(sceneOutdoorADSStaticUniform.colorCorrectionUniform, 0.7f);
//
//	//normal mapping
//	glUniform4fv(sceneOutdoorADSStaticUniform.viewpositionUniform, 1, camera.eye);
//
//	glUniform1i(sceneOutdoorADSStaticUniform.fogEnableUniform, 0);
//	glUniform1f(sceneOutdoorADSStaticUniform.densityUniform, density);
//	glUniform1f(sceneOutdoorADSStaticUniform.gradientUniform, gradient);
//	glUniform4fv(sceneOutdoorADSStaticUniform.skyFogColorUniform, 1, skyFogColor);
//	glUniform1i(sceneOutdoorADSStaticUniform.uniform_enable_godRays, godRays);
//	glUniform1i(sceneOutdoorADSStaticUniform.godrays_blackpass_sphere, 0);
//
//	//glUniform1i(sceneOutdoorADSStaticUniform.)
//	// ------ Rock Model ------
//	//translationMatrix = vmath::translate(2.0f, 2.0f, -6.0f);
//	translationMatrix = vmath::translate(15.40f, 5.00f, -19.70f);
//	scaleMatrix = vmath::scale(0.0045f, 0.0045f, 0.0045f);
//	rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
//
//	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
//
//	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
//	if (actualDepthQuadScene == 1)
//	{
//		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 0);
//		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 1);
//		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);
//
//	}
//	else
//	{
//		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 1);
//		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 0);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);
//	}
//	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
//	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
//
//	drawStaticModel(rockModel);
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	translationMatrix = mat4::identity();
//	rotationMatrix = mat4::identity();
//	modelMatrix = mat4::identity();
//	scaleMatrix = mat4::identity();
//	rotationMatrix_x = mat4::identity();
//	rotationMatrix_y = mat4::identity();
//	rotationMatrix_z = mat4::identity();
//
//	// ------ Streetlight Model ------
//	translationMatrix = vmath::translate(4.0f, 0.0f, -6.0f);
//	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
//
//	modelMatrix = translationMatrix * scaleMatrix;
//
//	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
//	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
//	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
//
//	drawStaticModel(streetLightModel);
//
#ifdef ENABLE_MASKSQUADS
	// Transformations - Quad For Mask
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(10.0f, 5.0f, -12.0f);					// glTranslatef() is replaced by this line.
	scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
	//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_veerMask);
	glUniform1i(sceneOutdoorADSStaticUniform.textureSamplerUniform_diffuse, 0);
		displayQuad();
#endif // ENABLE_MASKSQUADS
//
//
//	if (actualDepthQuadScene == 0)
//	{
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//
//	// Un-use ShaderProgramObject
//	glUseProgram(0);
//#endif // ENABLE_STATIC_MODELS

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

	// ------ Dancing Vampire Model ------
	// 15.50, 5.35, -19.70
	//glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(15.50f, 4.76f, -19.20f));
	glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(15.30f, model_y, -18.75f));
	// new location for dynamic model
	//glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(15.35f, 4.80f, -18.85f));
	glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.001f, 0.001f, 0.001f));
	//glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), 3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	// update_transformations_glm(&glm_translateMatrix, &glm_scaleMatrix, &glm_rotateMatrix);
	//update_transformations_glm(&glm_translateMatrix, &glm_scaleMatrix, &glm_rotateMatrix);
	glm_modelMatrix = glm_translateMatrix * glm_scaleMatrix * glm_rotateMatrix;

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

#ifdef ENABLE_CLOUD_NOISE
	// update Cloud
	updateCloud(cloudNoiseScaleIncrementVeerRas, cloudNoiseScaleVeerRas, 0.0001f);
#endif // ENABLE_CLOUD_NOISE

#endif
}

void updateScene09_VeerRas(void)
{
	/*cameraEyeX = preciselerp(cameraEyeX, 15.50f, 0.01f);
	cameraCenterX = preciselerp(cameraCenterX, -21.26f, 0.01f);
	
	cameraEyeY = preciselerp(cameraEyeY, 5.10f, 0.01f);
	cameraCenterY = preciselerp(cameraCenterY, -3.03f, 0.01f);
	
	cameraEyeZ = preciselerp(cameraEyeZ, -18.45f, 0.01f);
	cameraCenterZ = preciselerp(cameraCenterZ, -359.39f, 0.01f);*/

	/*[15.75, 6.85, -17.20], [-21.01, -75.48, -359.39]*/
	model_y += 0.001f;
	if (model_y >= 5.03f)
		model_y = 5.03f;

#ifdef ENABLE_CAMERA_ANIMATION
	if (isCameraRotation == false)
	{
		if (cameraEyeY <= 5.77f)
		{
			cameraEyeX = preciselerp(cameraEyeX, 15.75f, 0.002f);
			cameraCenterX = preciselerp(cameraCenterX, 15.40f, 0.002f);

			//cameraEyeY = preciselerp(cameraEyeY, 5.10f, 0.005f);
			cameraEyeY = preciselerp(cameraEyeY, 6.5f, 0.002f);
			cameraCenterY = preciselerp(cameraCenterY, 5.00f, 0.002f);

			cameraEyeZ = preciselerp(cameraEyeZ, -17.20f, 0.002f);
			cameraCenterZ = preciselerp(cameraCenterZ, -19.70f, 0.002f);

			/*cameraUpY = preciselerp(cameraUpY, 1.0f, 0.0002f);
			cameraUpZ = preciselerp(cameraUpZ, 0.0f, 0.0002f);*/

			// lookAt(16.19f, 5.71f, -14.07f, -38.36f, 13.19f, -357.79f, 0.00f, 0.55f, 0.45f)
			// lookAt(16.16f, 5.77f, -14.31f, -37.03f, 11.95f, -357.92f, 0.00f, 0.57f, 0.43f)
			// 
			// lookAt(15.34f, 6.50f, -15.70f, 15.40f, 4.99f, -19.70f, 0.00f, 1.00f, 0.00f)
		}
		else if (cameraEyeY > 5.77f)
		{
			/*isCameraRotation = true;
			cameraRadius = 5.39f;
			cameraUpY = 1.0f;
			cameraUpZ = 0.0f;*/
			//LOG("veer display camera lookat(%f, %f, %f, %f, %f, %f, %f, %f, %f)\n", cameraEyeX, cameraEyeY, cameraEyeZ, cameraCenterX, cameraCenterY, cameraCenterZ, cameraUpX, cameraUpY, cameraUpZ);
			cameraEyeX = preciselerp(cameraEyeX, 15.77f, 0.02f);
			cameraCenterX = preciselerp(cameraCenterX, 15.40f, 0.02f);

			//cameraEyeY = preciselerp(cameraEyeY, 5.10f, 0.005f);
			cameraEyeY = preciselerp(cameraEyeY, 5.77f, 0.02f);
			cameraCenterY = preciselerp(cameraCenterY, 4.99f, 0.02f);

			cameraEyeZ = preciselerp(cameraEyeZ, -14.32f, 0.02f);
			cameraCenterZ = preciselerp(cameraCenterZ, -19.70f, 0.02f);
		}
		if (cameraEyeX <= 15.90f && cameraEyeY > 5.77f)
		{
			isCameraRotation = true;
			cameraRadius = 5.39f;
			cameraUpY = 1.0f;
			cameraUpZ = 0.0f;
		}
	}
	else if (isCameraRotation == true && continueCameraRotation == true)
	{
		cameraRadius -= 0.005f;
		if (cameraRadius <= 2.0f)
			cameraRadius = 2.0f;

		if (cameraRotationCount == 0)
		{
			//cameraEyeY = 6.5f;
			cameraEyeY = 5.77f;
			cameraAngle += 0.3f;

			if (cameraAngle > 360.0f && cameraRotationCount == 0)
			{
				cameraAngle -= 360.0f;
				cameraRotationCount++;
			}
		}

		if (cameraRotationCount == 1)
		{
			cameraAngle = preciselerp(cameraAngle, 85.0f, 0.003f);
			/*if (cameraAngle >= 85.0f && cameraRotationCount == 1)
				continueCameraRotation = false;*/
		}
		
	}
	else
	{
		stopCameraRotation = true;
	}
#endif // ENABLE_CAMERA_ANIMATION
}

void uninitializeScene09_VeerRas(void)
{
	// Code
#ifdef ENABLE_MASKSQUADS
	if (texture_veerMask)
	{
		glDeleteTextures(1, &texture_veerMask);
		texture_veerMask = 0;
	}
#endif

//#ifdef ENABLE_STATIC_MODELS
//	//UNINIT models
//	unloadStaticModel(&rockModel);
//	unloadStaticModel(&streetLightModel);
//#endif // ENABLE_STATIC_MODELS


#ifdef ENABLE_DYNAMIC_MODELS
	unloadDynamicModel(&skeletonModel);
#endif
	//uninitializeCamera(&camera);

#ifdef ENABLE_CLOUD_NOISE
	uninitializeCloud();
	if (noise_texture_veer_ras)
	{
		glDeleteTextures(1, &noise_texture_veer_ras);
		noise_texture_veer_ras = 0;
	}
#endif // ENABLE_CLOUD_NOISE

}
