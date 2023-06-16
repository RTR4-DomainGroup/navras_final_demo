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

#define BB_Y_MIN (-0.3f)
#define BB_Y_MAX (0.0f)

#define BB_Z_MIN (-50.0f)
#define BB_Z_MAX (50.0f)

#define BB_NO_OF_INSTANCES 2000

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
//
#ifdef ENABLE_STATIC_MODELS
#include "../../inc/effects/StaticModelLoadingEffect.h"
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_STARFIELD
#include "../../inc/effects/StarfieldEffect.h"
#endif // ENABLE_STARFIELD

#include "../../inc/scenes/scene10_AdbhutRas.h"


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

#ifdef ENABLE_TERRIAN
static struct TerrainUniform terrainUniform;
#endif // ENABLE_TERRIAN

#ifdef ENABLE_BILLBOARDING
// variables for billboarding
static TEXTURE texture_flower;
static struct BillboardingUniform billboardingEffectUniform;
static GLuint frameTime; // = 0;
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_WATER
// Water Related Variables
static struct WaterUniform waterUniform;
static struct TextureVariables waterTextureVariables;
static struct WaterFrameBufferDetails waterReflectionFrameBufferDetails;
static struct WaterFrameBufferDetails waterRefractionFrameBufferDetails;
#endif // ENABLE_WATER

#ifdef ENABLE_ATMOSPHERE
// Atmosphere Scattering
static AtmosphereUniform atmosphereUniform_10;
AtmosphericVariables atmosVariables_10;
#endif // ENABLE_ATMOSPHERE

#ifdef ENABLE_SHADOW
// Shadow
extern ShadowFrameBufferDetails shadowFramebuffer;
static mat4 lightSpaceMatrix;
#endif // ENABLE_SHADOW

extern GLfloat waterHeight; // = 0.0f;
extern GLfloat moveFactor; // = 0.0f;
extern GLfloat planeReflection[]; // = { 0.0f, 1.0f, 0.0f, -waterHeight };
extern GLfloat planeRefration[]; // = { 0.0f, -1.0f, 0.0f, waterHeight }; 

extern mat4 perspectiveProjectionMatrix;

#ifdef ENABLE_STARFIELD
// Variables For Starfieldx
extern GLuint texture_star; 
extern double deltaTime;
static struct StarfieldUniform sceneStarfieldUniform;
#endif // ENABLE_STARFIELD

extern GLfloat density; // = 0.15;
extern GLfloat gradient; // = 0.5;
extern GLfloat skyFogColor[]; // = { 0.25f, 0.25f, 0.25f, 1.0f };


#ifdef ENABLE_STATIC_MODELS
static struct ADSUniform sceneOutdoorADSStaticUniform_10;

//Model variables
static STATIC_MODEL rockModel;
static STATIC_MODEL treeModel;
static STATIC_MODEL leafModel;
static STATIC_MODEL farmhouseModel;
// static STATIC_MODEL adbhutmanModel;
static STATIC_MODEL bridgeModel;
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
static DYNAMIC_MODEL skeletonModel;
static struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;
#endif // ENABLE_DYNAMIC_MODELS

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

static quad_instancing_buffers_t instBuffers_leftflowers;
static quad_instancing_buffers_t instBuffers_rightflowers;

static float distance10;

static bool isInitialDisplay_Scene10AdbhutRas = true;

static GLuint texture_adbhutMask;

static mat4 finalViewMatrix = mat4::identity();

// static float leaf_translate = 1.5f;
static float leaf_translate = 0.35f;
// static float leaf_rotate = 31.45f;
static float leaf_rotate = 0.0f;
static int scene10_state = 1; 

struct Point {
    float x, y ;
};
 
struct line {
    Point p1, p2;
};

bool checkInside(Point poly[], int n, Point p);

int initializeScene10_AdbhutRas(void)
{
	// Function Declarations
	// -10.000000, -2.250000, -8.000000], [-10.193824, -3.229223, -8.059628] [0.000000, 1.000000, 0.000000
    
	// Code.
	// initializeCamera(&camera);

#ifdef ENABLE_ATMOSPHERE


	atmosVariables_10.m_nSamples = 3;
	atmosVariables_10.m_Kr = 0.026200;
	atmosVariables_10.m_Kr4PI = 0.329238;
	atmosVariables_10.m_Km = 0.017300;
	atmosVariables_10.m_Km4PI = 0.217398;
	atmosVariables_10.m_ESun = 3.899959;
	atmosVariables_10.m_g = -0.984000;
	atmosVariables_10.m_fExposure = 2.000000;
	atmosVariables_10.m_fInnerRadius = 0.000000;
	atmosVariables_10.m_fOuterRadius = 102.000000;
	atmosVariables_10.m_fScale = 0.009804;
	atmosVariables_10.m_fWavelength[0] = 0.697999;
	atmosVariables_10.m_fWavelength[1] = 0.570000;
	atmosVariables_10.m_fWavelength[2] = 0.475000;
	atmosVariables_10.m_fWavelength4[0] = 0.237367;
	atmosVariables_10.m_fWavelength4[1] = 0.105560;
	atmosVariables_10.m_fWavelength4[2] = 0.050907;
	atmosVariables_10.m_fRayleighScaleDepth = 0.250000;
	atmosVariables_10.m_fMieScaleDepth = 0.100000;

	atmosVariables_10.m_vLight = vec3(-5, 90, -350);
	atmosVariables_10.m_vLightDirection = atmosVariables_10.m_vLight / sqrtf(atmosVariables_10.m_vLight[0] * atmosVariables_10.m_vLight[0] + atmosVariables_10.m_vLight[1] * atmosVariables_10.m_vLight[1] + atmosVariables_10.m_vLight[2] * atmosVariables_10.m_vLight[2]);

	//
	initializeAtmosphere(atmosVariables_10);

#endif // ENABLE_ATMOSPHERE

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

#ifdef ENABLE_STARFIELD
	initializeQuad();
	initializeCube();
	initializeCubeWithTilingTexcoords();

#endif  // ENABLE_STARFIELD

#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/rock/rock.obj", &rockModel);
	loadStaticModel("res/models/tree_adbhut/tree.fbx", &treeModel);
	loadStaticModel("res/models/tree_adbhut/leaf.obj", &leafModel);
	loadStaticModel("res/models/farmhouse/farmhouse.obj", &farmhouseModel);
	// loadStaticModel("res/models/scene10_adbhut/tempAdbhutMan.obj", &adbhutmanModel);
	loadStaticModel("res/models/bridge/bridge.obj", &bridgeModel);
#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	//loadDynamicModel("res/models/skeleton/sadWalk.fbx", &skeletonModel);
	//loadDynamicModel("res/models/exo/Walking.dae", &skeletonModel);
	// loadDynamicModel("res/models/man/man.fbx", &skeletonModel);
	loadDynamicModel("res/models/scene10_adbhut/adbhutManAnim.fbx", &skeletonModel);
#endif // ENABLE_DYNAMIC_MODELS

#ifdef ENABLE_BILLBOARDING
	void sort_instances_z_order(GLfloat instance_positions[], int numInstances) ;

	void generate_instance_positions(float instance_positions[], int numInstances, Point bondryPolygone[], int numPoints) ;

	char imagefile[64] = {};
	sprintf(imagefile, "%s", TEXTURE_DIR"/billboarding/flower5.png");
	if (LoadGLTextureData_UsingSOIL(&texture_flower, imagefile) == GL_FALSE)
	{
		LOG("Texture loading failed for image %s\n", imagefile);
		return (-6);
	}

	
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

	// left flowers
	Point pentaPos_lf[5];
	int i = 0;
	for (int i = 0; i < 5; i++)
	{
		pentaPos_lf[i].x = -25.0f + (-15.0f * sin((3 * M_PI / 2) + (i * 72.0 * M_PI / 180.0)));
		pentaPos_lf[i].y = 0.0f + (100.0f * cos((3 * M_PI / 2) + (i * 72.0 * M_PI / 180.0)));

		// pentaPos[i].x = -25.0f + (15.0f * cos( float(angle) *  M_PI / 180.0))  ;
		// pentaPos[i].y = 0.0f + (30.0f * sin( float(angle) *  M_PI / 180.0))  ;
		LOG("point %d:  (%ff, %ff)\n", i, pentaPos_lf[i].x, pentaPos_lf[i].y);
	}

	// generate_instance_positions(instance_positions, BB_NO_OF_INSTANCES);
	generate_instance_positions(instance_positions, BB_NO_OF_INSTANCES, pentaPos_lf, sizeof(pentaPos_lf)/sizeof(pentaPos_lf[0]));

	// Bhuichakkar
	// for(int i = 0; i < BB_NO_OF_INSTANCES; i++)
	// {
	// 	instance_positions[(i*4)+0] =  (i%50 * sinf(i * M_PI/180));
	// 	instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (BB_Y_MAX - BB_Y_MIN)) + BB_Y_MIN;
	// 	instance_positions[(i*4)+2] =  (i%50 * cosf(i * M_PI/180));
	// 	instance_positions[(i*4)+3] = 1.0f;
	// 	// LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
	// }

	// for(int i = 0; i < BB_NO_OF_INSTANCES; i++)
	// {
	// 	instance_positions[(i*4)+0] = ((((GLfloat)rand() / RAND_MAX) * (BB_X_MAX - BB_X_MIN)) + BB_X_MIN ) + (i%10 * sinf(i * M_PI/180));
	// 	instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (BB_Y_MAX - BB_Y_MIN)) + BB_Y_MIN;
	// 	instance_positions[(i*4)+2] = ((((GLfloat)rand() / RAND_MAX) * (BB_Z_MAX - BB_Z_MIN)) + BB_Z_MIN)  + (i%10 * cosf(i * M_PI/180));
	// 	instance_positions[(i*4)+3] = 1.0f;
	// 	// LOG("Instance %d Position: [%f %f %f]\n", i, instance_positions[(i*4)+0], instance_positions[(i*4)+1], instance_positions[(i*4)+2]);
	// }

	// sort z vertices
	sort_instances_z_order(instance_positions, BB_NO_OF_INSTANCES);

	initializeInstancedQuad(instBuffers_leftflowers, BB_NO_OF_INSTANCES, instance_positions);


	// /// Right flowers
	// Point pentaPos_rf[] = {
	// 	{13.46f, -35.49f},
	// 	{1.39f, -36.00f },
	// 	{5.30f, -22.40f },
	// 	{8.87f, -9.99f },
	// 	{9.21f, 0.04f  },
	// 	{13.12f, -1.15f },
	// 	{19.24f, -6.76f },
	// 	{21.11f, -17.47f},
	// 	{22.47f, -21.21f},
	// 	{22.47f, -33.79f},
	// 	{18.73f, -37.87f},
	// 	{13.46f, -35.49f},
	// };

	// generate_instance_positions(instance_positions, BB_NO_OF_INSTANCES, pentaPos_rf, sizeof(pentaPos_rf)/sizeof(pentaPos_rf[0]));

	// // sort z vertices
	// sort_instances_z_order(instance_positions, BB_NO_OF_INSTANCES);

	// initializeInstancedQuad(instBuffers_rightflowers, BB_NO_OF_INSTANCES, instance_positions);

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

	return 0;
}


