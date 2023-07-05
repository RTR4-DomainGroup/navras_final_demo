#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/constants.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../inc/shaders/BillboardingShader.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
#include "../../inc/effects/GaussianBlurEffect.h"

#include "../../inc/scenes/scenePlaceHolderIndoor.h"
#include "../../inc/scenes/scene07_Raudra.h"
#include "../../inc/debug/debug_transformation.h"

#ifdef ENABLE_GAUSSIAN_BLUR
#include "../../inc/effects/GaussianBlurEffect.h"
#endif // ENABLE_GAUSSIAN_BLUR
#define X_INCREMENT_SHELF 0.0f
#define X_SEPARTION_OFFSET_SHELF 100.0f
#define NO_OF_INSTANCES_DESK 6
#define NO_OF_INSTANCES_SHELF 2

#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static struct ADSUniform sceneIndoorADSUniform;
static struct ADSDynamicUniform raudraDynamicADSUniform;

extern GLfloat density;
extern GLfloat gradient;
extern GLfloat skyFogColor[];

GLfloat lightAmbient_raudra[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse_raudra[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular_raudra[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition_raudra[] = { 0.0f, 0.0f, 20.0f, 1.0f };

GLfloat materialAmbient_raudra[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse_raudra[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular_raudra[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess_raudra = 128.0f;

static GLuint texture_ceiling;
static GLuint texture_floor;
static GLuint texture_side;
static GLuint texture_back;
static GLuint texture_blackboard;
static GLuint texture_failed;
static GLuint texture_door;
static GLuint texture_worldMap;

//Model variables
static STATIC_MODEL deskModelRaudra;
static STATIC_MODEL shelfModel;
static STATIC_MODEL pencilModelRaudra;
static STATIC_MODEL tempBoyModelRaudra;
static DYNAMIC_MODEL boyModelRaudra;


static GLuint textures[4];

GLuint texture_raudraMask;
bool isInitialDisplayScene07_RaudraRas = true;


int initializeScene07_Raudra(void)
{
//#ifdef ENABLE_MASKSQUADS
	if (LoadGLTexture_UsingSOIL(&texture_raudraMask, TEXTURE_DIR"Masks\\RaudraMask.jpg") == FALSE)
	{
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED in Raudra for Raudra Mask !!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull for Raudra Mask = %u!!!\n", texture_raudraMask);
	}
//#endif // ENABLE_MASKSQUADS

#ifdef ENABLE_STATIC_MODELS
	// function declarations
	void initializeDeskInstancePositions(void);
	void initializeShelfInstancePositions(void);

	//load models
	initializeDeskInstancePositions();
	
	initializeShelfInstancePositions();

	loadStaticModel("res/models/pencil/Pencil.fbx", &pencilModelRaudra);
	loadStaticModel("res/models/scene07_raudra/tempKarunBoy1.obj", &tempBoyModelRaudra);

#endif

#ifdef	ENABLE_DYNAMIC_MODELS
	loadDynamicModel("res/models/scene07_raudra/raudraBoyAnim.fbx", &boyModelRaudra);
#endif

	if (LoadGLTexture_UsingSOIL(&texture_ceiling, TEXTURE_DIR"Room/ceiling.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED in Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_ceiling);
	}

	if (LoadGLTexture_UsingSOIL(&texture_floor, TEXTURE_DIR"Room/floor.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED in floor Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_floor);
	}
	if (LoadGLTexture_UsingSOIL(&texture_back, TEXTURE_DIR"Room/back.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED in backwall Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_back);
	}
	if (LoadGLTexture_UsingSOIL(&texture_side, TEXTURE_DIR"Room/sidewall.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for sidewall Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_side);
	}

	if (LoadGLTexture_UsingSOIL(&texture_blackboard, TEXTURE_DIR"Roudra/blackboard.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for blackboard Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_blackboard);
	}

	if (LoadGLTexture_UsingSOIL(&texture_failed, TEXTURE_DIR"Roudra/failed.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for Marksheet Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_failed);
	}

	if (LoadGLTexture_UsingSOIL(&texture_door, TEXTURE_DIR"Roudra/door.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for Door Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_door);
	}
	if (LoadGLTexture_UsingSOIL(&texture_worldMap, TEXTURE_DIR"Roudra/world_map2.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for World Map Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture for World Map Successfull = %u!!!\n", texture_worldMap);
	}
	initializeInvertedNormalCube();
	initializeQuad();

	textures[0] = (GLuint)texture_ceiling;
	textures[1] = (GLuint)texture_floor;
	textures[2] = (GLuint)texture_back;
	textures[3] = (GLuint)texture_side;

	

	// external debugging varaible
    tf_t = {1.8f, -0.4f, -1.1f}; // tree pos 
    // tf_s = {0.75f, 0.75f, 0.75f}; // tree scale 
    // tf_r = {0.0f, 0.0f, 0.0f}; // tree rotation 
	tf_Speed = 0.05f;
//	glEnable(GL_TEXTURE_2D);
	return 0;
}

void initializeDeskInstancePositions(void)
{
	LOG("Enter Raudra Initialize==================== \n");
	float instance_positions[NO_OF_INSTANCES_DESK * 3] = {};
	
	for(int i = 0; i < NO_OF_INSTANCES_DESK; i++)
    {
        static GLfloat xPos = X_MIN;
        static GLfloat xMinLast = X_MIN;
        static GLfloat xMaxLast = X_MAX;

        static GLfloat yPos = Y_MIN;
        static GLfloat zPos = Z_MIN;

		instance_positions[(i*3)+0] = xPos;
		instance_positions[(i*3)+1] = 0.0f;
		instance_positions[(i*3)+2] = zPos;

		LOG("Desk Instance %d Position: [%.02f %.02f %.02f]\n", i, 
            instance_positions[(i*3)+0], instance_positions[(i*3)+1], instance_positions[(i*3)+2]);

        xPos += X_INCREMENT + X_SEPARTION_OFFSET;    
        //zPos += Z_INCREMENT + Z_SEPARTION_OFFSET;    
        if(xPos >= xMaxLast)
        {    
            zPos += Z_INCREMENT;    
            if(zPos >= Z_MAX)
            {
                break;
            }    
            // xMinLast = xMinLast + (X_INCREMENT/2);
            // xMaxLast = xMaxLast - (X_INCREMENT/2);
            xPos = xMinLast;
        }
    }

	vector<float> tmpPositions {instance_positions, instance_positions + (NO_OF_INSTANCES_DESK * 3)};
	loadStaticModelInstanced("res/models/desk/desk.obj", &deskModelRaudra, NO_OF_INSTANCES_DESK, tmpPositions);
}

void initializeShelfInstancePositions(void)
{
	float instance_positions[NO_OF_INSTANCES_SHELF * 3] = {};
	GLfloat xPos = X_MIN_ROUDRA_SHELF;
    GLfloat xMinLast = X_MIN_ROUDRA_SHELF;

    GLfloat zPos = Z_MIN_ROUDRA_SHELF;

	for(int i = 0; i < NO_OF_INSTANCES_SHELF; i++)
    {
		instance_positions[(i*3)+0] = xPos;
		instance_positions[(i*3)+1] = 0.0f;
		instance_positions[(i*3)+2] = zPos;

		LOG("Shelf Instance %d Position: [%.02f %.02f %.02f]\n", i, 
            instance_positions[(i*3)+0], instance_positions[(i*3)+1], instance_positions[(i*3)+2]);

        xPos += (X_INCREMENT_SHELF + X_SEPARTION_OFFSET_SHELF);
        //zPos += Z_INCREMENT + Z_SEPARTION_OFFSET;    
        
    }

	vector<float> tmpPositions {instance_positions, instance_positions + (NO_OF_INSTANCES_SHELF * 3)};
	loadStaticModelInstanced("res/models/shelf/Shelf.fbx", &shelfModel, NO_OF_INSTANCES_SHELF, tmpPositions);
}

void setCameraScene07_RaudraRas(void)
{
	if (isInitialDisplayScene07_RaudraRas == true)
	{
		setCamera(0.15f, -0.35f, 3.05f, 0.15f, -0.35f, -0.15f, 0.00f, 1.00f, 0.00f);
		isInitialDisplayScene07_RaudraRas = false;
	}
}

void displayScene07_Raudra()
{  

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	displayCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
#ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneIndoorADSUniform = useADSShader();

	glUniform1i(sceneIndoorADSUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneIndoorADSUniform.laUniform, 1, lightAmbient_raudra);
	glUniform4fv(sceneIndoorADSUniform.ldUniform, 1, lightDiffuse_raudra);
	glUniform4fv(sceneIndoorADSUniform.lsUniform, 1, lightSpecular_raudra);
	glUniform4fv(sceneIndoorADSUniform.lightPositionUniform, 1, lightPosition_raudra);
	glUniform4fv(sceneIndoorADSUniform.kaUniform, 1, materialAmbient_raudra);
	glUniform4fv(sceneIndoorADSUniform.kdUniform, 1, materialDiffuse_raudra);
	glUniform4fv(sceneIndoorADSUniform.ksUniform, 1, materialSpecular_raudra);
	glUniform1f(sceneIndoorADSUniform.materialShininessUniform, materialShininess_raudra);

	glUniform1i(sceneIndoorADSUniform.fogEnableUniform, 0);
	glUniform1f(sceneIndoorADSUniform.densityUniform, density);
	glUniform1f(sceneIndoorADSUniform.gradientUniform, gradient);
	glUniform4fv(sceneIndoorADSUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneIndoorADSUniform.uniform_enable_godRays, 1);
	glUniform1i(sceneIndoorADSUniform.godrays_blackpass_sphere, 0);
	glUniform1i(sceneIndoorADSUniform.actualSceneUniform, 1);
	glUniform1i(sceneIndoorADSUniform.depthSceneUniform, 0);
	glUniform1i(sceneIndoorADSUniform.depthQuadSceneUniform, 0);
	glUniform1i(sceneIndoorADSUniform.isInstanced, 0);


	glUniform1f(sceneIndoorADSUniform.colorCorrectionUniform, 0.7f);
	

	translationMatrix = vmath::translate(0.0f, 0.0f, -1.0f);
	scaleMatrix = vmath::scale(3.0f, 1.5f, 4.0f);
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	//glEnable(GL_TEXTURE_2D);
	
	displayRoom(textures);
	glBindTexture(GL_TEXTURE_2D, 0);

#ifdef ENABLE_MASKSQUADS
	// Transformations For Mask Quad
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(2.0f, 0.0f, -2.0f);
	scaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
	//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_raudraMask);
	glUniform1i(sceneIndoorADSUniform.textureSamplerUniform_diffuse, 0);
		displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
#endif

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();
	translationMatrix = vmath::translate(0.25f, 0.00f, 2.99f);
	scaleMatrix = vmath::scale(2.0f, 1.0f, 1.0f);
	
	modelMatrix = translationMatrix * scaleMatrix;
	glUniform1i(sceneIndoorADSUniform.isInstanced, 0);
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_blackboard);	
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();
	translationMatrix = vmath::translate(-2.99f, -0.49f, 1.20f);
	scaleMatrix = vmath::scale(0.005f, 1.0f, 0.8f);
	rotationMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	glUniform1i(sceneIndoorADSUniform.isInstanced, 0);
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_door);	
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

    // ------ Desk Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	//translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	translationMatrix = vmath::translate(-0.10f, -1.35f, -8.00f);
	//scaleMatrix = vmath::scale(0.85f, 0.85f, 0.85f);
	rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
	
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	glUniform1i(sceneIndoorADSUniform.isInstanced, 1);
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModelInstanced(deskModelRaudra, NO_OF_INSTANCES_DESK);
	// ------ Shelf Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();


	translationMatrix = vmath::translate(2.25f, 0.00f, -4.85f);
	scaleMatrix = vmath::scale(0.075f, 0.1f, 0.075f);
	rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	
	//translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	//scaleMatrix = vmath::scale(tf_s.x, tf_s.x, tf_s.x);
	// rotationMatrix_x = vmath::rotate(tf_r.x, 1.0f, 0.0f, 0.0f);
	// rotationMatrix_y = vmath::rotate(tf_r.y, 0.0f, 1.0f, 0.0f);
	// rotationMatrix_z = vmath::rotate(tf_r.z, 1.0f, 0.0f, 1.0f);
	// rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	//update_transformations(&translationMatrix, NULL, NULL, NULL);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	glUniform1i(sceneIndoorADSUniform.isInstanced, 1);
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	drawStaticModelInstanced(shelfModel, NO_OF_INSTANCES_SHELF);
	// LOG("Display drawStaticModelInstanced Finished.....\n ");

	// ------ Pencil Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(-0.1f, -0.89f, -2.1f);
	//translationMatrix = vmath::translate(1.85f, -0.34f, -1.1f);
	scaleMatrix = vmath::scale(0.03f, 0.03f, 0.03f);
	//rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	glUniform1i(sceneIndoorADSUniform.isInstanced, 0);
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	
	drawStaticModel(pencilModelRaudra);

	
	//drawStaticModel(tempBoyModelRaudra);

	// ------ Failed Texture ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.15f, -0.89f, -2.1f);
	//translationMatrix = vmath::translate(1.75f, -0.54f, -1.1f);
	scaleMatrix = vmath::scale(0.12f, 0.12f, 0.15f);
	rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	//rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	glUniform1i(sceneIndoorADSUniform.isInstanced, 0);
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_failed);
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);


	// ------ WorldMap Texture ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.35f, 0.0f, -4.98f);
	//translationMatrix = vmath::translate(1.75f, -0.54f, -1.1f);
	scaleMatrix = vmath::scale(1.5f, 1.0f, 0.0f);
	//rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	//rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	//rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	glUniform1i(sceneIndoorADSUniform.isInstanced, 0);
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_worldMap);
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Un-use ShaderProgramObject
	glUseProgram(0);
#endif
	// ------ Dynamic Boy Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

#ifdef ENABLE_DYNAMIC_MODELS
	glm::mat4 glm_modelMatrix;
	glm::mat4 glm_translateMatrix;
	glm::mat4 glm_rotateMatrix;
	glm::mat4 glm_scaleMatrix;

	glm_modelMatrix = glm::mat4(1.0f);
	glm_translateMatrix = glm::mat4(1.0f);
	glm_rotateMatrix = glm::mat4(1.0f);
	glm_scaleMatrix = glm::mat4(1.0f);

	raudraDynamicADSUniform  = useADSDynamicShader();
	glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, -1.23f, -2.32f));
	glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.0078f, 0.0078f, 0.0078f));

	glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	//update_transformations_glm(&glm_translateMatrix, &glm_scaleMatrix, NULL, NULL);
	glm_modelMatrix = glm_translateMatrix * glm_scaleMatrix * glm_rotateMatrix;

	glUniform4fv(raudraDynamicADSUniform.laUniform, 1, lightAmbient_raudra);
	glUniform4fv(raudraDynamicADSUniform.ldUniform, 1, lightDiffuse_raudra);
	glUniform4fv(raudraDynamicADSUniform.lsUniform, 1, lightSpecular_raudra);
	glUniform4fv(raudraDynamicADSUniform.lightPositionUniform, 1, lightPosition_raudra);
	glUniform4fv(raudraDynamicADSUniform.kaUniform, 1, materialAmbient_raudra);
	glUniform4fv(raudraDynamicADSUniform.kdUniform, 1, materialDiffuse_raudra);
	glUniform4fv(raudraDynamicADSUniform.ksUniform, 1, materialSpecular_raudra);
	glUniform1f(raudraDynamicADSUniform.materialShininessUniform, materialShininess_raudra);

	glUniform1i(raudraDynamicADSUniform.fogEnableUniform, 0);
	glUniform1f(raudraDynamicADSUniform.densityUniform, density);
	glUniform1f(raudraDynamicADSUniform.gradientUniform, gradient);
	glUniform4fv(raudraDynamicADSUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(raudraDynamicADSUniform.uniform_enable_godRays, 1);
	glUniform1i(raudraDynamicADSUniform.godrays_blackpass_sphere, 0);
	glUniform1i(raudraDynamicADSUniform.actualSceneUniform, 1);
	glUniform1i(raudraDynamicADSUniform.depthSceneUniform, 0);
	glUniform1i(raudraDynamicADSUniform.depthQuadSceneUniform, 0);

	glUniform1f(raudraDynamicADSUniform.colorCorrectionUniform, 0.1f);

	glUniformMatrix4fv(raudraDynamicADSUniform.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(glm_modelMatrix));
	glUniformMatrix4fv(raudraDynamicADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(raudraDynamicADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	static bool replay_animation = true;
	if (replay_animation) // replay animation
	{
		LOG("Replaying model animation\n");
		reDrawDynamicModel(raudraDynamicADSUniform, boyModelRaudra, 1.0f);
		replay_animation = false;
	}
	else
		drawDynamicModel(raudraDynamicADSUniform, boyModelRaudra, 0.8f);
	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
#endif 

}

void updateScene07_RaudraRas(void)
{
	// Code
#ifdef ENABLE_CAMERA_ANIMATION
	 cameraEyeZ = preciselerp(cameraEyeZ, 0.1f, 0.002f);
	 cameraCenterZ = preciselerp(cameraCenterZ, -3.10f, 0.002f);
#endif // ENABLE_CAMERA_ANIMATION

}

void uninitializeScene07_Raudra(void)
{
    //UNINIT models
	#ifdef ENABLE_STATIC_MODELS
	unloadStaticModel(&deskModelRaudra);
	unloadStaticModel(&tempBoyModelRaudra);
	#endif


#ifdef ENABLE_MASKSQUADS
	if (texture_raudraMask)
	{
		glDeleteTextures(1, &texture_raudraMask);
		texture_raudraMask = 0;
	}
#endif

	if (texture_ceiling)
	{
		glDeleteTextures(1, &texture_ceiling);
		texture_ceiling = 0;
	}
	if (texture_floor)
	{
		glDeleteTextures(1, &texture_floor);
		texture_floor = 0;
	}
	if (texture_back)
	{
		glDeleteTextures(1, &texture_back);
		texture_floor = 0;
	}
	if (texture_side)
	{
		glDeleteTextures(1, &texture_side);
		texture_side = 0;
	}
	if (texture_blackboard)
	{
		glDeleteTextures(1, &texture_blackboard);
		texture_blackboard = 0;
	}
	if (texture_failed)
	{
		glDeleteTextures(1, &texture_failed);
		texture_failed = 0;
	}
	if (texture_door)
	{
		glDeleteTextures(1, &texture_door);
		texture_door = 0;
	}
}

