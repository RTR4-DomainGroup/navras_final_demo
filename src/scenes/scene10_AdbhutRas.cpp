// This File Will Be Replaced by Scene*.cpp

//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/FSQuadShader.h"

// billboarding config
#define BB_X_MIN (-40.0f)
#define BB_X_MAX (-10.0f)

// #define BB_Y_MIN (-3.0f)
// #define BB_Y_MAX (3.0f)

#define BB_Z_MIN (-50.0f)
#define BB_Z_MAX (50.0f)

#define BB_NO_OF_INSTANCES 5000

// #define X_INCREMENT 2.5f
// #define Y_INCREMENT 0.8f
// #define Z_INCREMENT -0.5f

#ifdef ENABLE_WATER
#include "../../inc/helper/waterframebuffer.h"
#include "../../inc/effects/WaterEffect.h"
#endif // ENABLE_WATER

#ifdef ENABLE_SHADOW
#include "../../inc/helper/shadowframebuffer.h"
#endif // ENABLE_SHADOW

#ifdef ENABLE_BILLBOARDING
#include "../../inc/shaders/BillboardingShader.h"
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_ATMOSPHERE
#include "../../inc/shaders/AtmosphereShader.h"
#include "../../inc/effects/AtmosphereEffect.h"
#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_TERRIAN
#include "../../inc/effects/TerrainEffect.h"
#endif // ENABLE_TERRIAN

#ifdef ENABLE_CLOUD_NOISE
#include "../../inc/effects/CloudEffect.h"
#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_STATIC_MODELS
#include "../../inc/effects/StaticModelLoadingEffect.h"
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_GAUSSIAN_BLUR
#include "../../inc/effects/GaussianBlurEffect.h"
#endif // ENABLE_GAUSSIAN_BLUR


#include "../../inc/scenes/scene10_AdbhutRas.h"


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern TEXTURE texture_grass;
extern TEXTURE texture_flower;

extern struct ADSUniform sceneOutdoorADSStaticUniform;
extern struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;

#ifdef ENABLE_TERRIAN
extern struct TerrainUniform terrainUniform;
#endif // ENABLE_TERRIAN

#ifdef ENABLE_CLOUD_NOISE
struct CloudNoiseUniform sceneAdbhutRasCloudNoiseUniform;
float cloudMyScaleAdbhutRas = 1.0f;
float cloudNoiseScaleAdbhutRas = 2.0f;
bool cloudNoiseScaleIncrementAdbhutRas = true;
GLuint noise_texture_adbhut_ras;
GLfloat skyColorForAdbhutRas[] = { 0.0f, 0.0f, 0.8f, 0.0f };
GLfloat cloudColorFOrAdbhutRas[] = { 0.8f, 0.8f, 0.8f, 1.0f };
#endif // ENABLE_CLOUD_NOISE


#ifdef ENABLE_BILLBOARDING
// variables for billboarding
extern struct BillboardingUniform billboardingEffectUniform;
extern GLuint frameTime; // = 0;
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_WATER
// Water Related Variables
extern struct WaterUniform waterUniform;
extern struct TextureVariables waterTextureVariables;
extern struct WaterFrameBufferDetails waterReflectionFrameBufferDetails;
extern struct WaterFrameBufferDetails waterRefractionFrameBufferDetails;
#endif // ENABLE_WATER

#ifdef ENABLE_GAUSSIAN_BLUR
// Gaussian Blur related variables
extern struct GaussianBlurEffect gaussianBlurEffect;
extern struct HorrizontalBlurUniform horizontalBlurUniform;
extern struct VerticalBlurUniform verticalBlurUniform;
extern struct FrameBufferDetails fullSceneFbo;
extern struct FSQuadUniform fsGaussBlurQuadUniform;
#endif // ENABLE_GAUSSIAN_BLUR


#ifdef ENABLE_ATMOSPHERE
// Atmosphere Scattering
extern AtmosphereUniform atmosphereUniform;
extern AtmosphericVariables atmosVariables;
#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_SHADOW
// Shadow
extern ShadowFrameBufferDetails shadowFramebuffer;
extern mat4 lightSpaceMatrix;
#endif // ENABLE_SHADOW

