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

static struct ADSUniform sceneOutdoorADSStaticUniform_10;
extern struct ADSDynamicUniform sceneOutdoorADSDynamicUniform;

#ifdef ENABLE_TERRIAN
extern struct TerrainUniform terrainUniform;
#endif // ENABLE_TERRIAN

#ifdef ENABLE_BILLBOARDING
// variables for billboarding
static TEXTURE texture_flower;
struct BillboardingUniform billboardingEffectUniform;
GLuint frameTime; // = 0;
#endif // ENABLE_BILLBOARDING

#ifdef ENABLE_WATER
// Water Related Variables
extern struct WaterUniform waterUniform;
extern struct TextureVariables waterTextureVariables;
extern struct WaterFrameBufferDetails waterReflectionFrameBufferDetails;
extern struct WaterFrameBufferDetails waterRefractionFrameBufferDetails;
#endif // ENABLE_WATER

#ifdef ENABLE_ATMOSPHERE
// Atmosphere Scattering
static AtmosphereUniform atmosphereUniform_10;
AtmosphericVariables atmosVariables_10;
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

quad_instancing_buffers_t instBuffers_leftflowers;
quad_instancing_buffers_t instBuffers_rightflowers;

float distance10;

bool isInitialDisplay_Scene10AdbhutRas = true;

GLuint texture_adbhutMask;

mat4 finalViewMatrix = mat4::identity();

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
	// if('1' == tf_Object) // terrain
	//	update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix, &rotationAngles) ;
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

//	if (i <= sizeof(cam_mov)/sizeof(cam_mov[0]))
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
		preciselerp_lookat(-0.71f, -0.76f, -45.00f, -362.13f, -0.76f, 21.77f, 0.00f, 1.00f, 0.00f, 0.0027f);
		if (cameraEyeZ < (-35.99f - 0.2f))
		{
			scene10_state = 3;
			LOG("Switching to state %d\n", scene10_state);
		}
	}

	if (scene10_state == 3) // top of the bridge
	{
		preciselerp_lookat(-20.21f, 1.75f, -37.50f, -365.63f, -0.76f, 21.77f, 0.00f, 1.00f, 0.00f, 0.0027f);
		if (cameraEyeX < (-4.21f - 0.2f))
		{
			scene10_state = 4;
			LOG("Switching to state %d\n", scene10_state);
		}
	}

	if (scene10_state == 4) // down to bridge
	{
		preciselerp_lookat(-30.35f, -2.11f, -33.00f, -377.14f, -1.11f, 25.08f, 0.00f, 1.00f, 0.00f, 0.0027f);
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

float cam_mov[][9] = {
	{21.90f, -1.11f, -1.18f, -150.37f, -1.11f, -327.17f, 0.00f, 1.00f, 0.00f},
	{21.85f, -1.11f, -1.18f, -150.42f, -1.11f, -327.17f, 0.00f, 1.00f, 0.00f}
};


