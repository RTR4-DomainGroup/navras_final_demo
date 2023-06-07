#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/constants.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../inc/shaders/BillboardingShader.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
#include "../../inc/effects/GaussianBlurEffect.h"

#include "../../inc/scenes/scenePlaceHolderIndoor.h"
#include "../../inc/scenes/scene07_Raudra.h"
#include "../../inc/debug/debug_transformation.h"

#ifdef ENABLE_GAUSSIAN_BLUR
#include "../../inc/effects/GaussianBlurEffect.h"
#endif // ENABLE_GAUSSIAN_BLUR


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

#ifdef ENABLE_GAUSSIAN_BLUR
// Gaussian Blur related variables
static struct GaussianBlurEffect gaussianBlurEffect;
static struct HorrizontalBlurUniform horizontalBlurUniform;
static struct VerticalBlurUniform verticalBlurUniform;
static struct FrameBufferDetails fullSceneFbo;
static struct FSQuadUniform fsGaussBlurQuadUniform;
#endif // ENABLE_GAUSSIAN_BLUR

extern mat4 perspectiveProjectionMatrix;

static struct ADSUniform sceneIndoorADSUniform;

extern GLfloat density;
extern GLfloat gradient;
extern GLfloat skyFogColor[];

GLfloat lightAmbient_raudra[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse_raudra[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular_raudra[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition_raudra[] = { 10.0f, 10.0f, 0.0f, 1.0f };

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

//Model variables
STATIC_MODEL deskModelRaudra;
STATIC_MODEL shelfModel;
STATIC_MODEL pencilModelRaudra;
STATIC_MODEL tempBoyModelRaudra;

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
	initializeShelfInstancePositions();
#endif
	initializeInvertedNormalCube();
	initializeQuad();

	textures[0] = (GLuint)texture_ceiling;
	textures[1] = (GLuint)texture_floor;
	textures[2] = (GLuint)texture_back;
	textures[3] = (GLuint)texture_side;

	loadStaticModel("res/models/pencil/Pencil.fbx", &pencilModelRaudra);
	loadStaticModel("res/models/scene07_raudra/tempKarunBoy1.obj", &tempBoyModelRaudra);


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
		setCamera(2.00f, 0.00f, 1.45f, 2.00f, 0.00f, -1.75f, 0.0f, 1.0f, 0.0f);
		isInitialDisplayScene07_RaudraRas = false;
	}
}


void displayScene07_Raudra(void)
{
    // set camera
	setCameraScene07_RaudraRas();

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
	

	translationMatrix = vmath::translate(0.0f, 0.0f, -1.0f);
	scaleMatrix = vmath::scale(4.0f, 2.0f, 5.0f);
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
	translationMatrix = vmath::translate(1.0f, 0.0f, 1.49f);
	scaleMatrix = vmath::scale(2.0f, 1.0f, 1.0f);
	modelMatrix = translationMatrix * scaleMatrix;
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
	translationMatrix = vmath::translate(0.01f, -0.375f, 0.95f);
	scaleMatrix = vmath::scale(0.01f, 1.2f, 1.0f);
	rotationMatrix = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
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
	translationMatrix = vmath::translate(1.85f, -0.85f, -9.55f);
	//scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * rotationMatrix;

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


	translationMatrix = vmath::translate(3.50f, 0.0f, -3.35f);
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
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

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

	translationMatrix = vmath::translate(1.85f, -0.385f, -1.10f);
	//translationMatrix = vmath::translate(1.85f, -0.34f, -1.1f);
	scaleMatrix = vmath::scale(0.05f, 0.05f, 0.05f);
	//rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	
	drawStaticModel(pencilModelRaudra);

	// ------ Temp Boy Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(1.85f, -0.385f, -1.10f);
	//translationMatrix = vmath::translate(1.85f, -0.34f, -1.1f);
	scaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);
	//rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(tempBoyModelRaudra);

	// ------ Failed Texture ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(1.9f, -0.395f, -1.12f);
	//translationMatrix = vmath::translate(1.75f, -0.54f, -1.1f);
	scaleMatrix = vmath::scale(0.15f, 0.12f, 0.2f);
	rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	//rotationMatrix_y = vmath::rotate(90.0f, 0.0f, 1.0f, 0.0f);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_failed);
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Un-use ShaderProgramObject
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
	unloadStaticModel(&deskModelRaudra);
	unloadStaticModel(&tempBoyModelRaudra);

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