extern GLfloat waterHeight; // = 0.0f;
extern GLfloat moveFactor; // = 0.0f;
extern GLfloat planeReflection[]; // = { 0.0f, 1.0f, 0.0f, -waterHeight };
extern GLfloat planeRefration[]; // = { 0.0f, -1.0f, 0.0f, waterHeight }; 

extern GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;


#ifdef ENABLE_SKYBOX
// Variables For Skybox
extern GLuint texture_skybox;
extern struct SkyboxUniform sceneSkyBoxUniform;
#endif // ENABLE_SKYBOX

#ifdef ENABLE_STARFIELD
// Variables For Starfieldx
extern GLuint texture_star; 
extern double deltaTime;
extern struct StarfieldUniform sceneStarfieldUniform;
#endif // ENABLE_STARFIELD

extern GLfloat density; // = 0.15;
extern GLfloat gradient; // = 0.5;
extern GLfloat skyFogColor[]; // = { 0.25f, 0.25f, 0.25f, 1.0f };


#ifdef ENABLE_STATIC_MODELS
//Model variables
static STATIC_MODEL rockModel;
static STATIC_MODEL treeModel;
static STATIC_MODEL farmhouseModel;
static STATIC_MODEL adbhutmanModel;
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
static DYNAMIC_MODEL skeletonModel;

#endif // ENABLE_STATIC_MODELS

static struct TextureVariables terrainTextureVariables;
static TRANFORM displacementmap_depth; // onl

static GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 104.0f, 103.0f, 3.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

// Camera angle for rotation
static GLfloat cameraAngle = 85.0f;
static GLfloat cameraRadius;

static bool isCameraRotation = false;
static bool continueCameraRotation = true;
static bool stopCameraRotation = false;

static int activeObject = 0;

quad_instancing_buffers_t instBuffers;

float distance10;

bool isInitialDisplay_Scene10AdbhutRas = true;

GLuint texture_adbhutMask;

mat4 finalViewMatrix = mat4::identity();

int initializeScene10_AdbhutRas(void)
{
	// Function Declarations
	// -10.000000, -2.250000, -8.000000], [-10.193824, -3.229223, -8.059628] [0.000000, 1.000000, 0.000000
    
	// Code.
	// initializeCamera(&camera);

#ifdef ENABLE_MASKSQUADS
	initializeQuad();

	if (LoadGLTexture_UsingSOIL(&texture_adbhutMask, TEXTURE_DIR"Masks\\AdbhutMask.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_adbhutMask FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_adbhutRas Successfull = %u!!!\n", texture_adbhutMask);
	}
#endif

#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/rock/rock.obj", &rockModel);
	loadStaticModel("res/models/tree_adbhut/tree.fbx", &treeModel);
	loadStaticModel("res/models/farmhouse/farmhouse.obj", &farmhouseModel);
	loadStaticModel("res/models/scene10_adbhut/tempAdbhutMan.obj", &adbhutmanModel);
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	//loadDynamicModel("res/models/skeleton/sadWalk.fbx", &skeletonModel);
	//loadDynamicModel("res/models/exo/Walking.dae", &skeletonModel);
	loadDynamicModel("res/models/man/man.fbx", &skeletonModel);
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_BILLBOARDING
	
	
	GLfloat instance_positions[BB_NO_OF_INSTANCES * 4] = {};
	// generate positions per instance
	// for(int i = 0; i < BB_NO_OF_INSTANCES; i++)
	// {
	// 	instance_positions[(i*4)+0] = (((GLfloat)rand() / RAND_MAX) * (BB_X_MAX - BB_X_MIN)) + BB_X_MIN;
	// 	instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (BB_Y_MAX - BB_Y_MIN)) + BB_Y_MIN;
	// 	instance_positions[(i*4)+2] = (((GLfloat)rand() / RAND_MAX) * (BB_Z_MAX - BB_Z_MIN)) + BB_Z_MIN;
	// 	instance_positions[(i*4)+3] = 1.0f;
	// 	// LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
	// }

	// Bhuichakkar
	// for(int i = 0; i < BB_NO_OF_INSTANCES; i++)
	// {
	// 	instance_positions[(i*4)+0] =  (i%50 * sinf(i * M_PI/180));
	// 	instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (BB_Y_MAX - BB_Y_MIN)) + BB_Y_MIN;
	// 	instance_positions[(i*4)+2] =  (i%50 * cosf(i * M_PI/180));
	// 	instance_positions[(i*4)+3] = 1.0f;
	// 	// LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
	// }

	for(int i = 0; i < BB_NO_OF_INSTANCES; i++)
	{
		instance_positions[(i*4)+0] = ((((GLfloat)rand() / RAND_MAX) * (BB_X_MAX - BB_X_MIN)) + BB_X_MIN ) + (i%10 * sinf(i * M_PI/180));
		instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (BB_Y_MAX - BB_Y_MIN)) + BB_Y_MIN;
		instance_positions[(i*4)+2] = ((((GLfloat)rand() / RAND_MAX) * (BB_Z_MAX - BB_Z_MIN)) + BB_Z_MIN)  + (i%10 * cosf(i * M_PI/180));
		instance_positions[(i*4)+3] = 1.0f;
		// LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
	}

	// sort z vertices
	// for(int i = 0; i < BB_NO_OF_INSTANCES; i++)
	// {
	// 	for (int j = i + 1; j < BB_NO_OF_INSTANCES; ++j)
	// 	{
	// 		if(instance_positions[(i*4)+2] > instance_positions[(j*4)+2]) 
	// 		{
	// 			auto a = instance_positions[(i*4)+2];
	// 			instance_positions[(i*4)+2] = instance_positions[(j*4)+2];
	// 			instance_positions[(j*4)+2] = a; 
	// 		}
	// 	}
	// }

	initializeInstancedQuad(instBuffers, BB_NO_OF_INSTANCES, instance_positions);


