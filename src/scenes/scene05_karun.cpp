#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/shaders/FSQuadShader.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../inc/shaders/BillboardingShader.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/GodraysEffect.h"
#include "../../inc/effects/GaussianBlurEffect.h"
#include "../../inc/scenes/scenePlaceHolderIndoor.h"
#include "../../inc/scenes/scene05_karun.h"
#include "../../inc/debug/debug_transformation.h"


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static struct ADSUniform sceneIndoorADSUniform;
GLuint texture_karunMask;

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

GLuint texture_withParent;
GLuint texture_onlyChild;
GLuint texture_kidroom_ceiling;
GLuint texture_kidroom_floor;
GLuint texture_kidroom_side;
GLuint texture_kidroom_back;

//blending variables
GLfloat blendingValue = 0.0f;

//Model variables
STATIC_MODEL roomModel;
STATIC_MODEL colorPencilModel;
STATIC_MODEL boyModel;

GLuint textures_kidroom[4];

//values
scene5_variables translatevalues;

bool isInitialDisplayScene05_karun = true;


int initializeScene5_karun(void)
{

#ifdef ENABLE_MASKSQUADS
	if (LoadGLTexture_UsingSOIL(&texture_karunMask, TEXTURE_DIR"Masks\\KarunMask.jpg") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in Mask Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull Mask Karun Ras = %u!!!\n", texture_karunMask);
	}


#endif // ENABLE_MASKSQUADS

#ifdef ENABLE_STATIC_MODELS
	//load models
	loadStaticModel("res/models/scene05-karunras/room/new/karunRoomNew1.obj", &roomModel); //done
	////loadStaticModel("res/models/scene05-karunras/colorpencil/pencil.fbx", &roomModel); 
	//loadStaticModel("res/models/scene05-karunras/crayons/crayons.obj", &roomModel); //done
	//loadStaticModel("res/models/scene05-karunras/colorpencil/pencil1obj.obj", &colorPencilModel); //done
	loadStaticModel("res/models/scene05-karunras/boy/tempKarunBoy1.obj", &boyModel);
	//loadStaticModel("res/models/scene05-karunras/woodenToy/woodenToy.obj", &roomModel); //done
	//loadStaticModel("res/models/kid-table/kidTable.obj", &roomModel); //done
	//loadStaticModel("res/models/scene05-karunras/Toytrain/toyTrain.obj", &roomModel); //change scale
	//loadStaticModel("res/models/kids-bicycle/bicycle_1.obj", &roomModel);

	if (LoadGLTexture_UsingSOIL(&texture_kidroom_ceiling, TEXTURE_DIR"Room/scene5Room/ceiling.png") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_ceiling);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_kidroom_floor, TEXTURE_DIR"Room/floor.jpg") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in floor Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_floor);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_kidroom_back, TEXTURE_DIR"Room/scene5Room/frontwall2.png") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in back Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_back);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_kidroom_side, TEXTURE_DIR"Room/scene5Room/sidewalls.png") == FALSE) 
	{
		//uninitialize();
		LOG("LoadGLTexture FAILED in sidewall Karun Ras!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_kidroom_side);
	}
	//loadStaticModel("res/models/schoolBag/schoolBag.fbx", &schoolBagModel);
#endif
	initializeInvertedNormalCube();
	initializeQuad();

	textures_kidroom[0] = (GLuint)texture_kidroom_ceiling;
	textures_kidroom[1] = (GLuint)texture_kidroom_floor;
	textures_kidroom[2] = (GLuint)texture_kidroom_back;
	textures_kidroom[3] = (GLuint)texture_kidroom_side;
//	glEnable(GL_TEXTURE_2D);

	initializeQuad();
	if (LoadGLTexture_UsingSOIL(&texture_withParent, TEXTURE_DIR"Scene5-karunRas\\withParents.png") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_withParent FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_withParent Successfull = %u!!!\n", texture_withParent);
	}
	if (LoadGLTexture_UsingSOIL(&texture_onlyChild, TEXTURE_DIR"Scene5-karunRas\\onlychild.png") == FALSE)
	{
		//uninitialize();
		LOG("LoadGLTexture for texture_onlyChild FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture texture_onlyChild Successfull = %u!!!\n", texture_onlyChild);
	}

	return 0;
}

void setCameraScene05_karun(void)
{
	if (isInitialDisplayScene05_karun == true)
	{
		//setCamera(6.750000, 0.000000, -1.500000, -309.215027, 0.000000, 184.353134, 0.000000, 1.000000, 0.000000);
		// lookAt(5.75f, -1.05f, -4.90f, -289.52f, -3.23f, 214.30f, 0.00f, 1.00f, 0.00f)
		setCamera(5.75f, -1.05f, -4.90f, -289.52f, -3.23f, 214.30f, 0.00f, 1.00f, 0.00f);
		isInitialDisplayScene05_karun = false;
	}
}