void generate_instance_positions(float instance_positions[], int numInstances, Point bondryPolygone[], int numPoints) {
	for(int i = 0; i < numInstances; i++)
	{
		// instance_positions[(i*4)+0] = pentaPos[i].x;
		// instance_positions[(i*4)+1] = 0.0f; // (((GLfloat)rand() / RAND_MAX) * (BB_Y_MAX - BB_Y_MIN)) + BB_Y_MIN;
		// instance_positions[(i*4)+2] = pentaPos[i].y;
		// instance_positions[(i*4)+3] = 1.0f;

		instance_positions[(i*4)+0] = (((GLfloat)rand() / RAND_MAX) * (BB_X_MAX - BB_X_MIN)) + BB_X_MIN;
		instance_positions[(i*4)+1] = (((GLfloat)rand() / RAND_MAX) * (BB_Y_MAX - BB_Y_MIN)) + BB_Y_MIN;
		instance_positions[(i*4)+2] = (((GLfloat)rand() / RAND_MAX) * (BB_Z_MAX - BB_Z_MIN)) + BB_Z_MIN;
		instance_positions[(i*4)+3] = 1.0f;

		Point p = {
			instance_positions[(i*4)+0],
			instance_positions[(i*4)+2]
		};

		if (!checkInside(bondryPolygone, numPoints, p))
		{
			i--;
		}

	}
}

void sort_instances_z_order(GLfloat instance_positions[], int numInstances) {
	for (int i = 0; i < numInstances; i++)
	{
		for (int j = i + 1; j < numInstances; ++j)
		{
			if (instance_positions[(i * 4) + 2] > instance_positions[(j * 4) + 2])
			{
				auto a = instance_positions[(i * 4) + 2];
				instance_positions[(i * 4) + 2] = instance_positions[(j * 4) + 2];
				instance_positions[(j * 4) + 2] = a;

				a = instance_positions[(i * 4) + 0];
				instance_positions[(i * 4) + 0] = instance_positions[(j * 4) + 0];
				instance_positions[(j * 4) + 0] = a;
			}
		}
	}
}

void setCameraScene10(void)
{
	if (isInitialDisplay_Scene10AdbhutRas == true)
	{
		// setCamera(17.20f, -0.70f, 6.05f, -134.67f, -0.70f, -333.86f, 0.00f, 1.00f, 0.00f);
		// setCamera(-15.78f, -1.20f, -34.73f, -362.21f, 49.98f, -14.27f, 0.00f, 1.00f, 0.00f);
		setCamera(21.90f, -1.11f, -1.13f, -150.37f, -1.11f, -327.12f, 0.00f, 1.00f, 0.00f);
		isInitialDisplay_Scene10AdbhutRas = false;
		scene10_state = 1;
		LOG("Switching to state %d\n", scene10_state);
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

	if (recordWaterReflectionRefraction == true) {

#ifdef ENABLE_WATER

	waterUniform = useWaterShader();

		if (isReflection == true)
		{
			distance10 = 2.0f * (cameraEyeY - waterHeight);
			glUniform4fv(waterUniform.planeUniform, 1, planeReflection);
			cameraEyeY -= distance10;
			cameraCenterY -= distance10;
			displayCamera();
			finalViewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
			glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		}
		else
		{
			glUniform4fv(waterUniform.planeUniform, 1, planeRefration);
			glUniformMatrix4fv(waterUniform.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
		}

#endif // ENABLE_WATER
	}

	if (actualDepthQuadScene == 0) {

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
		rotationMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);					// glTranslatef() is replaced by this line.
		//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
		if('k' == tf_Object)
			update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix);
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

		glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
		glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, finalViewMatrix);
		glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

		//glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);
		glUniform1i(sceneStarfieldUniform.uniform_enable_godRays, godRays);
		glUniform1f(sceneStarfieldUniform.timeUniform, time);

		displayStarfield(texture_star);
		glUseProgram(0);

#endif // ENABLE_STARFIELD

		if (godRays == 1) {

#ifdef ENABLE_ATMOSPHERE

			translationMatrix = mat4::identity();
			rotationMatrix = mat4::identity();
			modelMatrix = mat4::identity();

			//glBlendFunc(GL_ONE, GL_ONE);

			atmosphereUniform_10 = useAtmosphereShader();
			
			modelMatrix = vmath::scale(5.0f, 5.0f, 5.0f) * vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);

			glUniform3f(atmosphereUniform_10.cameraPosUniform, cameraEyeX, cameraEyeY, cameraEyeZ);
			glUniform3f(atmosphereUniform_10.lightPosUniform, atmosVariables_10.m_vLightDirection[0], atmosVariables_10.m_vLightDirection[1], atmosVariables_10.m_vLightDirection[2]);
			glUniform3f(atmosphereUniform_10.invWavelengthUniform, 1 / atmosVariables_10.m_fWavelength4[0], 1 / atmosVariables_10.m_fWavelength4[1], 1 / atmosVariables_10.m_fWavelength4[2]);
			glUniform1f(atmosphereUniform_10.cameraHeightUniform, sqrtf(cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ));
			glUniform1f(atmosphereUniform_10.cameraHeight2Uniform, cameraEyeX * cameraEyeX + cameraEyeY * cameraEyeY + cameraEyeZ * cameraEyeZ);
			glUniform1f(atmosphereUniform_10.innerRadiusUniform, atmosVariables_10.m_fInnerRadius);
			glUniform1f(atmosphereUniform_10.innerRadius2Uniform, atmosVariables_10.m_fInnerRadius * atmosVariables_10.m_fInnerRadius);
			glUniform1f(atmosphereUniform_10.outerRadiusUniform, atmosVariables_10.m_fOuterRadius);
			glUniform1f(atmosphereUniform_10.outerRadius2Uniform, atmosVariables_10.m_fOuterRadius * atmosVariables_10.m_fOuterRadius);
			glUniform1f(atmosphereUniform_10.KrESunUniform, atmosVariables_10.m_Kr * atmosVariables_10.m_ESun);
			glUniform1f(atmosphereUniform_10.KmESunUniform, atmosVariables_10.m_Km * atmosVariables_10.m_ESun);
			glUniform1f(atmosphereUniform_10.Kr4PIUniform, atmosVariables_10.m_Kr4PI);
			glUniform1f(atmosphereUniform_10.Km4PIUniform, atmosVariables_10.m_Km4PI);
			glUniform1f(atmosphereUniform_10.scaleUniform, 1.0f / (atmosVariables_10.m_fOuterRadius - atmosVariables_10.m_fInnerRadius));
			glUniform1f(atmosphereUniform_10.scaleDepthUniform, atmosVariables_10.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniform_10.scaleOverScaleDepthUniform, (1.0f / (atmosVariables_10.m_fOuterRadius - atmosVariables_10.m_fInnerRadius)) / atmosVariables_10.m_fRayleighScaleDepth);
			glUniform1f(atmosphereUniform_10.gUniform, atmosVariables_10.m_g);
			glUniform1f(atmosphereUniform_10.g2Uniform, atmosVariables_10.m_g * atmosVariables_10.m_g);

			glUniformMatrix4fv(atmosphereUniform_10.modelMatrix, 1, GL_FALSE, modelMatrix);
			glUniformMatrix4fv(atmosphereUniform_10.viewMatrix, 1, GL_FALSE, finalViewMatrix);
			glUniformMatrix4fv(atmosphereUniform_10.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

			displayAtmosphere();

			glUseProgram(0);

#endif // ENABLE_ATMOSPHERE

		} // if(godRays == 1)

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
	sceneOutdoorADSStaticUniform_10 = useADSShader();
	glUniform1f(sceneOutdoorADSStaticUniform_10.isInstanced, 0);

	// Sending Light Related Uniforms
	glUniform1i(sceneOutdoorADSStaticUniform_10.lightingEnableUniform, 1);
	glUniform4fv(sceneOutdoorADSStaticUniform_10.laUniform, 1, lightAmbient);
	glUniform4fv(sceneOutdoorADSStaticUniform_10.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneOutdoorADSStaticUniform_10.lsUniform, 1, lightSpecular);
	// glUniform4fv(sceneOutdoorADSStaticUniform_10.lightPositionUniform, 1, lightPosition);
	TRANFORM lightPos = { 104.0f, 103.0f, 3.0f, 1.0f };
	// update_transformations(NULL, NULL, NULL, &lightPos);
	vmath::vec4 lp = {lightPos.x, lightPos.y, lightPos.z, lightPos.w}; 
	glUniform4fv(sceneOutdoorADSStaticUniform_10.lightPositionUniform, 1, lp);
	glUniform4fv(sceneOutdoorADSStaticUniform_10.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneOutdoorADSStaticUniform_10.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneOutdoorADSStaticUniform_10.ksUniform, 1, materialSpecular);
	glUniform1f(sceneOutdoorADSStaticUniform_10.materialShininessUniform, materialShininess);
	glUniform1f(sceneOutdoorADSStaticUniform_10.colorCorrectionUniform, 0.7f);

	//normal mapping
	glUniform4fv(sceneOutdoorADSStaticUniform_10.viewpositionUniform, 1, camera.eye);

	glUniform1i(sceneOutdoorADSStaticUniform_10.fogEnableUniform, 0);
	glUniform1f(sceneOutdoorADSStaticUniform_10.densityUniform, density);
	glUniform1f(sceneOutdoorADSStaticUniform_10.gradientUniform, gradient);
	glUniform4fv(sceneOutdoorADSStaticUniform_10.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneOutdoorADSStaticUniform_10.uniform_enable_godRays, godRays);
	glUniform1i(sceneOutdoorADSStaticUniform_10.godrays_blackpass_sphere, 0);

    // once for all static models drawing
 	glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.viewMatrixUniform, 1, GL_FALSE, finalViewMatrix);
	glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// ------ Rock Model ------
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};

	translationMatrix = vmath::translate(-6.80f, -3.5f, -15.88f);
	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	
	// usage type 2
	if('2' == tf_Object) // Rock model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles);

	rotationMatrix_x = vmath::rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	rotationMatrix_z = vmath::rotate(rotationAngles.z, 1.0f, 0.0f, 1.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	if (actualDepthQuadScene == 1) {

		glUniform1i(sceneOutdoorADSStaticUniform_10.actualSceneUniform, 0);
		glUniform1i(sceneOutdoorADSStaticUniform_10.depthSceneUniform, 1);
		glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.lightSpaceMatrixUniform, 1, GL_FALSE, lightSpaceMatrix);

	} else {

		glUniform1i(sceneOutdoorADSStaticUniform_10.actualSceneUniform, 1);
		glUniform1i(sceneOutdoorADSStaticUniform_10.depthSceneUniform, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowFramebuffer.frameBufferDepthTexture);

	}

	drawStaticModel(rockModel);

	// ------ Tree Model ------
	modelMatrix = mat4::identity();
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();

	translationMatrix = vmath::translate(-20.75f, -2.27f, -34.19f);
	scaleMatrix = vmath::scale(0.34f,  0.34f,  0.34f);

	// usage type 1 
	if('3' == tf_Object) // Tree model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix) ;
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	drawStaticModel(treeModel);

	// ------ Leaf Model ------
	modelMatrix = mat4::identity();
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};

	// start
	translationMatrix = vmath::translate(-19.57f, leaf_translate, -34.25f);
	scaleMatrix = vmath::scale(0.14f,  0.14f,  0.14f);
	rotationAngles = {674.35f, leaf_rotate, -976.88f};
	// // end
	// translationMatrix = vmath::translate(-19.55f, -1.83f, -34.20f);
	// rotationAngles = {674.35f, 247.20f, -976.88f};

	// usage type 1 
	if('1' == tf_Object) // Leaf model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
	rotationMatrix = vmath::rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);
	rotationMatrix *= vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	rotationMatrix *= vmath::rotate(rotationAngles.z, 0.0f, 0.0f, 1.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	drawStaticModel(leafModel);


	// ------ farmhouse Model ------
	modelMatrix = mat4::identity();
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};

	translationMatrix = vmath::translate(9.80f, -2.15f, -37.75f);
	scaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
	rotationAngles = {0.0f, -59.25f, 0.0f};

	// usage type 1 
	if('4' == tf_Object) // farmhouse model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
	rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	drawStaticModel(farmhouseModel);


	// // ------ adbhutmanModel Model ------
	// translationMatrix = mat4::identity();
	// rotationMatrix = mat4::identity();
	// modelMatrix = mat4::identity();
	// scaleMatrix = mat4::identity();
	// rotationAngles = {0.0f, 0.0f, 0.0f};

	// translationMatrix = vmath::translate(-19.94f, -2.18f, -34.25f);
	// scaleMatrix = vmath::scale(0.05f, 0.05f, 0.05f);
	// rotationAngles = {0.0f, 99.55f, 0.0f};

	// // usage type 1 
	// if('5' == tf_Object) // adbhutman model
	// 	update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
	// rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	// modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	// glUniformMatrix4fv(sceneOutdoorADSStaticUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	// drawStaticModel(adbhutmanModel);


	// ------ Bridge Model ------
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	rotationAngles = {0.0f, 0.0f, 0.0f};
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(-6.86f, -1.40f, -36.00f);
	scaleMatrix = vmath::scale(1.81f, 1.81f, 1.81f);
	rotationAngles = {2.32f, 189.75f, -5.41f};

	// usage type 1 
	if('9' == tf_Object) // bridge model
		update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
	rotationMatrix = vmath::rotate(rotationAngles.z, 0.0f, 0.0f, 1.0f);
	rotationMatrix *= vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);
	rotationMatrix *= vmath::rotate(rotationAngles.x, 1.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneOutdoorADSStaticUniform_10.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	drawStaticModel(bridgeModel);

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

	glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-19.94f, -2.13f, -34.31f));
	glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.008f, 0.008f, 0.008f));
	glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), 1.80f, glm::vec3(0.0f, 1.0f, 0.0f));
	glm_modelMatrix = glm_translateMatrix * glm_scaleMatrix * glm_rotateMatrix;

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

	if (scene10_state == 1) // replay animation
	{
		LOG("Replaying model animation\n");
		reDrawDynamicModel(sceneOutdoorADSDynamicUniform, skeletonModel, 1.0f);
		scene10_state = 2;
	}
	else
		drawDynamicModel(sceneOutdoorADSDynamicUniform, skeletonModel, 1.0f);
	glUniform1f(sceneOutdoorADSDynamicUniform.colorCorrectionUniform, 0.0f);


	glUseProgram(0);