#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_TERRIAN
	displacementmap_depth.y = 8.0f;
	// displacementmap_depth = 3.0f;
	// displacementmap_depth = 0.5f;

	terrainTextureVariables.albedoPath = TEXTURE_DIR"terrain/Scene10_Adbhut/aerial_grass_rock_diff_2k.jpg"; // albedo, color, diffuse, base color, are one and same
	terrainTextureVariables.displacementPath = TEXTURE_DIR"terrain/Scene10_Adbhut/test2.jpg";
	terrainTextureVariables.normalPath = TEXTURE_DIR"terrain/Scene10_Adbhut/aerial_grass_rock_nor_gl_2k.jpg";

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

#ifdef ENABLE_CLOUD_NOISE
	noise_texture_adbhut_ras = initializeCloud();
	if (noise_texture_adbhut_ras == 0)
	{
		LOG("initializeCloud() - noise_texture_adbhut_ras FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeCloud() - noise_texture_adbhut_ras Successfull!!!\n");
	}
#endif // ENABLE_CLOUD_NOISE

	return 0;
}

void setCameraScene10(void)
{
	if (isInitialDisplay_Scene10AdbhutRas == true)
	{
		// lookAt(17.20f, -0.70f, 6.05f, -134.67f, -0.70f, -333.86f, 0.00f, 1.00f, 0.00f)
		setCamera(17.20f, -0.70f, 6.05f, -134.67f, -0.70f, -333.86f, 0.00f, 1.00f, 0.00f);
		isInitialDisplay_Scene10AdbhutRas = false;
	}
}

