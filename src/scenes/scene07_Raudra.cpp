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


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static struct ADSUniform sceneIndoorADSUniform;

extern GLfloat density;
extern GLfloat gradient;
extern GLfloat skyFogColor[];

static GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 10.0f, 10.0f, 0.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

static GLuint texture_ceiling;
static GLuint texture_floor;
static GLuint texture_side;
static GLuint texture_back;
static GLuint texture_blackboard;
static GLuint texture_failed;
static GLuint texture_door;

//Model variables
static STATIC_MODEL deskModel;
STATIC_MODEL shelfModel;

static GLuint textures[4];
int initializeScene07_Raudra(void)
{
#ifdef ENABLE_STATIC_MODELS
	// function declarations
	void initializeDeskInstancePositions(void);
	void initializeShelfInstancePositions(void);

	//load models
	initializeDeskInstancePositions();
	
	if (LoadGLTexture_UsingSOIL(&texture_ceiling, TEXTURE_DIR"Room\\ceiling.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED in Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_ceiling);
	}
	if (LoadGLTexture_UsingSOIL(&texture_floor, TEXTURE_DIR"Room\\floor.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED in floor Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_floor);
	}
	if (LoadGLTexture_UsingSOIL(&texture_back, TEXTURE_DIR"Room\\back.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED in backwall Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_back);
	}
	if (LoadGLTexture_UsingSOIL(&texture_side, TEXTURE_DIR"Room\\sidewall.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for sidewall Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_side);
	}

	if (LoadGLTexture_UsingSOIL(&texture_blackboard, TEXTURE_DIR"Roudra\\blackboard.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for blackboard Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_blackboard);
	}

	if (LoadGLTexture_UsingSOIL(&texture_failed, TEXTURE_DIR"Roudra\\failed.jpg") == FALSE) {
		uninitializeScene07_Raudra();
		LOG("LoadGLTexture FAILED for Marksheet Raudra!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_failed);
	}

	if (LoadGLTexture_UsingSOIL(&texture_door, TEXTURE_DIR"Roudra\\door.jpg") == FALSE) {
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

	// external debugging varaible
    tf_t = {2.0f, 0.0f, -3.35f}; // tree pos 
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

		LOG("Desk Instance %d Position: [%f %f %f]\n", i, 
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
	loadStaticModelInstanced("res/models/desk/desk.obj", &deskModel, NO_OF_INSTANCES_DESK, tmpPositions);
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

		LOG("Shelf Instance %d Position: [%f %f %f]\n", i, 
            instance_positions[(i*3)+0], instance_positions[(i*3)+1], instance_positions[(i*3)+2]);

        xPos += (X_INCREMENT_SHELF + X_SEPARTION_OFFSET_SHELF);
        //zPos += Z_INCREMENT + Z_SEPARTION_OFFSET;    
        
    }

	vector<float> tmpPositions {instance_positions, instance_positions + (NO_OF_INSTANCES_SHELF * 3)};
	loadStaticModelInstanced("res/models/shelf/Shelf.fbx", &shelfModel, NO_OF_INSTANCES_SHELF, tmpPositions);
}

void displayScene07_Raudra(void)
{
    // set camera
	setCamera();

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	
	viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
    #ifdef ENABLE_STATIC_MODELS
	//MODELS
	sceneIndoorADSUniform = useADSShader();

	glUniform1i(sceneIndoorADSUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneIndoorADSUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneIndoorADSUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneIndoorADSUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneIndoorADSUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneIndoorADSUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneIndoorADSUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneIndoorADSUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneIndoorADSUniform.materialShininessUniform, materialShininess);

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


	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();
	translationMatrix = vmath::translate(1.0f, 0.0f, 1.395f);
	scaleMatrix = vmath::scale(2.0f, 1.0f, 1.0f);
	modelMatrix = translationMatrix * scaleMatrix;
	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
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


	translationMatrix = vmath::translate(1.75f, -1.0f, -7.0f);
	//scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModelInstanced(deskModel, NO_OF_INSTANCES_DESK);

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


// ------ Globe Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
// 	translationMatrix = vmath::translate(1.5f, 0.0f, -3.35f);
// 	scaleMatrix = vmath::scale(0.075f, 0.1f, 0.075f);
// 	rotationMatrix_x = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
// 	rotationMatrix_y = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
// 	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

// 	drawStaticModel(shelfModel);

	// Un-use ShaderProgramObject
	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
    #endif 
}

void uninitializeScene07_Raudra(void)
{
    //UNINIT models
	unloadStaticModel(&deskModel);
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