#endif

#ifdef ENABLE_WATER

	if(waterDraw == true){
		
		waterUniform = useWaterShader();

		translationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();

		translationMatrix = vmath::translate(-2.00f, -3.00f, -3.00f);
		scaleMatrix = vmath::scale(80.0f, 1.0f, 80.0f);

		// update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix) ;
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

		/// /////////////////////////////
		/// Flower on left side of river
		/// /////////////////////////////
		/// 

		// instanced quads with grass texture
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		rotationAngles = {0.0f, 0.0f, 0.0f};

		/// Left Flower
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationAngles = {0.0f, 0.0f, 0.0f};
		
		if (texture_flower.height > texture_flower.width)
			scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
		else
			scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

		translationMatrix = vmath::translate(3.00f, -1.90f, -16.75f);
		scaleMatrix *= vmath::scale(0.80f, 0.68f, 0.65f);
		rotationAngles = {0.0f, 7.25f, 0.0f};

		if('7' == tf_Object) // White Flower
			update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
		rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);	
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		// send to shader
		glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_flower.id);
		displayInstancedQuads(instBuffers_leftflowers, BB_NO_OF_INSTANCES);  // how many instances to draw
		glBindTexture(GL_TEXTURE_2D, 0);


		/// /////////////////////////
		/// Flower on right side of river
		/// /////////////////////////


		/// right Flower
		translationMatrix = mat4::identity();
		rotationMatrix = mat4::identity();
		modelMatrix = mat4::identity();
		scaleMatrix = mat4::identity();
		rotationAngles = {0.0f, 0.0f, 0.0f};
		
		if (texture_flower.height > texture_flower.width)
			scaleMatrix = vmath::scale(texture_flower.width / (GLfloat)texture_flower.height, 1.0f, 1.0f);
		else
			scaleMatrix = vmath::scale(1.0f, texture_flower.height / (GLfloat)texture_flower.width, 1.0f);

		translationMatrix = vmath::translate(29.63f, -2.05f, -22.43f);
		scaleMatrix *= vmath::scale(0.65f, 0.40f, 0.65f);
		rotationAngles = {0.0f, 15.00f, 0.0f};

		if('8' == tf_Object) // White Flower
			update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
		rotationMatrix = vmath::rotate(rotationAngles.y, 0.0f, 1.0f, 0.0f);	
		modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

		// send to shader
		glUniformMatrix4fv(billboardingEffectUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_flower.id);
		displayInstancedQuads(instBuffers_leftflowers, BB_NO_OF_INSTANCES);  // how many instances to draw
		glBindTexture(GL_TEXTURE_2D, 0);

		glUseProgram(0);
		glDisable(GL_BLEND);
	}

#endif // ENABLE_BILLBOARDING

	if (isReflection == true) {

		cameraEyeY += distance10;
		cameraCenterY += distance10;
		displayCamera();
		finalViewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	}

}

extern float cam_mov[][9];
int i = 0;
int interval = 5; 
void updateScene10_AdbhutRas(void)
{
	// Code

#ifdef ENABLE_CAMERA_ANIMATION
	//TRANFORM speedVector = {0.0f, 0.0f, 0.0f};
	//speedVector.x = 0.04;
	//// update_transformations(NULL, NULL, NULL, &speedVector);
	//cameraEyeZ -= speedVector.x;
	//cameraCenterZ -= speedVector.x;

//	if (i <= 524)
//	{
//		preciselerp_lookat(cam_mov[i][0], cam_mov[i][1], cam_mov[i][2], cam_mov[i][3], cam_mov[i][4],
//						   cam_mov[i][5], cam_mov[i][6], cam_mov[i][7], cam_mov[i][8]);
//		if(interval > 6)
//		{
//			i++;
//			interval = 0;
//		}
//		interval++;
//	}

	// if (scene10_state == 0)
	// {
	// 	preciselerp_lookat((21.90f, -1.11f, -1.13f, -150.37f, -1.11f, -327.12f, 0.00f, 1.00f, 0.00f););
	// 	if (cameraEyeX < (1.50f - 0.2f))
	// 	{
	// 		scene10_state = 2;
	// 		LOG("Switching to state %d\n", scene10_state);
	// 	}
	// }

	if (scene10_state == 2) // straight to front to bridge
	{
		preciselerp_lookat(-0.71f, -0.76f, -45.00f, -362.13f, -0.76f, 21.77f, 0.00f, 1.00f, 0.00f, 0.002f);
		if (cameraEyeZ < (-35.99f - 0.2f))
		{
			scene10_state = 3;
			LOG("Switching to state %d\n", scene10_state);
		}
	}

	if (scene10_state == 3) // top of the bridge
	{
		preciselerp_lookat(-20.21f, 1.75f, -37.50f, -365.63f, -0.76f, 21.77f, 0.00f, 1.00f, 0.00f, 0.002f);
		if (cameraEyeX < (-4.21f - 0.2f))
		{
			scene10_state = 4;
			LOG("Switching to state %d\n", scene10_state);
		}
	}

	if (scene10_state == 4) // down to bridge
	{
		preciselerp_lookat(-30.35f, -2.11f, -33.00f, -377.14f, -1.11f, 25.08f, 0.00f, 1.00f, 0.00f, 0.002f);
		if (cameraEyeX < (-16.28f - 0.2f))
		{
			scene10_state = 5;
		}
	}

#endif

	if(scene10_state == 5 && skeletonModel.pAnimator->GetCurrentFrame() >= 1315.0) 
	{
		leaf_translate -= 0.015f;
		leaf_rotate += 9.52f;	
		if(leaf_translate < -1.83f) {
			leaf_translate = -1.83f;
			leaf_rotate = 247.20f;	
			scene10_state = 6;
		}
		if(leaf_rotate > 360.0f) {
			leaf_rotate = 0.0f;
		}
	}

	if('i' == tf_Object) // farmhouse model
	{
		isInitialDisplay_Scene10AdbhutRas = true;
		tf_Object = 0;
		leaf_translate = 0.35f;
		i = 0;
	}

	if ('n' == tf_Object) // replay animation
	{
		LOG("Replaying model animation\n");
		reDrawDynamicModel(sceneOutdoorADSDynamicUniform, skeletonModel, 1.0f);
		tf_Object = 0;
	}

#ifdef ENABLE_BILLBOARDING
	frameTime += 4;

#endif // ENABLE_BILLBOARDING


}

void uninitializeScene10_AdbhutRas(void)
{
	// Code
#ifdef ENABLE_BILLBOARDING
    uninitializeInstancedQuads(instBuffers_rightflowers);
    uninitializeInstancedQuads(instBuffers_leftflowers);

	// texture
    if(texture_flower.id)
    {
        glDeleteTextures(1, &texture_flower.id);
        texture_flower.id = 0;
    }

#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariables);
#endif // ENABLE_TERRIAN

#ifdef ENABLE_STATIC_MODELS
	//UNINIT models
	unloadStaticModel(&bridgeModel);
	// unloadStaticModel(&adbhutmanModel);
	unloadStaticModel(&farmhouseModel);
	unloadStaticModel(&rockModel);
	unloadStaticModel(&leafModel);
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

}
 
bool onLine(line l1, Point p)
{
    // Check whether p is on the line or not
    if (p.x <= vmath::max(l1.p1.x, l1.p2.x) && p.x <= vmath::min(l1.p1.x, l1.p2.x)
        && (p.y <= vmath::max(l1.p1.y, l1.p2.y) && p.y <= vmath::min(l1.p1.y, l1.p2.y)))
        return true;
 
    return false;
}
 
int direction(Point a, Point b, Point c)
{
    int val = (b.y - a.y) * (c.x - b.x)
              - (b.x - a.x) * (c.y - b.y);
 
    if (val == 0)
 
        // Collinear
        return 0;
 
    else if (val < 0)
 
        // Anti-clockwise direction
        return 2;
 
    // Clockwise direction
    return 1;
}
 