void displayScene10_Passes(int godRays, bool recordWaterReflectionRefraction, bool isReflection, bool waterDraw, int actualDepthQuadScene) {

	// Code
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	TRANFORM rotationAngles = {0.0f, 0.0f, 0.0f};


	if (isCameraRotation == false || stopCameraRotation == true)
		displayCamera();
	else
	{
		rotateCamera(15.40f, 4.99f, -19.70f, cameraRadius, cameraAngle);
	}
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);

	if (actualDepthQuadScene == 0) {
	
		finalViewMatrix = viewMatrix;

	} else if(actualDepthQuadScene == 1) {

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

	if (actualDepthQuadScene == 0) {

		if (godRays == 1) {

#ifdef ENABLE_ATMOSPHERE

			translationMatrix = mat4::identity();
			rotationMatrix = mat4::identity();
			modelMatrix = mat4::identity();

			//glBlendFunc(GL_ONE, GL_ONE);

			atmosphereUniform = useAtmosphereShader();

			glUniform3f(atmosphereUniform.cameraPosUniform, cameraEyeX, cameraEyeY, cameraEyeZ);
			glUniform3f(atmosphereUniform.lightPosUniform, atmosVariables.m_vLightDirection[0], atmosVariables.m_vLightDirection[1], atmosVariables.m_vLightDirection[2]);
			glUniform3f(atmosphereUniform.invWavelengthUniform, 1 / atmosVariables.m_fWavelength4[0], 1 / atmosVariables.m_fWavelength4[1], 1 / atmosVariables.m_fWavelength4[2]);
			glUniform1f(atmosphereUniform.cameraHeightUniform, sqrtf(cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ));
			glUniform1f(atmosphereUniform.cameraHeight2Uniform, cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ);
			glUniform1f(atmosphereUniform.innerRadiusUniform, atmosVariables.m_fInnerRadius);
			glUniform1f(atmosphereUniform.innerRadius2Uniform, atmosVariables.m_fInnerRadius * atmosVariables.m_fInnerRadius);
			glUniform1f(atmosphereUniform.outerRadiusUniform, atmosVariables.m_fOuterRadius);
			glUniform1f(atmosphereUniform.outerRadius2Uniform, atmosVariables.m_fOuterRadius * atmosVariables.m_fOuterRadius);
			glUniform1f(atmosphereUniform.KrESunUniform, atmosVariables.m_Kr * atmosVariables.m_ESun);
			glUniform1f(atmosphereUniform.KmESunUniform, atmosVariables.m_Km * atmosVariables.m_ESun);
			glUniform1f(atmosphereUniform.Kr4PIUniform, atmosVariables.m_Kr4PI);
			glUniform1f(atmosphereUniform.Km4PIUniform, atmosVariables.m_Km4PI);
			glUniform1f(atmosphereUniform.scaleUniform, 1.0f / (atmosVariables.m_fOuterRadius - atmosVariables.m_fInnerRadius));
			glUniform1f(atmosphereUniform.scaleDepthUniform, atmosVariables.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniform.scaleOverScaleDepthUniform, (1.0f / (atmosVariables.m_fOuterRadius - atmosVariables.m_fInnerRadius)) / atmosVariables.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniform.gUniform, atmosVariables.m_g);
			glUniform1f(atmosphereUniform.g2Uniform, atmosVariables.m_g * atmosVariables.m_g);

			glUniformMatrix4fv(atmosphereUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(atmosphereUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
			glUniformMatrix4fv(atmosphereUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

			displayAtmosphere();

			glUseProgram(0);

#endif // ENABLE_ATMOSPHERE

		} // if(godRays == 1)

#ifdef ENABLE_CLOUD_NOISE

		glEnable(GL_TEXTURE_3D);
		sceneAdbhutRasCloudNoiseUniform = useCloudNoiseShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		rotationMatrix_x = mat4::identity();
		rotationMatrix_y = mat4::identity();
		rotationMatrix_z = mat4::identity();

		translationMatrix = vmath::translate(0.0f, 0.0f, 0.0f);
		scaleMatrix = vmath::scale(100.0f, 100.0f, 100.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.laUniform, 1, lightAmbient);
		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.ldUniform, 1, lightDiffuse);
		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.lsUniform, 1, lightSpecular);
		glUniform4fv(sceneAdbhutRasCloudNoiseUniform.lightPositionUniform, 1, lightPosition);

		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.kaUniform, 1, materialAmbient);
		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.kdUniform, 1, materialDiffuse);
		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.ksUniform, 1, materialSpecular);
		glUniform1f(sceneAdbhutRasCloudNoiseUniform.materialShininessUniform, materialShininess);

		glUniform1f(sceneAdbhutRasCloudNoiseUniform.scaleUniform, cloudMyScaleAdbhutRas);
		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.skyColorUniform, 1, skyColorForAdbhutRas);
		glUniform3fv(sceneAdbhutRasCloudNoiseUniform.cloudColorUniform, 1, cloudColorFOrAdbhutRas);
		glUniform1f(sceneAdbhutRasCloudNoiseUniform.noiseScaleUniform, cloudNoiseScaleAdbhutRas);
		glUniform1i(sceneAdbhutRasCloudNoiseUniform.uniform_enable_godRays, godRays);
		//glUniform1f(sceneAdbhutRasCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noise_texture_adbhut_ras);

		float color[3] = {1.0f, 1.0f, 1.0f};
		glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f,1.0f,1.0f));
		displaySphere(color);

		glUseProgram(0);

		glDisable(GL_TEXTURE_3D);

