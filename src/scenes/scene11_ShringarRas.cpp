// This File Will Be Replaced by Scene*.cpp

//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/debug/debug_transformation.h"


#ifdef ENABLE_WATER
#include "../../inc/helper/waterframebuffer.h"
#endif // ENABLE_WATER

#ifdef ENABLE_SHADOW
#include "../../inc/helper/shadowframebuffer.h"
#endif // ENABLE_SHADOW

#undef ENABLE_BILLBOARDING

#ifdef ENABLE_BILLBOARDING
#include "../../inc/shaders/BillboardingShader.h"
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_ATMOSPHERE
#include "../../inc/shaders/AtmosphereShader.h"
#include "../../inc/effects/AtmosphereEffect.h"
#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_VIDEO_RENDER
#include "../../inc/effects/videoEffect.h"
#endif // ENABLE_VIDEO_RENDER

#ifdef ENABLE_TERRIAN
#include "../../inc/effects/TerrainEffect.h"
#endif // ENABLE_TERRIAN

#ifdef ENABLE_STARFIELD
#include "../../inc/effects/StarfieldEffect.h"
#endif // ENABLE_STARFIELD

#ifdef ENABLE_CLOUD_NOISE
#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_SKYBOX
#include "../../inc/effects/SkyboxEffect.h"
#endif // ENABLE_SKYBOX

#ifdef ENABLE_CLOUD_NOISE
#include "../../inc/effects/CloudEffect.h"
#endif // ENABLE_CLOUD_NOISE

#ifdef ENABLE_WATER
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


#include "../../inc/scenes/scene11_ShringarRas.h"



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
extern struct CloudNoiseUniform sceneCloudNoiseUniform;
#endif // ENABLE_CLOUD_NOISE

static struct TextureVariables terrainTextureVariables;

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
extern mat4 viewmatrixDepth;
extern mat4 lightSpaceMatrix;
extern mat4 perspectiveProjectionDepth;
#endif // ENABLE_SHADOW

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

#ifdef ENABLE_STATIC_MODELS
//Model variables
STATIC_MODEL treemodel_11;
STATIC_MODEL manModel_11;
STATIC_MODEL womanModel_11;

#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
DYNAMIC_MODEL skeletonModel_11;

#endif // ENABLE_STATIC_MODELS

extern GLfloat density; // = 0.15;
extern GLfloat gradient; // = 0.5;
extern GLfloat skyFogColor[]; // = { 0.25f, 0.25f, 0.25f, 1.0f };


// Camera angle for rotation
GLfloat cameraAngle_shringar = 90.0f; // = 0.0f;
GLfloat cameraRadius_shringar = 18.0f;
extern GLfloat dispersal; // = 0.1875f;
extern GLfloat haloWidth; // = 0.45f;
extern GLfloat intensity; // = 1.5f;
extern GLfloat distortion[]; // = { 0.94f, 0.97f, 1.0f };

static GLfloat lightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 104.0f, 103.0f, 3.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

float distance11;

bool isInitialDisplayScene11_ShringarRas = true;

GLuint texture_shringarMask;

int initializeScene11_ShringarRas(void)
{

	// external debugging varaible
	tf_t = { -1.5f, -1.0f, 0.0f }; // tree pos 
	// tf_s = {0.75f, 0.75f, 0.75f}; // tree scale 
	// tf_r = {0.0f, 0.0f, 0.0f}; // tree rotation 
	//tf_Speed = 0.05f;

    // Code.
	// initializeCamera(&camera);

#ifdef ENABLE_MASKSQUADS
	initializeQuad();

	if (LoadGLTexture_UsingSOIL(&texture_shringarMask, TEXTURE_DIR"Masks/ShringarMask.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_shringarMask FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_shringarMask Successfull = %u!!!\n", texture_shringarMask);
	}
#endif

#ifdef ENABLE_STATIC_MODELS
	//load models
	//loadStaticModel("res/models/tree_shringar/Shelf.obj", &rockModel_11);
	loadStaticModel("res/models/tree_shringar/palmTree.obj", &treemodel_11);
	loadStaticModel("res/models/scene11_shringar/man/tempShringrMan.obj", &manModel_11);
	loadStaticModel("res/models/scene11_shringar/woman/tempShringarWoman.obj", &womanModel_11);
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	//loadDynamicModel("res/models/skeleton/sadWalk.fbx", &skeletonModel_11);
	//loadDynamicModel("res/models/exo/Walking.dae", &skeletonModel_11);
	loadDynamicModel("res/models/man/man.fbx", &skeletonModel_11);
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_TERRIAN
	displacementmap_depth = 5.0f;

	terrainTextureVariables.albedoPath = TEXTURE_DIR"terrain/Scene11_Shringar/1diffuse14.png";
	terrainTextureVariables.displacementPath = TEXTURE_DIR"terrain/Scene11_Shringar/1disp.jpg";
	terrainTextureVariables.normalPath = TEXTURE_DIR"terrain/Scene11_Shringar/normal.jpg";

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


#ifdef ENABLE_BILLBOARDING
	GLfloat instance_positions[NO_OF_INSTANCES * 4] = {};
	// generate positions per instance
	for(int i = 0; i < NO_OF_INSTANCES; i++)
	{
		instance_positions[(i*4)+0] = (((GLfloat)rand() / RAND_MAX) * (X_MAX - X_MIN)) + X_MIN;
		instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (Y_MAX - Y_MIN)) + Y_MIN;
		instance_positions[(i*4)+2] = (((GLfloat)rand() / RAND_MAX) * (Z_MAX - Z_MIN)) + Z_MIN;
		instance_positions[(i*4)+3] = 1.0f;
		// LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
	}

	// sort z vertices
	for(int i = 0; i < NO_OF_INSTANCES; i++)
	{
		for (int j = i + 1; j < NO_OF_INSTANCES; ++j)
		{
			if(instance_positions[(i*4)+2] > instance_positions[(j*4)+2]) 
			{
				auto a = instance_positions[(i*4)+2];
				instance_positions[(i*4)+2] = instance_positions[(j*4)+2];
				instance_positions[(j*4)+2] = a; 
			}
		}
	}

	initializeInstancedQuad(NO_OF_INSTANCES, instance_positions);