bool isIntersect(line l1, line l2)
{
    // Four direction for two lines and points of other line
    int dir1 = direction(l1.p1, l1.p2, l2.p1);
    int dir2 = direction(l1.p1, l1.p2, l2.p2);
    int dir3 = direction(l2.p1, l2.p2, l1.p1);
    int dir4 = direction(l2.p1, l2.p2, l1.p2);
 
    // When intersecting
    if (dir1 != dir2 && dir3 != dir4)
        return true;
 
    // When p2 of line2 are on the line1
    if (dir1 == 0 && onLine(l1, l2.p1))
        return true;
 
    // When p1 of line2 are on the line1
    if (dir2 == 0 && onLine(l1, l2.p2))
        return true;
 
    // When p2 of line1 are on the line2
    if (dir3 == 0 && onLine(l2, l1.p1))
        return true;
 
    // When p1 of line1 are on the line2
    if (dir4 == 0 && onLine(l2, l1.p2))
        return true;
 
    return false;
}
 
bool checkInside(Point poly[], int n, Point p)
{
 
    // When polygon has less than 3 edge, it is not polygon
    if (n < 3)
        return false;
 
    // Create a point at infinity, y is same as point p
    line exline = { p, { 9999, p.y } };
    int count = 0;
    int i = 0;
    do {
 
        // Forming a line from two consecutive points of
        // poly
        line side = { poly[i], poly[(i + 1) % n] };
        if (isIntersect(side, exline)) {
 
            // If side is intersects exline
            if (direction(side.p1, p, side.p2) == 0)
                return onLine(side, p);
            count++;
        }
        i = (i + 1) % n;
    } while (i != 0);
 
    // When count is odd
    return count & 1;
}