#endif // ENABLE_CLOUD_NOISE
	} // (actualDepthQuadScene == 0)

#ifdef ENABLE_TERRIAN
	// Terrain
	terrainUniform = useTerrainShader();

	vmath::mat4 mv_matrix = mat4::identity();
	vmath::mat4 proj_matrix = mat4::identity();

	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};

	modelMatrix = mat4::identity();


	//normal mapping
	translationMatrix = vmath::translate(-0.25f, -4.0f, -20.0f);
	if(1 == tf_Object) // terrain
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
	modelMatrix = translationMatrix * scaleMatrix;

	// viewMatrix = finalViewMatrix;
	mv_matrix = finalViewMatrix * modelMatrix;
	proj_matrix = perspectiveProjectionMatrix;

	glUniformMatrix4fv(terrainUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(terrainUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(terrainUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

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

	// update_transformations(NULL, NULL, NULL, &displacementmap_depth);
	glUniform1f(terrainUniform.uniform_dmap_depth, displacementmap_depth.y);
	// glUniform1f(terrainUniform.uniform_dmap_depth, rotationAngles.y);
	//glUniform1i(terrainUniform.uniform_enable_fog, enable_fog ? 1 : 0);
	//glUniform1i(terrainUniform.uniform_enable_fog, 0);
	glUniform1i(terrainUniform.uniform_enable_godRays, godRays);

	if (actualDepthQuadScene == 1) {

		glUniform1i(terrainUniform.actualSceneUniform, 0);
		glUniform1i(terrainUniform.depthSceneUniform, 1);
		glUniformMatrix4fv(terrainUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);

	}
	else {

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

	glUniform1i(sceneOutdoorADSStaticUniform.fogEnableUniform, 0);
	glUniform1f(sceneOutdoorADSStaticUniform.densityUniform, density);
	glUniform1f(sceneOutdoorADSStaticUniform.gradientUniform, gradient);
	glUniform4fv(sceneOutdoorADSStaticUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneOutdoorADSStaticUniform.uniform_enable_godRays, godRays);
	glUniform1i(sceneOutdoorADSStaticUniform.godrays_blackpass_sphere, 0);

    // once for all static models drawing
 	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ------ Rock Model ------
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};

	translationMatrix = vmath::translate(-6.80f, -3.5f, -15.88f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	
	// usage type 2
	if(2 == tf_Object) // Rock model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles);

	rotationMatrix_x = vmath::rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	rotationMatrix_z = vmath::rotate(rotationAngles.z, 1.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

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

	drawStaticModel(rockModel);

	// ------ Tree Model ------
	modelMatrix = mat4::identity();
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	translationMatrix = vmath::translate(-20.75f, -2.27f, -34.25f);
	scaleMatrix = vmath::scale(0.59f, 0.59f, 0.59f);

	// usage type 1 
	if(3 == tf_Object) // Tree model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix) ;
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	drawStaticModel(treeModel);


	// ------ farmhouse Model ------
	modelMatrix = mat4::identity();
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};

	translationMatrix = vmath::translate(19.00f, -2.15f, -2.75f);
	scaleMatrix = vmath::scale(0.7f, 0.7f, 0.7f);
	rotationAngles = {0.0f, -59.25f, 0.0f};

	// usage type 1 
	if(4 == tf_Object) // farmhouse model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
	rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	drawStaticModel(farmhouseModel);


	// ------ adbhutmanModel Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};

	translationMatrix = vmath::translate(-19.94f, -2.18f, -34.25f);
	scaleMatrix = vmath::scale(0.05f, 0.05f, 0.05f);
	rotationAngles = {0.0f, 99.55f, 0.0f};

	// usage type 1 
	if(5 == tf_Object) // adbhutman model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
	rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	drawStaticModel(adbhutmanModel);

#ifdef ENABLE_MASKSQUADS
	// Quad For Mask
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	translationMatrix = vmath::translate(2.0f, 1.0f, -3.0f);
	scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
	//rotationMatrix = vmath::rotate(72.45f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_adbhutMask);
	glUniform1i(sceneOutdoorADSStaticUniform.textureSamplerUniform_diffuse, 0);
	displayQuad();
#endif // ENABLE_MASKSQUADS

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

	glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSDynamicUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

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

	drawDynamicModel(sceneOutdoorADSDynamicUniform, skeletonModel, 1.0f);

	glUseProgram(0);

#endif

#ifdef ENABLE_WATER

	if(waterDraw == true){
		waterUniform = useWaterShader();

		if (recordWaterReflectionRefraction == true)
		{
			if (isReflection == true)
			{
				distance10 = 2.0f * (cameraEyeY - waterHeight);
				cameraEyeY -= distance10;
				cameraCenterY -= distance10;
				displayCamera();
				finalViewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
				glUniform4fv(waterUniform.planeUniform, 1, planeReflection);
			}
			else
			{
				glUniform4fv(waterUniform.planeUniform, 1, planeRefration);
			}
		}

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(-2.00f, -3.00f, -3.00f);
		scaleMatrix = vmath::scale(50.0f, 1.0f, 50.0f);

		if(6 == tf_Object) // Water
			update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix) ;
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

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

		glUniform1f(waterUniform.uniform_waveStrength, 0.04f);
		glUniform4fv(waterUniform.uniform_watercolor, 1, vec4(0.0f, 0.3f, 0.5f, 1.0));

		glUniform1f(waterUniform.uniform_enable_godRays, godRays);

		displayWater();

		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);
    }

#endif // ENABLE_WATER	

#ifdef ENABLE_BILLBOARDING	
	if (actualDepthQuadScene == 0) { // 0 - Actual Scene, 1 - Depth scene

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		billboardingEffectUniform = useBillboardingShader();

		// send to shader
		glUniformMatrix4fv(billboardingEffectUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(billboardingEffectUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
		glUniform1i(billboardingEffectUniform.textureSamplerUniform, 0);
		glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 1);
		glUniform1i(billboardingEffectUniform.frameTimeUniform, frameTime);
		glUniform1i(billboardingEffectUniform.uniform_enable_godRays, godRays);

		/// grass
		/// /////////////////////////
		/// Flower on left side of river
		/// /////////////////////////
		/// 

		// instanced quads with grass texture
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		rotationAngles = {0.0f, 0.0f, 0.0f};

		// Red flower
		if (texture_grass.height > texture_grass.width)
			scaleMatrix = vmath::scale(texture_grass.width / (GLfloat)texture_grass.height, 1.0f, 1.0f);
		else
			scaleMatrix = vmath::scale(1.0f, texture_grass.height / (GLfloat)texture_grass.width, 1.0f);


		translationMatrix = vmath::translate(-5.00f, -2.15f, -13.75f);
		scaleMatrix *= vmath::scale(0.65f, 0.65f, 0.65f);
		// rotationAngles = { 0.50f, 18.75f, 2.00f};

		// if(7 == tf_Object) // Red Flower
		// 	update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
		rotationMatrix_x = vmath::rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);	
		rotationMatrix_y = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);	
		rotationMatrix_z = vmath::rotate(rotationAngles.z, 0.0f, 0.0f, 1.0f);
		rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_grass.id);
		displayInstancedQuads(instBuffers, BB_NO_OF_INSTANCES);  // how many instances to draw
		glBindTexture(GL_TEXTURE_2D, 0);


		/// White Flower
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationAngles = {0.0f, 0.0f, 0.0f};
		
		if (texture_flower.height > texture_flower.width)
			scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
		else
			scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

		translationMatrix = vmath::translate(-4.75f, -1.90f, -13.50f);
		scaleMatrix *= vmath::scale(0.65f, 0.65f, 0.65f);

		// if(8 == tf_Object) // White Flower
		// 	update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
		rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);	
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		// send to shader
		glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_flower.id);
		displayInstancedQuads(instBuffers, BB_NO_OF_INSTANCES);  // how many instances to draw
		glBindTexture(GL_TEXTURE_2D, 0);


		/// /////////////////////////
		/// Flower on right side of river
		/// /////////////////////////
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		rotationAngles = {0.0f, 0.0f, 0.0f};

		// Red flower
		if (texture_grass.height > texture_grass.width)
			scaleMatrix = vmath::scale(texture_grass.width / (GLfloat)texture_grass.height, 1.0f, 1.0f);
		else
			scaleMatrix = vmath::scale(1.0f, texture_grass.height / (GLfloat)texture_grass.width, 1.0f);

		translationMatrix = vmath::translate(29.50f, -2.15f, -21.70f);
		scaleMatrix *= vmath::scale(0.65f, 0.65f, 0.65f);
		// rotationAngles = { 0.50f, 18.75f, 2.00f};

		if(7 == tf_Object) // Red Flower
			update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;

		rotationMatrix_x = vmath::rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);	
		rotationMatrix_y = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);	
		rotationMatrix_z = vmath::rotate(rotationAngles.z, 0.0f, 0.0f, 1.0f);
		rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_grass.id);
		displayInstancedQuads(instBuffers, BB_NO_OF_INSTANCES);  // how many instances to draw
		glBindTexture(GL_TEXTURE_2D, 0);


		/// White Flower
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationAngles = {0.0f, 0.0f, 0.0f};
		
		if (texture_flower.height > texture_flower.width)
			scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
		else
			scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

		translationMatrix = vmath::translate(33.00f, -2.05f, -27.75f);
		scaleMatrix *= vmath::scale(0.65f, 0.65f, 0.65f);
		rotationAngles = {0.0f, 17.0f, 0.0f};

		if(8 == tf_Object) // White Flower
			update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
		rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);	
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		// send to shader
		glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_flower.id);
		displayInstancedQuads(instBuffers, BB_NO_OF_INSTANCES);  // how many instances to draw
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);
		glDisable(GL_BLEND);

	}