#endif // ENABLE_BILLBOARDING


	return 0;
}

void setCameraScene11_ShringarRas(void)
{
	if (isInitialDisplayScene11_ShringarRas == true)
	{
		//setCamera(0.0, 1.5f, 6.0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		setCamera(0.0, 0.25f, 6.0, 0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f);

		//setCamera(0.00, 0.00, -12.50, 0.00f, 0.00f, -18.50f, 0.0f, 0.0f, 1.0f);
		isInitialDisplayScene11_ShringarRas = false;
	}
}

void displayScene11_ShringarRas(int godRays = 1, bool recordWaterReflectionRefraction = false, bool isReflection = false, bool waterDraw = false, int actualDepthQuadScene = 0) {

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


	//rotateCamera(0.0f, 0.8f, -12.85f, cameraRadius_shringar, cameraAngle_shringar);
	rotateCamera(0.0f, 0.6f, -15.0f, cameraRadius_shringar, cameraAngle_shringar);
	//lookAt([0.00, 1.25, 6.00], [0.00, 1.25, 0.00] [0.00, 1.00, 0.00])
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	//setCamera(&camera);
	//displayCamera();

	mat4 finalViewMatrix = mat4::identity();

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

	//lightPosition[0] = cameraEyeX;
	//lightPosition[1] = cameraEyeY;
	//lightPosition[2] = cameraEyeZ;


	if (recordWaterReflectionRefraction == true) {

#ifdef ENABLE_WATER
		waterUniform = useWaterShader();

		
		if (isReflection == true) {
			distance11 = 2.0f * (cameraEyeY - waterHeight);
			glUniform4fv(waterUniform.planeUniform, 1, planeReflection);
			cameraEyeY -= distance11;
			cameraCenterY -= distance11;
			displayCamera();
			//setCamera(&camera);
			finalViewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
			//setCamera(&camera);
			glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		}

		if (isReflection == false) {
			glUniform4fv(waterUniform.planeUniform, 1, planeRefration);
			glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		}
#endif // ENABLE_WATER
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
		sceneCloudNoiseUniform = useCloudNoiseShader();

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

		glUniformMatrix4fv(sceneCloudNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneCloudNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneCloudNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform3fv(sceneCloudNoiseUniform.laUniform, 1, lightAmbient);
		glUniform3fv(sceneCloudNoiseUniform.ldUniform, 1, lightDiffuse);
		glUniform3fv(sceneCloudNoiseUniform.lsUniform, 1, lightSpecular);
		glUniform4fv(sceneCloudNoiseUniform.lightPositionUniform, 1, lightPosition);

		glUniform3fv(sceneCloudNoiseUniform.kaUniform, 1, materialAmbient);
		glUniform3fv(sceneCloudNoiseUniform.kdUniform, 1, materialDiffuse);
		glUniform3fv(sceneCloudNoiseUniform.ksUniform, 1, materialSpecular);
		glUniform1f(sceneCloudNoiseUniform.materialShininessUniform, materialShininess);

		glUniform1f(sceneCloudNoiseUniform.scaleUniform, myScale);
		glUniform4fv(sceneCloudNoiseUniform.skyColorUniform, 1, skyColor);
		glUniform4fv(sceneCloudNoiseUniform.cloudColorUniform, 1, cloudColor);
		glUniform1f(sceneCloudNoiseUniform.noiseScaleUniform, noiseScale);
		glUniform1i(sceneCloudNoiseUniform.uniform_enable_godRays, godRays);
		//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);
		glEnable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, noise_texture);

		float color[3] = {1.0f, 1.0f, 1.0f};
		glVertexAttrib3fv(DOMAIN_ATTRIBUTE_COLOR, vec3(1.0f,1.0f,1.0f));
		displaySphere(color);
		glDisable(GL_BLEND);

		glUseProgram(0);

		glDisable(GL_TEXTURE_3D);

#endif // ENABLE_CLOUD_NOISE

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

		translationMatrix = vmath::translate(0.0f, 0.0f, -56.0f);					// glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

		glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);
		glUniform1i(sceneStarfieldUniform.uniform_enable_godRays, godRays);
		glUniform1f(sceneStarfieldUniform.timeUniform, time);

		displayStarfield(texture_star);
		glUseProgram(0);

#endif // ENABLE_STARFIELD

#ifdef ENABLE_SKYBOX

		sceneSkyBoxUniform = useSkyboxShader();

		// Transformations
		translationMatrix = vmath::translate(0.0f, 0.0f, -10.0f);					// glTranslatef() is replaced by this line.
		scaleMatrix = vmath::scale(30.0f, 30.0f, 30.0f);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(sceneSkyBoxUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneSkyBoxUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(sceneSkyBoxUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

		displaySkybox(texture_skybox);
		glUseProgram(0);
#endif // ENABLE_SKYBOX
	}

#ifdef ENABLE_TERRIAN
	// Terrain
	terrainUniform = useTerrainShader();

	vmath::mat4 mv_matrix = mat4::identity();
	vmath::mat4 proj_matrix = mat4::identity();

	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	translationMatrix = translate(0.0f, -0.67f, -20.0f);
	scaleMatrix = scale(1.0f, 1.0f, 1.0f);


	//update_transformations(&translationMatrix, &scaleMatrix, NULL);

	//normal mapping
	vmath::mat4 m_matrix = (translationMatrix * scaleMatrix);
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

	glUniform1f(terrainUniform.uniform_dmap_depth, displacementmap_depth);
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

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.albedo);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.normal);
	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_BLEND);

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

	//glUniform1i(sceneOutdoorADSStaticUniform.)
	// ------ TREE Model ------

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.25f, 0.35f, -17.0f);
	scaleMatrix = vmath::scale(0.01f, 0.01f, 0.01f);

	update_transformations(&translationMatrix, &scaleMatrix, NULL);

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

	drawStaticModel(treemodel_11);

	// Man
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(-0.06f, 0.03f, -15.0f);
	scaleMatrix = vmath::scale(0.0065f, 0.0065f, 0.0065f);

	//update_transformations(&translationMatrix, &scaleMatrix, NULL);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	if (actualDepthQuadScene == 1) {

		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 0);
		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 1);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);

	}
	else {

		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 1);
		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);

	}

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(manModel_11);

	// Woman
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.04f, 0.02f, -15.0f);
	scaleMatrix = vmath::scale(0.0065f, 0.0065f, 0.0065f);

	//update_transformations(&translationMatrix, &scaleMatrix, NULL);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	if (actualDepthQuadScene == 1) {

		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 0);
		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 1);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);

	}
	else {

		glUniform1i(sceneOutdoorADSStaticUniform.actualSceneUniform, 1);
		glUniform1i(sceneOutdoorADSStaticUniform.depthSceneUniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);

	}

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(womanModel_11);