float cam_mov[][9] = {{21.90f, -1.11f, -1.18f, -150.37f, -1.11f, -327.17f, 0.00f, 1.00f, 0.00f},
{21.85f, -1.11f, -1.18f, -150.42f, -1.11f, -327.17f, 0.00f, 1.00f, 0.00f},
{21.85f, -1.11f, -1.23f, -150.42f, -1.11f, -327.22f, 0.00f, 1.00f, 0.00f},
{21.80f, -1.11f, -1.23f, -150.47f, -1.11f, -327.22f, 0.00f, 1.00f, 0.00f},
{21.80f, -1.11f, -1.28f, -150.47f, -1.11f, -327.27f, 0.00f, 1.00f, 0.00f},
{21.75f, -1.11f, -1.28f, -150.52f, -1.11f, -327.27f, 0.00f, 1.00f, 0.00f},
{21.75f, -1.11f, -1.33f, -150.52f, -1.11f, -327.32f, 0.00f, 1.00f, 0.00f},
{21.70f, -1.11f, -1.33f, -150.57f, -1.11f, -327.32f, 0.00f, 1.00f, 0.00f},
{21.70f, -1.11f, -1.38f, -150.57f, -1.11f, -327.37f, 0.00f, 1.00f, 0.00f},
{21.65f, -1.11f, -1.38f, -150.62f, -1.11f, -327.37f, 0.00f, 1.00f, 0.00f},
{21.65f, -1.11f, -1.43f, -150.62f, -1.11f, -327.42f, 0.00f, 1.00f, 0.00f},
{21.60f, -1.11f, -1.43f, -150.67f, -1.11f, -327.42f, 0.00f, 1.00f, 0.00f},
{21.60f, -1.11f, -1.48f, -150.67f, -1.11f, -327.47f, 0.00f, 1.00f, 0.00f},
{21.55f, -1.11f, -1.48f, -150.72f, -1.11f, -327.47f, 0.00f, 1.00f, 0.00f},
{21.55f, -1.11f, -1.53f, -150.72f, -1.11f, -327.52f, 0.00f, 1.00f, 0.00f},
{21.50f, -1.11f, -1.53f, -150.77f, -1.11f, -327.52f, 0.00f, 1.00f, 0.00f},
{21.50f, -1.11f, -1.58f, -150.77f, -1.11f, -327.57f, 0.00f, 1.00f, 0.00f},
{21.45f, -1.11f, -1.58f, -150.82f, -1.11f, -327.57f, 0.00f, 1.00f, 0.00f},
{21.45f, -1.11f, -1.63f, -150.82f, -1.11f, -327.62f, 0.00f, 1.00f, 0.00f},
{21.40f, -1.11f, -1.63f, -150.87f, -1.11f, -327.62f, 0.00f, 1.00f, 0.00f},
{21.40f, -1.11f, -1.84f, -150.87f, -1.11f, -327.83f, 0.00f, 1.00f, 0.00f},
{21.19f, -1.11f, -1.84f, -151.08f, -1.11f, -327.83f, 0.00f, 1.00f, 0.00f},
{21.19f, -1.11f, -2.05f, -151.08f, -1.11f, -328.04f, 0.00f, 1.00f, 0.00f},
{20.98f, -1.11f, -2.05f, -151.29f, -1.11f, -328.04f, 0.00f, 1.00f, 0.00f},
{20.98f, -1.11f, -2.26f, -151.29f, -1.11f, -328.25f, 0.00f, 1.00f, 0.00f},
{20.77f, -1.11f, -2.26f, -151.50f, -1.11f, -328.25f, 0.00f, 1.00f, 0.00f},
{20.77f, -1.11f, -2.47f, -151.50f, -1.11f, -328.46f, 0.00f, 1.00f, 0.00f},
{20.56f, -1.11f, -2.47f, -151.71f, -1.11f, -328.46f, 0.00f, 1.00f, 0.00f},
{20.56f, -1.11f, -2.68f, -151.71f, -1.11f, -328.67f, 0.00f, 1.00f, 0.00f},
{20.35f, -1.11f, -2.68f, -151.92f, -1.11f, -328.67f, 0.00f, 1.00f, 0.00f},
{20.35f, -1.11f, -2.89f, -151.92f, -1.11f, -328.88f, 0.00f, 1.00f, 0.00f},
{20.14f, -1.11f, -2.89f, -152.13f, -1.11f, -328.88f, 0.00f, 1.00f, 0.00f},
{20.14f, -1.11f, -3.10f, -152.13f, -1.11f, -329.09f, 0.00f, 1.00f, 0.00f},
{19.93f, -1.11f, -3.10f, -152.34f, -1.11f, -329.09f, 0.00f, 1.00f, 0.00f},
{19.93f, -1.11f, -3.31f, -152.34f, -1.11f, -329.30f, 0.00f, 1.00f, 0.00f},
{19.72f, -1.11f, -3.31f, -152.55f, -1.11f, -329.30f, 0.00f, 1.00f, 0.00f},
{19.72f, -1.11f, -3.52f, -152.55f, -1.11f, -329.51f, 0.00f, 1.00f, 0.00f},
{19.51f, -1.11f, -3.52f, -152.76f, -1.11f, -329.51f, 0.00f, 1.00f, 0.00f},
{19.51f, -1.11f, -3.73f, -152.76f, -1.11f, -329.72f, 0.00f, 1.00f, 0.00f},
{19.30f, -1.11f, -3.73f, -152.97f, -1.11f, -329.72f, 0.00f, 1.00f, 0.00f},
{19.30f, -1.11f, -3.94f, -152.97f, -1.11f, -329.93f, 0.00f, 1.00f, 0.00f},
{19.09f, -1.11f, -3.94f, -153.18f, -1.11f, -329.93f, 0.00f, 1.00f, 0.00f},
{19.09f, -1.11f, -4.15f, -153.18f, -1.11f, -330.14f, 0.00f, 1.00f, 0.00f},
{18.88f, -1.11f, -4.15f, -153.39f, -1.11f, -330.14f, 0.00f, 1.00f, 0.00f},
{18.88f, -1.11f, -4.36f, -153.39f, -1.11f, -330.35f, 0.00f, 1.00f, 0.00f},
{18.67f, -1.11f, -4.36f, -153.60f, -1.11f, -330.35f, 0.00f, 1.00f, 0.00f},
{18.67f, -1.11f, -4.57f, -153.60f, -1.11f, -330.56f, 0.00f, 1.00f, 0.00f},
{18.46f, -1.11f, -4.57f, -153.81f, -1.11f, -330.56f, 0.00f, 1.00f, 0.00f},
{18.46f, -1.11f, -4.78f, -153.81f, -1.11f, -330.77f, 0.00f, 1.00f, 0.00f},
{18.25f, -1.11f, -4.78f, -154.02f, -1.11f, -330.77f, 0.00f, 1.00f, 0.00f},
{18.25f, -1.11f, -4.99f, -154.02f, -1.11f, -330.98f, 0.00f, 1.00f, 0.00f},
{18.04f, -1.11f, -4.99f, -154.23f, -1.11f, -330.98f, 0.00f, 1.00f, 0.00f},
{18.04f, -1.11f, -5.20f, -154.23f, -1.11f, -331.19f, 0.00f, 1.00f, 0.00f},
{17.83f, -1.11f, -5.20f, -154.44f, -1.11f, -331.19f, 0.00f, 1.00f, 0.00f},
{17.83f, -1.11f, -5.41f, -154.44f, -1.11f, -331.40f, 0.00f, 1.00f, 0.00f},
{17.62f, -1.11f, -5.41f, -154.65f, -1.11f, -331.40f, 0.00f, 1.00f, 0.00f},
{17.62f, -1.11f, -5.62f, -154.65f, -1.11f, -331.61f, 0.00f, 1.00f, 0.00f},
{17.41f, -1.11f, -5.62f, -154.86f, -1.11f, -331.61f, 0.00f, 1.00f, 0.00f},
{17.41f, -1.11f, -5.83f, -154.86f, -1.11f, -331.82f, 0.00f, 1.00f, 0.00f},
{17.20f, -1.11f, -5.83f, -155.07f, -1.11f, -331.82f, 0.00f, 1.00f, 0.00f},
{17.20f, -1.11f, -6.04f, -155.07f, -1.11f, -332.03f, 0.00f, 1.00f, 0.00f},
{16.99f, -1.11f, -6.04f, -155.28f, -1.11f, -332.03f, 0.00f, 1.00f, 0.00f},
{16.99f, -1.11f, -6.25f, -155.28f, -1.11f, -332.24f, 0.00f, 1.00f, 0.00f},
{16.78f, -1.11f, -6.25f, -155.49f, -1.11f, -332.24f, 0.00f, 1.00f, 0.00f},
{16.78f, -1.11f, -6.46f, -155.49f, -1.11f, -332.45f, 0.00f, 1.00f, 0.00f},
{16.57f, -1.11f, -6.46f, -155.70f, -1.11f, -332.45f, 0.00f, 1.00f, 0.00f},
{16.57f, -1.11f, -6.67f, -155.70f, -1.11f, -332.66f, 0.00f, 1.00f, 0.00f},
{16.57f, -1.11f, -6.88f, -155.70f, -1.11f, -332.87f, 0.00f, 1.00f, 0.00f},
{16.36f, -1.11f, -6.88f, -155.91f, -1.11f, -332.87f, 0.00f, 1.00f, 0.00f},
{16.36f, -1.11f, -7.09f, -155.91f, -1.11f, -333.08f, 0.00f, 1.00f, 0.00f},
{16.36f, -1.11f, -7.30f, -155.91f, -1.11f, -333.29f, 0.00f, 1.00f, 0.00f},
{16.15f, -1.11f, -7.30f, -156.12f, -1.11f, -333.29f, 0.00f, 1.00f, 0.00f},
{16.15f, -1.11f, -7.51f, -156.12f, -1.11f, -333.50f, 0.00f, 1.00f, 0.00f},
{16.15f, -1.11f, -7.72f, -156.12f, -1.11f, -333.71f, 0.00f, 1.00f, 0.00f},
{15.94f, -1.11f, -7.72f, -156.33f, -1.11f, -333.71f, 0.00f, 1.00f, 0.00f},
{15.94f, -1.11f, -7.93f, -156.33f, -1.11f, -333.92f, 0.00f, 1.00f, 0.00f},
{15.94f, -1.11f, -8.14f, -156.33f, -1.11f, -334.13f, 0.00f, 1.00f, 0.00f},
{15.73f, -1.11f, -8.14f, -156.54f, -1.11f, -334.13f, 0.00f, 1.00f, 0.00f},
{15.73f, -1.11f, -8.35f, -156.54f, -1.11f, -334.34f, 0.00f, 1.00f, 0.00f},
{15.73f, -1.11f, -8.56f, -156.54f, -1.11f, -334.55f, 0.00f, 1.00f, 0.00f},
{15.52f, -1.11f, -8.56f, -156.75f, -1.11f, -334.55f, 0.00f, 1.00f, 0.00f},
{15.52f, -1.11f, -8.77f, -156.75f, -1.11f, -334.76f, 0.00f, 1.00f, 0.00f},
{15.52f, -1.11f, -8.98f, -156.75f, -1.11f, -334.97f, 0.00f, 1.00f, 0.00f},
{15.31f, -1.11f, -8.98f, -156.96f, -1.11f, -334.97f, 0.00f, 1.00f, 0.00f},
{15.31f, -1.11f, -9.19f, -156.96f, -1.11f, -335.18f, 0.00f, 1.00f, 0.00f},
{15.31f, -1.11f, -9.40f, -156.96f, -1.11f, -335.39f, 0.00f, 1.00f, 0.00f},
{15.10f, -1.11f, -9.40f, -157.17f, -1.11f, -335.39f, 0.00f, 1.00f, 0.00f},
{15.10f, -1.11f, -9.61f, -157.17f, -1.11f, -335.60f, 0.00f, 1.00f, 0.00f},
{15.10f, -1.11f, -9.82f, -157.17f, -1.11f, -335.81f, 0.00f, 1.00f, 0.00f},
{14.89f, -1.11f, -9.82f, -157.38f, -1.11f, -335.81f, 0.00f, 1.00f, 0.00f},
{14.89f, -1.11f, -10.03f, -157.38f, -1.11f, -336.02f, 0.00f, 1.00f, 0.00f},
{14.89f, -1.11f, -10.24f, -157.38f, -1.11f, -336.23f, 0.00f, 1.00f, 0.00f},
{14.68f, -1.11f, -10.24f, -157.59f, -1.11f, -336.23f, 0.00f, 1.00f, 0.00f},
{14.68f, -1.11f, -10.45f, -157.59f, -1.11f, -336.44f, 0.00f, 1.00f, 0.00f},
{14.68f, -1.11f, -10.66f, -157.59f, -1.11f, -336.65f, 0.00f, 1.00f, 0.00f},
{14.47f, -1.11f, -10.66f, -157.80f, -1.11f, -336.65f, 0.00f, 1.00f, 0.00f},
{14.47f, -1.11f, -10.87f, -157.80f, -1.11f, -336.86f, 0.00f, 1.00f, 0.00f},
{14.47f, -1.11f, -11.08f, -157.80f, -1.11f, -337.07f, 0.00f, 1.00f, 0.00f},
{14.26f, -1.11f, -11.08f, -158.01f, -1.11f, -337.07f, 0.00f, 1.00f, 0.00f},
{14.26f, -1.11f, -11.29f, -158.01f, -1.11f, -337.28f, 0.00f, 1.00f, 0.00f},
{14.26f, -1.11f, -11.50f, -158.01f, -1.11f, -337.49f, 0.00f, 1.00f, 0.00f},
{14.05f, -1.11f, -11.50f, -158.22f, -1.11f, -337.49f, 0.00f, 1.00f, 0.00f},
{14.05f, -1.11f, -11.71f, -158.22f, -1.11f, -337.70f, 0.00f, 1.00f, 0.00f},
{14.05f, -1.11f, -11.92f, -158.22f, -1.11f, -337.91f, 0.00f, 1.00f, 0.00f},
{13.84f, -1.11f, -11.92f, -158.43f, -1.11f, -337.91f, 0.00f, 1.00f, 0.00f},
{13.84f, -1.11f, -12.13f, -158.43f, -1.11f, -338.12f, 0.00f, 1.00f, 0.00f},
{13.84f, -1.11f, -12.34f, -158.43f, -1.11f, -338.33f, 0.00f, 1.00f, 0.00f},
{13.63f, -1.11f, -12.34f, -158.64f, -1.11f, -338.33f, 0.00f, 1.00f, 0.00f},
{13.63f, -1.11f, -12.55f, -158.64f, -1.11f, -338.54f, 0.00f, 1.00f, 0.00f},
{13.63f, -1.11f, -12.76f, -158.64f, -1.11f, -338.75f, 0.00f, 1.00f, 0.00f},
{13.42f, -1.11f, -12.76f, -158.85f, -1.11f, -338.75f, 0.00f, 1.00f, 0.00f},
{13.42f, -1.11f, -12.97f, -158.85f, -1.11f, -338.96f, 0.00f, 1.00f, 0.00f},
{13.42f, -1.11f, -13.18f, -158.85f, -1.11f, -339.17f, 0.00f, 1.00f, 0.00f},
{13.21f, -1.11f, -13.18f, -159.06f, -1.11f, -339.17f, 0.00f, 1.00f, 0.00f},
{13.21f, -1.11f, -13.39f, -159.06f, -1.11f, -339.38f, 0.00f, 1.00f, 0.00f},
{13.21f, -1.11f, -13.60f, -159.06f, -1.11f, -339.59f, 0.00f, 1.00f, 0.00f},
{13.00f, -1.11f, -13.60f, -159.27f, -1.11f, -339.59f, 0.00f, 1.00f, 0.00f},
{13.00f, -1.11f, -13.81f, -159.27f, -1.11f, -339.80f, 0.00f, 1.00f, 0.00f},
{13.00f, -1.11f, -14.02f, -159.27f, -1.11f, -340.01f, 0.00f, 1.00f, 0.00f},
{12.79f, -1.11f, -14.02f, -159.48f, -1.11f, -340.01f, 0.00f, 1.00f, 0.00f},
{12.79f, -1.11f, -14.23f, -159.48f, -1.11f, -340.22f, 0.00f, 1.00f, 0.00f},
{12.79f, -1.11f, -14.44f, -159.48f, -1.11f, -340.43f, 0.00f, 1.00f, 0.00f},
{12.58f, -1.11f, -14.44f, -159.69f, -1.11f, -340.43f, 0.00f, 1.00f, 0.00f},
{12.58f, -1.11f, -14.65f, -159.69f, -1.11f, -340.64f, 0.00f, 1.00f, 0.00f},
{12.58f, -1.11f, -14.86f, -159.69f, -1.11f, -340.85f, 0.00f, 1.00f, 0.00f},
{12.37f, -1.11f, -14.86f, -159.90f, -1.11f, -340.85f, 0.00f, 1.00f, 0.00f},
{12.37f, -1.11f, -15.07f, -159.90f, -1.11f, -341.06f, 0.00f, 1.00f, 0.00f},
{12.37f, -1.11f, -15.28f, -159.90f, -1.11f, -341.27f, 0.00f, 1.00f, 0.00f},
{12.16f, -1.11f, -15.28f, -160.11f, -1.11f, -341.27f, 0.00f, 1.00f, 0.00f},
{12.16f, -1.11f, -15.49f, -160.11f, -1.11f, -341.48f, 0.00f, 1.00f, 0.00f},
{12.16f, -1.11f, -15.70f, -160.11f, -1.11f, -341.69f, 0.00f, 1.00f, 0.00f},
{11.95f, -1.11f, -15.70f, -160.32f, -1.11f, -341.69f, 0.00f, 1.00f, 0.00f},
{11.95f, -1.11f, -15.91f, -160.32f, -1.11f, -341.90f, 0.00f, 1.00f, 0.00f},
{11.95f, -1.11f, -16.12f, -160.32f, -1.11f, -342.11f, 0.00f, 1.00f, 0.00f},
{11.74f, -1.11f, -16.12f, -160.53f, -1.11f, -342.11f, 0.00f, 1.00f, 0.00f},
{11.74f, -1.11f, -16.33f, -160.53f, -1.11f, -342.32f, 0.00f, 1.00f, 0.00f},
{11.74f, -1.11f, -16.54f, -160.53f, -1.11f, -342.53f, 0.00f, 1.00f, 0.00f},
{11.53f, -1.11f, -16.54f, -160.74f, -1.11f, -342.53f, 0.00f, 1.00f, 0.00f},
{11.53f, -1.11f, -16.75f, -160.74f, -1.11f, -342.74f, 0.00f, 1.00f, 0.00f},
{11.53f, -1.11f, -16.96f, -160.74f, -1.11f, -342.95f, 0.00f, 1.00f, 0.00f},
{11.32f, -1.11f, -16.96f, -160.95f, -1.11f, -342.95f, 0.00f, 1.00f, 0.00f},
{11.32f, -1.11f, -17.17f, -160.95f, -1.11f, -343.16f, 0.00f, 1.00f, 0.00f},
{11.32f, -1.11f, -17.38f, -160.95f, -1.11f, -343.37f, 0.00f, 1.00f, 0.00f},
{11.11f, -1.11f, -17.38f, -161.16f, -1.11f, -343.37f, 0.00f, 1.00f, 0.00f},
{11.11f, -1.11f, -17.59f, -161.16f, -1.11f, -343.58f, 0.00f, 1.00f, 0.00f},
{11.11f, -1.11f, -17.80f, -161.16f, -1.11f, -343.79f, 0.00f, 1.00f, 0.00f},
{10.90f, -1.11f, -17.80f, -161.37f, -1.11f, -343.79f, 0.00f, 1.00f, 0.00f},
{10.90f, -1.11f, -18.01f, -161.37f, -1.11f, -344.00f, 0.00f, 1.00f, 0.00f},
{10.90f, -1.11f, -18.22f, -161.37f, -1.11f, -344.21f, 0.00f, 1.00f, 0.00f},
{10.69f, -1.11f, -18.22f, -161.58f, -1.11f, -344.21f, 0.00f, 1.00f, 0.00f},
{10.69f, -1.11f, -18.43f, -161.58f, -1.11f, -344.42f, 0.00f, 1.00f, 0.00f},
{10.69f, -1.11f, -18.64f, -161.58f, -1.11f, -344.63f, 0.00f, 1.00f, 0.00f},
{10.48f, -1.11f, -18.64f, -161.79f, -1.11f, -344.63f, 0.00f, 1.00f, 0.00f},
{10.48f, -1.11f, -18.85f, -161.79f, -1.11f, -344.84f, 0.00f, 1.00f, 0.00f},
{10.48f, -1.11f, -19.06f, -161.79f, -1.11f, -345.05f, 0.00f, 1.00f, 0.00f},
{10.27f, -1.11f, -19.06f, -162.00f, -1.11f, -345.05f, 0.00f, 1.00f, 0.00f},
{10.27f, -1.11f, -19.27f, -162.00f, -1.11f, -345.26f, 0.00f, 1.00f, 0.00f},
{10.27f, -1.11f, -19.48f, -162.00f, -1.11f, -345.47f, 0.00f, 1.00f, 0.00f},
{10.06f, -1.11f, -19.48f, -162.21f, -1.11f, -345.47f, 0.00f, 1.00f, 0.00f},
{10.06f, -1.11f, -19.69f, -162.21f, -1.11f, -345.68f, 0.00f, 1.00f, 0.00f},
{10.06f, -1.11f, -19.90f, -162.21f, -1.11f, -345.89f, 0.00f, 1.00f, 0.00f},
{9.85f, -1.11f, -19.90f, -162.42f, -1.11f, -345.89f, 0.00f, 1.00f, 0.00f},
{9.85f, -1.11f, -20.11f, -162.42f, -1.11f, -346.10f, 0.00f, 1.00f, 0.00f},
{9.85f, -1.11f, -20.32f, -162.42f, -1.11f, -346.31f, 0.00f, 1.00f, 0.00f},
{9.64f, -1.11f, -20.32f, -162.63f, -1.11f, -346.31f, 0.00f, 1.00f, 0.00f},
{9.64f, -1.11f, -20.53f, -162.63f, -1.11f, -346.52f, 0.00f, 1.00f, 0.00f},
{9.64f, -1.11f, -20.74f, -162.63f, -1.11f, -346.73f, 0.00f, 1.00f, 0.00f},
{9.43f, -1.11f, -20.74f, -162.84f, -1.11f, -346.73f, 0.00f, 1.00f, 0.00f},
{9.43f, -1.11f, -20.95f, -162.84f, -1.11f, -346.94f, 0.00f, 1.00f, 0.00f},
{9.43f, -1.11f, -21.16f, -162.84f, -1.11f, -347.15f, 0.00f, 1.00f, 0.00f},
{9.22f, -1.11f, -21.16f, -163.05f, -1.11f, -347.15f, 0.00f, 1.00f, 0.00f},
{9.22f, -1.11f, -21.37f, -163.05f, -1.11f, -347.36f, 0.00f, 1.00f, 0.00f},
{9.22f, -1.11f, -21.58f, -163.05f, -1.11f, -347.57f, 0.00f, 1.00f, 0.00f},
{9.01f, -1.11f, -21.58f, -163.26f, -1.11f, -347.57f, 0.00f, 1.00f, 0.00f},
{9.01f, -1.11f, -21.79f, -163.26f, -1.11f, -347.78f, 0.00f, 1.00f, 0.00f},
{9.01f, -1.11f, -22.00f, -163.26f, -1.11f, -347.99f, 0.00f, 1.00f, 0.00f},
{9.01f, -1.11f, -22.21f, -163.26f, -1.11f, -348.20f, 0.00f, 1.00f, 0.00f},
{8.80f, -1.11f, -22.21f, -163.47f, -1.11f, -348.20f, 0.00f, 1.00f, 0.00f},
{8.80f, -1.11f, -22.42f, -163.47f, -1.11f, -348.41f, 0.00f, 1.00f, 0.00f},
{8.80f, -1.11f, -22.63f, -163.47f, -1.11f, -348.62f, 0.00f, 1.00f, 0.00f},
{8.59f, -1.11f, -22.63f, -163.68f, -1.11f, -348.62f, 0.00f, 1.00f, 0.00f},
{8.59f, -1.11f, -22.84f, -163.68f, -1.11f, -348.83f, 0.00f, 1.00f, 0.00f},
{8.59f, -1.11f, -23.05f, -163.68f, -1.11f, -349.04f, 0.00f, 1.00f, 0.00f},
{8.38f, -1.11f, -23.05f, -163.89f, -1.11f, -349.04f, 0.00f, 1.00f, 0.00f},
{8.38f, -1.11f, -23.26f, -163.89f, -1.11f, -349.25f, 0.00f, 1.00f, 0.00f},
{8.38f, -1.11f, -23.47f, -163.89f, -1.11f, -349.46f, 0.00f, 1.00f, 0.00f},
{8.17f, -1.11f, -23.47f, -164.10f, -1.11f, -349.46f, 0.00f, 1.00f, 0.00f},
{8.17f, -1.11f, -23.68f, -164.10f, -1.11f, -349.67f, 0.00f, 1.00f, 0.00f},
{8.17f, -1.11f, -23.89f, -164.10f, -1.11f, -349.88f, 0.00f, 1.00f, 0.00f},
{7.96f, -1.11f, -23.89f, -164.31f, -1.11f, -349.88f, 0.00f, 1.00f, 0.00f},
{7.96f, -1.11f, -24.10f, -164.31f, -1.11f, -350.09f, 0.00f, 1.00f, 0.00f},
{7.96f, -1.11f, -24.31f, -164.31f, -1.11f, -350.30f, 0.00f, 1.00f, 0.00f},
{7.75f, -1.11f, -24.31f, -164.52f, -1.11f, -350.30f, 0.00f, 1.00f, 0.00f},
{7.75f, -1.11f, -24.52f, -164.52f, -1.11f, -350.51f, 0.00f, 1.00f, 0.00f},
{7.75f, -1.11f, -24.73f, -164.52f, -1.11f, -350.72f, 0.00f, 1.00f, 0.00f},
{7.54f, -1.11f, -24.73f, -164.73f, -1.11f, -350.72f, 0.00f, 1.00f, 0.00f},
{7.54f, -1.11f, -24.94f, -164.73f, -1.11f, -350.93f, 0.00f, 1.00f, 0.00f},
{7.54f, -1.11f, -25.15f, -164.73f, -1.11f, -351.14f, 0.00f, 1.00f, 0.00f},
{7.33f, -1.11f, -25.15f, -164.94f, -1.11f, -351.14f, 0.00f, 1.00f, 0.00f},
{7.33f, -1.11f, -25.36f, -164.94f, -1.11f, -351.35f, 0.00f, 1.00f, 0.00f},
{7.33f, -1.11f, -25.57f, -164.94f, -1.11f, -351.56f, 0.00f, 1.00f, 0.00f},
{7.33f, -1.11f, -25.78f, -164.94f, -1.11f, -351.77f, 0.00f, 1.00f, 0.00f},
{7.12f, -1.11f, -25.78f, -165.15f, -1.11f, -351.77f, 0.00f, 1.00f, 0.00f},
{7.12f, -1.11f, -25.99f, -165.15f, -1.11f, -351.98f, 0.00f, 1.00f, 0.00f},
{7.12f, -1.11f, -26.20f, -165.15f, -1.11f, -352.19f, 0.00f, 1.00f, 0.00f},
{6.91f, -1.11f, -26.20f, -165.36f, -1.11f, -352.19f, 0.00f, 1.00f, 0.00f},
{6.91f, -1.11f, -26.41f, -165.36f, -1.11f, -352.40f, 0.00f, 1.00f, 0.00f},
{6.91f, -1.11f, -26.62f, -165.36f, -1.11f, -352.61f, 0.00f, 1.00f, 0.00f},
{6.70f, -1.11f, -26.62f, -165.57f, -1.11f, -352.61f, 0.00f, 1.00f, 0.00f},
{6.70f, -1.11f, -26.83f, -165.57f, -1.11f, -352.82f, 0.00f, 1.00f, 0.00f},
{6.70f, -1.11f, -27.04f, -165.57f, -1.11f, -353.03f, 0.00f, 1.00f, 0.00f},
{6.49f, -1.11f, -27.04f, -165.78f, -1.11f, -353.03f, 0.00f, 1.00f, 0.00f},
{6.49f, -1.11f, -27.25f, -165.78f, -1.11f, -353.24f, 0.00f, 1.00f, 0.00f},
{6.49f, -1.11f, -27.46f, -165.78f, -1.11f, -353.45f, 0.00f, 1.00f, 0.00f},
{6.28f, -1.11f, -27.46f, -165.99f, -1.11f, -353.45f, 0.00f, 1.00f, 0.00f},
{6.28f, -1.11f, -27.67f, -165.99f, -1.11f, -353.66f, 0.00f, 1.00f, 0.00f},
{6.28f, -1.11f, -27.88f, -165.99f, -1.11f, -353.87f, 0.00f, 1.00f, 0.00f},
{6.07f, -1.11f, -27.88f, -166.20f, -1.11f, -353.87f, 0.00f, 1.00f, 0.00f},
{6.07f, -1.11f, -28.09f, -166.20f, -1.11f, -354.08f, 0.00f, 1.00f, 0.00f},
{6.07f, -1.11f, -28.30f, -166.20f, -1.11f, -354.29f, 0.00f, 1.00f, 0.00f},
{5.86f, -1.11f, -28.30f, -166.41f, -1.11f, -354.29f, 0.00f, 1.00f, 0.00f},
{5.86f, -1.11f, -28.51f, -166.41f, -1.11f, -354.50f, 0.00f, 1.00f, 0.00f},
{5.86f, -1.11f, -28.72f, -166.41f, -1.11f, -354.71f, 0.00f, 1.00f, 0.00f},
{5.65f, -1.11f, -28.72f, -166.62f, -1.11f, -354.71f, 0.00f, 1.00f, 0.00f},
{5.65f, -1.11f, -28.93f, -166.62f, -1.11f, -354.92f, 0.00f, 1.00f, 0.00f},
{5.65f, -1.11f, -29.14f, -166.62f, -1.11f, -355.13f, 0.00f, 1.00f, 0.00f},
{5.44f, -1.11f, -29.14f, -166.83f, -1.11f, -355.13f, 0.00f, 1.00f, 0.00f},
{5.44f, -1.11f, -29.35f, -166.83f, -1.11f, -355.34f, 0.00f, 1.00f, 0.00f},
{5.44f, -1.11f, -29.56f, -166.83f, -1.11f, -355.55f, 0.00f, 1.00f, 0.00f},
{5.23f, -1.11f, -29.56f, -167.04f, -1.11f, -355.55f, 0.00f, 1.00f, 0.00f},
{5.23f, -1.11f, -29.77f, -167.04f, -1.11f, -355.76f, 0.00f, 1.00f, 0.00f},
{5.23f, -1.11f, -29.98f, -167.04f, -1.11f, -355.97f, 0.00f, 1.00f, 0.00f},
{5.02f, -1.11f, -29.98f, -167.25f, -1.11f, -355.97f, 0.00f, 1.00f, 0.00f},
{5.02f, -1.11f, -30.19f, -167.25f, -1.11f, -356.18f, 0.00f, 1.00f, 0.00f},
{5.02f, -1.11f, -30.40f, -167.25f, -1.11f, -356.39f, 0.00f, 1.00f, 0.00f},
{4.81f, -1.11f, -30.40f, -167.46f, -1.11f, -356.39f, 0.00f, 1.00f, 0.00f},
{4.81f, -1.11f, -30.61f, -318.08f, -1.11f, -168.80f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -30.61f, -318.29f, -1.11f, -168.80f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -30.82f, -318.29f, -1.11f, -169.01f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -31.03f, -318.29f, -1.11f, -169.22f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -31.24f, -318.29f, -1.11f, -169.43f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -31.45f, -318.29f, -1.11f, -169.64f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -31.66f, -318.29f, -1.11f, -169.85f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -31.87f, -318.29f, -1.11f, -170.06f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -32.08f, -318.29f, -1.11f, -170.27f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -32.29f, -318.29f, -1.11f, -170.48f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -32.50f, -318.29f, -1.11f, -170.69f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -32.71f, -318.29f, -1.11f, -170.90f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -32.92f, -318.29f, -1.11f, -171.11f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -33.13f, -318.29f, -1.11f, -171.32f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -33.34f, -318.29f, -1.11f, -171.53f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -33.55f, -318.29f, -1.11f, -171.74f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -33.76f, -318.29f, -1.11f, -171.95f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -33.97f, -318.29f, -1.11f, -172.16f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -34.18f, -318.29f, -1.11f, -172.37f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -34.39f, -318.29f, -1.11f, -172.58f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -34.60f, -318.29f, -1.11f, -172.79f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -34.81f, -318.29f, -1.11f, -173.00f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -35.02f, -318.29f, -1.11f, -173.21f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -35.23f, -318.29f, -1.11f, -173.42f, 0.00f, 1.00f, 0.00f},
{4.60f, -1.11f, -35.44f, -318.29f, -1.11f, -173.63f, 0.00f, 1.00f, 0.00f},
{4.39f, -1.11f, -35.44f, -318.50f, -1.11f, -173.63f, 0.00f, 1.00f, 0.00f},
{4.39f, -1.11f, -35.65f, -347.68f, -1.11f, -93.59f, 0.00f, 1.00f, 0.00f},
{4.18f, -1.11f, -35.65f, -347.89f, -1.11f, -93.59f, 0.00f, 1.00f, 0.00f},
{4.18f, -1.11f, -35.86f, -347.89f, -1.11f, -93.80f, 0.00f, 1.00f, 0.00f},
{3.97f, -1.11f, -35.86f, -348.10f, -1.11f, -93.80f, 0.00f, 1.00f, 0.00f},
{3.97f, -1.11f, -36.07f, -348.10f, -1.11f, -94.01f, 0.00f, 1.00f, 0.00f},
{3.76f, -1.11f, -36.07f, -348.31f, -1.11f, -94.01f, 0.00f, 1.00f, 0.00f},
{3.76f, -1.11f, -36.28f, -348.31f, -1.11f, -94.22f, 0.00f, 1.00f, 0.00f},
{3.55f, -1.11f, -36.28f, -348.52f, -1.11f, -94.22f, 0.00f, 1.00f, 0.00f},
{3.55f, -1.11f, -36.49f, -348.52f, -1.11f, -94.43f, 0.00f, 1.00f, 0.00f},
{3.34f, -1.11f, -36.49f, -348.73f, -1.11f, -94.43f, 0.00f, 1.00f, 0.00f},
{3.34f, -1.11f, -36.70f, -349.90f, -1.11f, -84.87f, 0.00f, 1.00f, 0.00f},
{3.34f, -1.11f, -36.91f, -353.70f, -1.11f, -67.27f, 0.00f, 1.00f, 0.00f},
{3.13f, -1.11f, -36.91f, -353.91f, -1.11f, -67.27f, 0.00f, 1.00f, 0.00f},
{3.13f, -1.11f, -37.12f, -355.26f, -1.11f, -58.41f, 0.00f, 1.00f, 0.00f},
{3.13f, -1.11f, -37.33f, -357.73f, -1.11f, -40.58f, 0.00f, 1.00f, 0.00f},
{2.92f, -1.11f, -37.33f, -357.94f, -1.11f, -40.58f, 0.00f, 1.00f, 0.00f},
{2.92f, -1.11f, -37.54f, -357.94f, -1.11f, -40.79f, 0.00f, 1.00f, 0.00f},
{2.71f, -1.11f, -37.54f, -359.96f, -1.11f, -13.45f, 0.00f, 1.00f, 0.00f},
{2.50f, -1.11f, -37.54f, -360.17f, -1.11f, -13.45f, 0.00f, 1.00f, 0.00f},
{2.29f, -1.11f, -37.54f, -360.38f, -1.11f, -13.45f, 0.00f, 1.00f, 0.00f},
{2.08f, -1.11f, -37.54f, -360.59f, -1.11f, -13.45f, 0.00f, 1.00f, 0.00f},
{1.87f, -1.11f, -37.54f, -360.18f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{1.66f, -1.11f, -37.54f, -360.39f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{1.45f, -1.11f, -37.54f, -360.60f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{1.24f, -1.11f, -37.54f, -360.81f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{1.03f, -1.11f, -37.54f, -361.02f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{0.82f, -1.11f, -37.54f, -361.23f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{0.61f, -1.11f, -37.54f, -361.44f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{0.40f, -1.11f, -37.54f, -361.65f, -1.11f, -4.46f, 0.00f, 1.00f, 0.00f},
{0.40f, -1.11f, -37.33f, -361.65f, -1.11f, -4.25f, 0.00f, 1.00f, 0.00f},
{0.40f, -1.11f, -37.12f, -361.65f, -1.11f, -4.04f, 0.00f, 1.00f, 0.00f},
{0.19f, -1.11f, -37.12f, -361.86f, -1.11f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.02f, -1.11f, -37.12f, -362.07f, -1.11f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.15f, -1.11f, -37.12f, -362.20f, -1.11f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.28f, -1.11f, -37.12f, -362.33f, -1.11f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.41f, -1.11f, -37.12f, -362.46f, -1.11f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.54f, -1.11f, -37.12f, -362.59f, -1.11f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.54f, -0.98f, -37.12f, -362.59f, -0.98f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.67f, -0.98f, -37.12f, -362.72f, -0.98f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.67f, -0.85f, -37.12f, -362.72f, -0.85f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.80f, -0.85f, -37.12f, -362.85f, -0.85f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.80f, -0.72f, -37.12f, -362.85f, -0.72f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.93f, -0.72f, -37.12f, -362.98f, -0.72f, -4.04f, 0.00f, 1.00f, 0.00f},
{-0.93f, -0.59f, -37.12f, -362.98f, -0.59f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.06f, -0.59f, -37.12f, -363.11f, -0.59f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.06f, -0.46f, -37.12f, -363.11f, -0.46f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.19f, -0.46f, -37.12f, -363.24f, -0.46f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.19f, -0.33f, -37.12f, -363.24f, -0.33f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.32f, -0.33f, -37.12f, -363.37f, -0.33f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.32f, -0.20f, -37.12f, -363.37f, -0.20f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.45f, -0.20f, -37.12f, -363.50f, -0.20f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.45f, -0.07f, -37.12f, -363.50f, -0.07f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.58f, -0.07f, -37.12f, -363.63f, -0.07f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.58f, 0.06f, -37.12f, -363.63f, 0.06f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.71f, 0.06f, -37.12f, -363.76f, 0.06f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.71f, 0.19f, -37.12f, -363.76f, 0.19f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.84f, 0.19f, -37.12f, -363.89f, 0.19f, -4.04f, 0.00f, 1.00f, 0.00f},
{-1.97f, 0.19f, -37.12f, -364.02f, 0.19f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.10f, 0.19f, -37.12f, -364.15f, 0.19f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.23f, 0.19f, -37.12f, -364.28f, 0.19f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.36f, 0.19f, -37.12f, -364.41f, 0.19f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.36f, 0.06f, -37.12f, -364.41f, 0.06f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.49f, 0.06f, -37.12f, -364.54f, 0.06f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.49f, -0.07f, -37.12f, -364.54f, -0.07f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.62f, -0.07f, -37.12f, -364.67f, -0.07f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.71f, -0.07f, -37.12f, -364.76f, -0.07f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.71f, -0.16f, -37.12f, -364.76f, -0.16f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.80f, -0.16f, -37.12f, -364.85f, -0.16f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.89f, -0.16f, -37.12f, -364.94f, -0.16f, -4.04f, 0.00f, 1.00f, 0.00f},
{-2.98f, -0.16f, -37.12f, -365.03f, -0.16f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.07f, -0.16f, -37.12f, -365.12f, -0.16f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.07f, -0.25f, -37.12f, -365.12f, -0.25f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.16f, -0.25f, -37.12f, -365.21f, -0.25f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.25f, -0.25f, -37.12f, -365.30f, -0.25f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.25f, -0.34f, -37.12f, -365.30f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.34f, -0.34f, -37.12f, -365.39f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.43f, -0.34f, -37.12f, -365.48f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.52f, -0.34f, -37.12f, -365.57f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.61f, -0.34f, -37.12f, -365.66f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.70f, -0.34f, -37.12f, -365.75f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.79f, -0.34f, -37.12f, -365.84f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.88f, -0.34f, -37.12f, -365.93f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-3.97f, -0.34f, -37.12f, -366.02f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.06f, -0.34f, -37.12f, -366.11f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.15f, -0.34f, -37.12f, -366.20f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.24f, -0.34f, -37.12f, -366.29f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.33f, -0.34f, -37.12f, -366.38f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.42f, -0.34f, -37.12f, -366.47f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.51f, -0.34f, -37.12f, -366.56f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.60f, -0.34f, -37.12f, -366.65f, -0.34f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.60f, -0.43f, -37.12f, -366.65f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.69f, -0.43f, -37.12f, -366.74f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.78f, -0.43f, -37.12f, -366.83f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.87f, -0.43f, -37.12f, -366.92f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-4.96f, -0.43f, -37.12f, -367.01f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.05f, -0.43f, -37.12f, -367.10f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.14f, -0.43f, -37.12f, -367.19f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.23f, -0.43f, -37.12f, -367.28f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.32f, -0.43f, -37.12f, -367.37f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.41f, -0.43f, -37.12f, -367.46f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.50f, -0.43f, -37.12f, -367.55f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.59f, -0.43f, -37.12f, -367.64f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.68f, -0.43f, -37.12f, -367.73f, -0.43f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.68f, -0.52f, -37.12f, -367.73f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.77f, -0.52f, -37.12f, -367.82f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.86f, -0.52f, -37.12f, -367.91f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-5.95f, -0.52f, -37.12f, -368.00f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.04f, -0.52f, -37.12f, -368.09f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.13f, -0.52f, -37.12f, -368.18f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.22f, -0.52f, -37.12f, -368.27f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.31f, -0.52f, -37.12f, -368.36f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.40f, -0.52f, -37.12f, -368.45f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.49f, -0.52f, -37.12f, -368.54f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.58f, -0.52f, -37.12f, -368.63f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.67f, -0.52f, -37.12f, -368.72f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.76f, -0.52f, -37.12f, -368.81f, -0.52f, -4.04f, 0.00f, 1.00f, 0.00f},
{-6.76f, -0.52f, -37.03f, -368.81f, -0.52f, -3.95f, 0.00f, 1.00f, 0.00f},
{-6.76f, -0.52f, -36.94f, -368.81f, -0.52f, -3.86f, 0.00f, 1.00f, 0.00f},
{-6.76f, -0.52f, -36.85f, -368.81f, -0.52f, -3.77f, 0.00f, 1.00f, 0.00f},
{-6.76f, -0.61f, -36.85f, -368.81f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-6.85f, -0.61f, -36.85f, -368.90f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-6.94f, -0.61f, -36.85f, -368.99f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.03f, -0.61f, -36.85f, -369.08f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.12f, -0.61f, -36.85f, -369.17f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.21f, -0.61f, -36.85f, -369.26f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.30f, -0.61f, -36.85f, -369.35f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.39f, -0.61f, -36.85f, -369.44f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.48f, -0.61f, -36.85f, -369.53f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.57f, -0.61f, -36.85f, -369.62f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.66f, -0.61f, -36.85f, -369.71f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.75f, -0.61f, -36.85f, -369.80f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.84f, -0.61f, -36.85f, -369.89f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-7.93f, -0.61f, -36.85f, -369.98f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.02f, -0.61f, -36.85f, -370.07f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.11f, -0.61f, -36.85f, -370.16f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.20f, -0.61f, -36.85f, -370.25f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.29f, -0.61f, -36.85f, -370.34f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.38f, -0.61f, -36.85f, -370.43f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.47f, -0.61f, -36.85f, -370.52f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.56f, -0.61f, -36.85f, -370.61f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.65f, -0.61f, -36.85f, -370.70f, -0.61f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.65f, -0.70f, -36.85f, -370.70f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.74f, -0.70f, -36.85f, -370.79f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.83f, -0.70f, -36.85f, -370.88f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-8.92f, -0.70f, -36.85f, -370.97f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.01f, -0.70f, -36.85f, -371.06f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.10f, -0.70f, -36.85f, -371.15f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.19f, -0.70f, -36.85f, -371.24f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.28f, -0.70f, -36.85f, -371.33f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.37f, -0.70f, -36.85f, -371.42f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.46f, -0.70f, -36.85f, -371.51f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.55f, -0.70f, -36.85f, -371.60f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.64f, -0.70f, -36.85f, -371.69f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.73f, -0.70f, -36.85f, -371.78f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.82f, -0.70f, -36.85f, -371.87f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-9.91f, -0.70f, -36.85f, -371.96f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.00f, -0.70f, -36.85f, -372.05f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.09f, -0.70f, -36.85f, -372.14f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.18f, -0.70f, -36.85f, -372.23f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.27f, -0.70f, -36.85f, -372.32f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.36f, -0.70f, -36.85f, -372.41f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.45f, -0.70f, -36.85f, -372.50f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.54f, -0.70f, -36.85f, -372.59f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.63f, -0.70f, -36.85f, -372.68f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.72f, -0.70f, -36.85f, -372.77f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.81f, -0.70f, -36.85f, -372.86f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.90f, -0.70f, -36.85f, -372.95f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.99f, -0.70f, -36.85f, -373.04f, -0.70f, -3.77f, 0.00f, 1.00f, 0.00f},
{-10.99f, -0.70f, -36.76f, -373.04f, -0.70f, -3.68f, 0.00f, 1.00f, 0.00f},
{-10.99f, -0.70f, -36.67f, -373.04f, -0.70f, -3.59f, 0.00f, 1.00f, 0.00f},
{-10.99f, -0.70f, -36.58f, -373.04f, -0.70f, -3.50f, 0.00f, 1.00f, 0.00f},
{-10.99f, -0.70f, -36.49f, -373.04f, -0.70f, -3.41f, 0.00f, 1.00f, 0.00f},
{-10.99f, -0.79f, -36.49f, -373.04f, -0.79f, -3.41f, 0.00f, 1.00f, 0.00f},
{-11.08f, -0.79f, -36.49f, -373.13f, -0.79f, -3.41f, 0.00f, 1.00f, 0.00f},
{-11.17f, -0.79f, -36.49f, -373.22f, -0.79f, -3.41f, 0.00f, 1.00f, 0.00f},
{-11.26f, -0.79f, -36.49f, -373.31f, -0.79f, -3.41f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -36.49f, -373.40f, -0.79f, -3.41f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -36.40f, -373.40f, -0.79f, -3.32f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -36.31f, -373.40f, -0.79f, -3.23f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -36.22f, -373.40f, -0.79f, -3.14f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -36.13f, -373.40f, -0.79f, -3.05f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -36.04f, -373.40f, -0.79f, -2.96f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -35.95f, -373.40f, -0.79f, -2.87f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -35.86f, -373.40f, -0.79f, -2.78f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -35.77f, -373.40f, -0.79f, -2.69f, 0.00f, 1.00f, 0.00f},
{-11.35f, -0.79f, -35.68f, -373.40f, -0.79f, -2.60f, 0.00f, 1.00f, 0.00f},
{-11.44f, -0.79f, -35.68f, -373.49f, -0.79f, -2.60f, 0.00f, 1.00f, 0.00f},
{-11.44f, -0.88f, -35.68f, -373.49f, -0.88f, -2.60f, 0.00f, 1.00f, 0.00f},
{-11.53f, -0.88f, -35.68f, -373.58f, -0.88f, -2.60f, 0.00f, 1.00f, 0.00f},
{-11.53f, -0.88f, -35.59f, -373.58f, -0.88f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.53f, -0.97f, -35.59f, -373.58f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.53f, -1.06f, -35.59f, -373.58f, -1.06f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.53f, -0.97f, -35.59f, -373.58f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.62f, -0.97f, -35.59f, -373.67f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.71f, -0.97f, -35.59f, -373.76f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.80f, -0.97f, -35.59f, -373.85f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.89f, -0.97f, -35.59f, -373.94f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-11.98f, -0.97f, -35.59f, -374.03f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.07f, -0.97f, -35.59f, -374.12f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.16f, -0.97f, -35.59f, -374.21f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.25f, -0.97f, -35.59f, -374.30f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.34f, -0.97f, -35.59f, -374.39f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.43f, -0.97f, -35.59f, -374.48f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.52f, -0.97f, -35.59f, -374.57f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.61f, -0.97f, -35.59f, -374.66f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.70f, -0.97f, -35.59f, -374.75f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.79f, -0.97f, -35.59f, -374.84f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.88f, -0.97f, -35.59f, -374.93f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.97f, -0.97f, -35.59f, -375.02f, -0.97f, -2.51f, 0.00f, 1.00f, 0.00f},
{-12.97f, -0.97f, -35.50f, -375.02f, -0.97f, -2.42f, 0.00f, 1.00f, 0.00f},
{-12.97f, -0.97f, -35.41f, -375.02f, -0.97f, -2.33f, 0.00f, 1.00f, 0.00f},
{-12.97f, -1.06f, -35.41f, -375.02f, -1.06f, -2.33f, 0.00f, 1.00f, 0.00f},
{-12.97f, -0.97f, -35.41f, -375.02f, -0.97f, -2.33f, 0.00f, 1.00f, 0.00f},
{-13.06f, -0.97f, -35.41f, -375.11f, -0.97f, -2.33f, 0.00f, 1.00f, 0.00f},
{-13.06f, -0.97f, -35.32f, -375.11f, -0.97f, -2.24f, 0.00f, 1.00f, 0.00f},
{-13.15f, -0.97f, -35.32f, -375.20f, -0.97f, -2.24f, 0.00f, 1.00f, 0.00f},
{-13.24f, -0.97f, -35.32f, -375.29f, -0.97f, -2.24f, 0.00f, 1.00f, 0.00f},
{-13.24f, -0.97f, -35.23f, -375.29f, -0.97f, -2.15f, 0.00f, 1.00f, 0.00f},
{-13.33f, -0.97f, -35.23f, -375.38f, -0.97f, -2.15f, 0.00f, 1.00f, 0.00f},
{-13.42f, -0.97f, -35.23f, -375.47f, -0.97f, -2.15f, 0.00f, 1.00f, 0.00f},
{-13.42f, -0.97f, -35.14f, -375.47f, -0.97f, -2.06f, 0.00f, 1.00f, 0.00f},
{-13.51f, -0.97f, -35.14f, -375.56f, -0.97f, -2.06f, 0.00f, 1.00f, 0.00f},
{-13.60f, -0.97f, -35.14f, -375.65f, -0.97f, -2.06f, 0.00f, 1.00f, 0.00f},
{-13.60f, -0.97f, -35.05f, -375.65f, -0.97f, -1.97f, 0.00f, 1.00f, 0.00f},
{-13.69f, -0.97f, -35.05f, -375.74f, -0.97f, -1.97f, 0.00f, 1.00f, 0.00f},
{-13.78f, -0.97f, -35.05f, -375.83f, -0.97f, -1.97f, 0.00f, 1.00f, 0.00f},
{-13.78f, -0.97f, -34.96f, -375.83f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-13.87f, -0.97f, -34.96f, -375.92f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-13.96f, -0.97f, -34.96f, -376.01f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-13.96f, -0.97f, -34.87f, -376.01f, -0.97f, -1.79f, 0.00f, 1.00f, 0.00f},
{-13.96f, -0.97f, -34.96f, -376.01f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.05f, -0.97f, -34.96f, -376.10f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.14f, -0.97f, -34.96f, -376.19f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.23f, -0.97f, -34.96f, -376.28f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.32f, -0.97f, -34.96f, -376.37f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.41f, -0.97f, -34.96f, -376.46f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.50f, -0.97f, -34.96f, -376.55f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.59f, -0.97f, -34.96f, -376.64f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.68f, -0.97f, -34.96f, -376.73f, -0.97f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.68f, -1.06f, -34.96f, -376.73f, -1.06f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.68f, -1.15f, -34.96f, -376.73f, -1.15f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.68f, -1.24f, -34.96f, -376.73f, -1.24f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.77f, -1.24f, -34.96f, -376.82f, -1.24f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.77f, -1.33f, -34.96f, -376.82f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.86f, -1.33f, -34.96f, -376.91f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-14.95f, -1.33f, -34.96f, -377.00f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.04f, -1.33f, -34.96f, -377.09f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.13f, -1.33f, -34.96f, -377.18f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.22f, -1.33f, -34.96f, -377.27f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.31f, -1.33f, -34.96f, -377.36f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.40f, -1.33f, -34.96f, -377.45f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.49f, -1.33f, -34.96f, -377.54f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.58f, -1.33f, -34.96f, -377.63f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.67f, -1.33f, -34.96f, -377.72f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.76f, -1.33f, -34.96f, -377.81f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.85f, -1.33f, -34.96f, -377.90f, -1.33f, -1.88f, 0.00f, 1.00f, 0.00f},
{-15.85f, -1.33f, -34.87f, -377.90f, -1.33f, -1.79f, 0.00f, 1.00f, 0.00f},
{-15.85f, -1.33f, -34.78f, -377.90f, -1.33f, -1.70f, 0.00f, 1.00f, 0.00f},
{-15.94f, -1.33f, -34.78f, -377.99f, -1.33f, -1.70f, 0.00f, 1.00f, 0.00f},
{-15.94f, -1.33f, -34.69f, -377.99f, -1.33f, -1.61f, 0.00f, 1.00f, 0.00f},
{-16.03f, -1.33f, -34.69f, -378.08f, -1.33f, -1.61f, 0.00f, 1.00f, 0.00f},
{-16.03f, -1.33f, -34.69f, -378.08f, -1.33f, -1.61f, 0.00f, 1.00f, 0.00f}};