#endif // ENABLE_BILLBOARDING
}

void updateScene10_AdbhutRas(void)
{
	// Code

#ifdef ENABLE_CAMERA_ANIMATION
	//TRANFORM speedVector = {0.0f, 0.0f, 0.0f};
	//speedVector.x = 0.04;
	//// update_transformations(NULL, NULL, NULL, &speedVector);
	//cameraEyeZ -= speedVector.x;
	//cameraCenterZ -= speedVector.x;

	// lookAt(-17.10f, -1.40f, -33.85f, -320.07f, -1.40f, -177.19f, 0.00f, 1.00f, 0.00f)
	cameraEyeX = preciselerp(cameraEyeX, -17.10f, 0.001f);
	cameraEyeY = preciselerp(cameraEyeY, -1.40f, 0.001f);
	cameraEyeZ = preciselerp(cameraEyeZ, -33.85f, 0.001f);

	cameraCenterX = preciselerp(cameraCenterX, -320.07f, 0.001f);
	cameraCenterY = preciselerp(cameraCenterY, -1.40f, 0.001f);
	cameraCenterZ = preciselerp(cameraCenterZ, -177.19f, 0.001f);

#endif

#ifdef ENABLE_BILLBOARDING
	frameTime += 1;

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_CLOUD_NOISE
	// update Cloud
	updateCloud(cloudNoiseScaleIncrementAdbhutRas, cloudNoiseScaleAdbhutRas, 0.0001f);
#endif // ENABLE_CLOUD_NOISE
}