#ifdef ENABLE_MASKSQUADS
	// Transformations - Quad For Mask
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(5.0f, 5.0f, -12.0f);					// glTranslatef() is replaced by this line.
	scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
	//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_shringarMask);
	glUniform1i(sceneOutdoorADSStaticUniform.textureSamplerUniform_diffuse, 0);
	displayQuad();
#endif // ENABLE_MASKQ

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

	// ------ Dancing Vampire Model ------

	glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.025f, -12.85f));
	glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.0008f, 0.0008f, 0.0008f));
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

	drawDynamicModel(sceneOutdoorADSDynamicUniform, skeletonModel_11, 1.0f);

	glUseProgram(0);

#endif

#ifdef ENABLE_WATER
	if(waterDraw == true){
		waterUniform = useWaterShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(0.0f, 0.0f, -20.0f);

		scaleMatrix = vmath::scale(180.0f, 1.0f, 180.0f);

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

		glUniform1f(waterUniform.uniform_waveStrength, 0.04f);
		glUniform4fv(waterUniform.uniform_watercolor, 1, vec4(0.0f, 0.0f, 0.0f, 1.0));

		glUniform1f(waterUniform.uniform_enable_godRays, godRays);

		displayWater();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
    }

#endif // ENABLE_WATER