void displayScene5_karun(void)
{
    // set camera
	setCameraScene05_karun();

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	displayCamera();
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
	//displayRoom(textures_kidroom);
	glBindTexture(GL_TEXTURE_2D, 0);

#ifdef ENABLE_MASKSQUADS
	// Transformations For Mask Quad
	translationMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -5.0f);
	scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
	//rotationMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_karunMask);
	glUniform1i(sceneIndoorADSUniform.textureSamplerUniform_diffuse, 0);
		displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
#endif // ENABLE_MASKSQUADS

    // ------ Room Model ------
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

	//translatevalues.x = 1.650000 , translatevalues.y = -1.000000, translatevalues.z = -2.500001  
	translationMatrix = vmath::translate(1.650000f,-1.000000f, -2.500001f);
	//translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	//LOG(" translatevalues.x = %f , translatevalues.y = %f, translatevalues.z = %f \n", tf_t.x, tf_t.y, tf_t.z);
	//scaleMatrix = vmath::scale(tf_s.x, tf_s.y, tf_s.z);
	//LOG(" scale.x = %f , scale.y = %f, scale.z = %f \n", tf_s.x, tf_s.y, tf_s.z);
	//scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
	scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
	rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * scaleMatrix ;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(roomModel);


	//***********BOY********************
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();

    translationMatrix = vmath::translate(-0.37f, -2.26f, -1.27f);
	scaleMatrix = vmath::scale(1.40f, 1.40f, 1.40f);
	//scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
	rotationMatrix_x = vmath::rotate(4.80f, 1.0f, 0.0f, 0.0f); //rotatefX
	rotationMatrix_y = vmath::rotate(36.18f, 0.0f, 1.0f, 0.0f); //rotatefY
	rotationMatrix_z = vmath::rotate(-0.30f, 0.0f, 0.0f, 1.0f); //rotatefZ
	//TRANFORM speedVector = { 0.0f, 0.0f, 0.0f };
	//update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix,&speedVector);
	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(boyModel);

    //// ------ colorPencil Model ------
	//translationMatrix = mat4::identity();
	//rotationMatrix = mat4::identity();
	//modelMatrix = mat4::identity();
	//scaleMatrix = mat4::identity();
	//rotationMatrix_x = mat4::identity();
	//rotationMatrix_y = mat4::identity();
	//rotationMatrix_z = mat4::identity();
	//
	////translatevalues.x = 1.650000 , translatevalues.y = -1.000000, translatevalues.z = -2.500001  
	//translationMatrix = vmath::translate(1.650000f,-1.000000f, -2.500001f);
	////translationMatrix = vmath::translate(tf_t.x, tf_t.y, tf_t.z);
	////LOG(" translatevalues.x = %f , translatevalues.y = %f, translatevalues.z = %f \n", tf_t.x, tf_t.y, tf_t.z);
	////scaleMatrix = vmath::scale(tf_s.x, tf_s.y, tf_s.z);
	////LOG(" scale.x = %f , scale.y = %f, scale.z = %f \n", tf_s.x, tf_s.y, tf_s.z);
	////scaleMatrix = vmath::scale(1.0f, 1.0f, 1.0f);
	//scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
	//rotationMatrix = vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
	//TRANFORM speedVector = { 0.0f, 0.0f, 0.0f };
	//update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix,&speedVector);
	//modelMatrix = translationMatrix * scaleMatrix ;
	//
	//glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	//glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	//glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	//
	////drawStaticModel(colorPencilModel);


	//**************QUAD*********************
	translationMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	modelMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	rotationMatrix_x = mat4::identity();
	rotationMatrix_y = mat4::identity();
	rotationMatrix_z = mat4::identity();
 
	//translatevalues.x = 2.750000, translatevalues.y = 0.100000, translatevalues.z = 0.799998
	//translationMatrix = vmath::translate(1.650000f,-1.000000f, -2.500001f);
	translationMatrix = vmath::translate(0.13f, -1.73f, -1.04f);
	scaleMatrix = vmath::scale(0.54f, 0.54f, 0.54f);
	rotationMatrix = vmath::rotate(89.45f, 1.0f, 0.00f, 0.00f);
	//TRANFORM speedVector = { 0.0f, 0.0f, 0.0f };
	//update_transformations(&translationMatrix, NULL, &rotationMatrix,&speedVector);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	

	glUniform1f(sceneIndoorADSUniform.blendingUniform, blendingValue);
	glUniform1i(sceneIndoorADSUniform.uniform_enable_blending, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_withParent);
	glUniform1i(sceneIndoorADSUniform.textureSamplerUniform1, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_onlyChild);
	glUniform1i(sceneIndoorADSUniform.textureSamplerUniform2, 1);
	
	displayQuad();
	glBindTexture(GL_TEXTURE_2D, 0);

	glUniform1i(sceneIndoorADSUniform.uniform_enable_blending, 0);

	// Un-use ShaderProgramObject
	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
    #endif 
}