void uninitializeScene10_AdbhutRas(void)
{
	// Code
#ifdef ENABLE_BILLBOARDING
    uninitializeInstancedQuads(instBuffers);

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariables);
#endif // ENABLE_TERRIAN

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&farmhouseModel);
	unloadStaticModel(&rockModel);
	unloadStaticModel(&treeModel);
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_MASKSQUADS
	if (texture_adbhutMask)
	{
		glDeleteTextures(1, &texture_adbhutMask);
		texture_adbhutMask = 0;
	}
#endif // ENABLE_MASKSQUADS



#ifdef ENABLE_DYNAMIC_MODELS
	unloadDynamicModel(&skeletonModel);
#endif
	//uninitializeCamera(&camera);

#ifdef ENABLE_CLOUD_NOISE
	uninitializeCloud();
	if (noise_texture_adbhut_ras)
	{
		glDeleteTextures(1, &noise_texture_adbhut_ras);
		noise_texture_adbhut_ras = 0;
	}
#endif // ENABLE_CLOUD_NOISE

}

//////////////


// static bool firstcall = 1;
// if(firstcall)
// {
// 	LOG("Before update tranform\n");
// 	print_matrices(translationMatrix, scaleMatrix, rotationMatrix, rotationAngles);
// }

// if(firstcall)
// {
// 	LOG("After update tranform\n");
// 	print_matrices(translationMatrix, scaleMatrix, rotationMatrix, rotationAngles);
// 	firstcall = 0;
// }