#ifdef ENABLE_BILLBOARDING	
	if (actualDepthQuadScene == 0) { // 0 - Actual Scene, 1 - Depth scene
		void displayScene11_Billboarding(int);

		displayScene11_Billboarding(godRays);	
	}
#endif // ENABLE_BILLBOARDING

	if (isReflection == true) {

		cameraEyeY += distance11;
		cameraCenterY += distance11;
		displayCamera();
		finalViewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	}

}

#ifdef ENABLE_BILLBOARDING
void displayScene11_Billboarding(int godRays = 1)
{
	// variable declaration
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	
	// code
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	billboardingEffectUniform = useBillboardingShader();

	// instanced quads with grass texture
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(billboardingEffectUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	/// Grass
	if (texture_grass.height > texture_grass.width)
		scaleMatrix = vmath::scale(texture_grass.width / (GLfloat)texture_grass.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_grass.height / (GLfloat)texture_grass.width, 1.0f);

	translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniform1i(billboardingEffectUniform.textureSamplerUniform, 0);
	glUniform1i(billboardingEffectUniform.billboardingEnableUniform, 1);
	glUniform1i(billboardingEffectUniform.frameTimeUniform, frameTime);
	glUniform1i(billboardingEffectUniform.uniform_enable_godRays, godRays);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_grass.id);
	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw
	glBindTexture(GL_TEXTURE_2D, 0);


	/// Flower
	if (texture_flower.height > texture_flower.width)
		scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

	//translationMatrix = vmath::translate(-1.5f, 1.0f, 0.0f);
	translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	// scaleMatrix = vmath::scale(tf_s.x, tf_s.x, tf_s.x);
	// rotationMatrix_x = vmath::rotate(tf_r.x, 1.0f, 0.0f, 0.0f);
	// rotationMatrix_y = vmath::rotate(tf_r.y, 0.0f, 1.0f, 0.0f);
	// rotationMatrix_z = vmath::rotate(tf_r.z, 1.0f, 0.0f, 1.0f);
	// rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// send to shader
	glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_flower.id);
	displayInstancedQuads(NO_OF_INSTANCES);  // how many instances to draw
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
	glDisable(GL_BLEND);

}
#endif // ENABLE_BILLBOARDING	

void updateScene11_ShringarRas(void)
{
	// Code
#ifdef ENABLE_CAMERA_ANIMATION
	////lookAt([0.00, 5.75, -17.25], [0.00, 5.75, -23.25][0.00, 1.00, 0.00])
	//cameraEyeZ = preciselerp(cameraEyeZ, -17.25f, 0.005f);
	//cameraCenterZ = preciselerp(cameraCenterZ, -23.25f, 0.005f);

	//cameraEyeY = impreciselerp(cameraEyeY, 5.75f, 0.005f);
	//cameraCenterY = impreciselerp(cameraCenterY, 5.75f, 0.005f);

	cameraAngle_shringar += 0.14f;
	if (cameraAngle_shringar >= 270.0f)
		cameraAngle_shringar = 270.0f;
	//cameraAngle_shringar = preciselerp(cameraAngle_shringar, 270.f, 0.07f);

	cameraRadius_shringar -= 0.022f;
	if (cameraRadius_shringar <= 2.0f)
		cameraRadius_shringar = 2.0f;
	//cameraRadius_shringar = preciselerp(cameraRadius_shringar, 2.0f, 0.018f);

	cameraEyeY = preciselerp(cameraEyeY, 0.4f, 0.0025f);

#endif

#ifdef ENABLE_BILLBOARDING
	frameTime += 1;

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_WATER

	moveFactor += 0.0003f;
	if (moveFactor >= 360.0f)
		moveFactor -= 360.0f;
#endif // ENABLE_WATER
}

void uninitializeScene11_ShringarRas(void)
{
	// Code
#ifdef ENABLE_BILLBOARDING
    uninitializeInstancedQuads();

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariables);
#endif // ENABLE_TERRIAN

#ifdef ENABLE_MASKSQUADS
	if (texture_shringarMask)
	{
		glDeleteTextures(1, &texture_shringarMask);
		texture_shringarMask = 0;
	}
#endif

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&treemodel_11);
#endif // ENABLE_STATIC_MODELS


#ifdef ENABLE_DYNAMIC_MODELS
	unloadDynamicModel(&skeletonModel_11);
#endif
	//uninitializeCamera(&camera);

}