void updateScene5_karun(void)
{
	blendingValue += 0.005f;
	if (blendingValue >= 1.0f)
	{
		blendingValue = 0.0f;
	}

#ifdef ENABLE_CAMERA_ANIMATION
	// setCamera(6.70f, 0.10f, -1.95f, 6.00f, 0.03f, -1.24f, 0.00f, 1.00f, 0.00f);
	// lookAt   (4.10f, 0.50f, -0.40f, -287.50f, -106.39f, 216.66f, 0.00f, 1.00f, 0.00f)

	//cameraEyeX = preciselerp(cameraEyeX, 4.10f, 0.001f);
	//cameraEyeY = preciselerp(cameraEyeY, 0.50f, 0.001f);
	//cameraEyeZ = preciselerp(cameraEyeZ, -0.40f, 0.001);

	//cameraCenterX = preciselerp(cameraCenterX, -287.50f, 0.001f);
	//cameraCenterZ = preciselerp(cameraCenterZ, 216.66f, 0.001f);

	//if (cameraEyeX <= 5.00f)
	//	cameraCenterY = preciselerp(cameraCenterY, -106.39f, 0.001f);


	// lookAt   (3.45f, 0.60f, -0.05f, 2.89f, -0.03f, 0.49f, 0.00f, 1.00f, 0.00f)
	// lookAt   (3.55f, 0.70f, -0.00f, -260.29f, -270.46f, 237.59f, 0.00f, 1.00f, 0.00f)

	//cameraEyeX = preciselerp(cameraEyeX, 3.55f, 0.001f);
	//cameraEyeY = preciselerp(cameraEyeY, 0.70f, 0.001f);
	//cameraEyeZ = preciselerp(cameraEyeZ, 0.0f, 0.001);

	//cameraCenterX = preciselerp(cameraCenterX, -260.29f, 0.001f);
	//cameraCenterZ = preciselerp(cameraCenterZ, 237.59f, 0.001f);

	//if (cameraEyeX <= 5.00f)
	//	cameraCenterY = preciselerp(cameraCenterY, -270.46f, 0.001f);

	// lookAt(5.75f, -1.05f, -4.90f, -289.52f, -3.23f, 214.30f, 0.00f, 1.00f, 0.00f) (source)
	// lookAt(1.05f, -1.10f, -2.00f, 0.40f, -1.58f, -1.41f, 0.00f, 1.00f, 0.00f) (target)
	// lookAt(1.20f, -1.05f, -1.90f, 0.51f, -1.52f, -1.36f, 0.00f, 1.00f, 0.00f) (target2)

	cameraEyeX = preciselerp(cameraEyeX, 1.05f, 0.001f);
	cameraEyeY = preciselerp(cameraEyeY, -1.10f, 0.001f);
	cameraEyeZ = preciselerp(cameraEyeZ, -2.00f, 0.001);

	cameraCenterX = preciselerp(cameraCenterX, 0.40f, 0.08f);
	cameraCenterZ = preciselerp(cameraCenterZ, -1.41f, 0.08f);

	//if (cameraEyeX <= 4.8f)
		cameraCenterY = preciselerp(cameraCenterY, -1.58f, 0.08f);

#endif // ENABLE_CAMERA_ANIMATION

}


void uninitializeScene5_karun(void)
{
    //UNINIT models
	unloadStaticModel(&roomModel);
	unloadStaticModel(&colorPencilModel);
	unloadStaticModel(&boyModel);

#ifdef ENABLE_MASKSQUADS
	if (texture_karunMask)
	{
		glDeleteTextures(1, &texture_karunMask);
		texture_karunMask = 0;
	}
#endif

	if (texture_kidroom_ceiling)
	{
		glDeleteTextures(1, &texture_kidroom_ceiling);
		texture_kidroom_ceiling = 0;
	}
	if (texture_kidroom_floor)
	{
		glDeleteTextures(1, &texture_kidroom_floor);
		texture_kidroom_floor = 0;
	}
	if (texture_kidroom_back)
	{
		glDeleteTextures(1, &texture_kidroom_back);
		texture_kidroom_floor = 0;
	}
	if (texture_kidroom_side)
	{
		glDeleteTextures(1, &texture_kidroom_side);
		texture_kidroom_side = 0;
	}
	if (texture_withParent)
	{
		glDeleteTextures(1, &texture_withParent);
		texture_withParent = 0;
	}
	
}
