#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/constants.h"
#include "../../inc/shaders/ADSLightShader.h"
#include "../../inc/effects/StaticModelLoadingEffect.h"
#include "../../inc/effects/DynamicModelLoadingEffect.h"
#include "../../inc/scenes/scenePlaceHolderIndoor.h"
#include "../../inc/scenes/scene12_Hasya.h"
#include "../../inc/debug/debug_transformation.h"


#define FBO_WIDTH WIN_WIDTH
#define FBO_HEIGHT WIN_HEIGHT

extern int windowWidth;
extern int windowHeight;

//GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

static struct ADSUniform sceneIndoorADSUniform;
static struct ADSDynamicUniform sceneIndoorDynamicADSUniform;

extern GLfloat density;
extern GLfloat gradient;
extern GLfloat skyFogColor[];

static GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat lightPosition[] = { 10.0f, 10.0f, 10.0f, 1.0f };

static GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat materialShininess = 128.0f;

//Model variables
STATIC_MODEL hasya_roomModel;
STATIC_MODEL manBabyModel;
static DYNAMIC_MODEL hasyaManModel;
GLuint texture_hasyaMask;
bool isInitialDisplayScene12_hasya = true;

GLuint cameraHasyaUpdate = 1;

int initializeScene12_Hasya(void)
{

	initializeQuad();

#ifdef ENABLE_STATIC_MODELS
	// 	// function declarations

		//load models
	loadStaticModel("res/models/scene12_hasya/room/HasyaRoom7.obj", &hasya_roomModel);
	loadStaticModel("res/models/scene12_hasya/man/manWithBaby.obj", &manBabyModel);


#endif // ENABLE_STATIC_MODELS


#ifdef	ENABLE_DYNAMIC_MODELS
	//loadDynamicModel("res/models/scene07_raudra/raudraBoyAnim.fbx", &hasyaManModel);
	loadDynamicModel("res/models/scene12_hasya/man/manWithBabyAnim05.fbx", &hasyaManModel);
#endif


	//	glEnable(GL_TEXTURE_2D);
	return 0;
}

void setCameraScene12_Hasya(void)
{
	if (isInitialDisplayScene12_hasya == true)
	{
		//setCamera(6.750000, 0.000000, -1.500000, -309.215027, 0.000000, 184.353134, 0.000000, 1.000000, 0.000000);
		// lookAt(5.75f, -1.05f, -4.90f, -289.52f, -3.23f, 214.30f, 0.00f, 1.00f, 0.00f)
		// setCamera(1.75f, 0.45f, 1.90f, 1.75f, 0.45f, -4.10f, 0.00f, 1.00f, 0.00f);
		setCamera(1.75f, 0.60f, -2.95f, 1.75f, 0.60f, -8.95f, 0.00f, 1.00f, 0.00f);


		isInitialDisplayScene12_hasya = false;
	}
}

void displayScene12_Hasya(void)
{
	// set camera
	//setCameraScene12_Hasya();

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
	// viewMatrix = vmath::lookat(camera.eye, camera.center, camera.up);
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
	glUniform1i(sceneIndoorADSUniform.isInstanced, 0);

	glUniform1f(sceneIndoorADSUniform.blackOrWhiteRoomUniform, 1.0f);
	glUniform1f(sceneIndoorADSUniform.blackOrWhiteRoomMixDeltaUniform, 0.0f);
	glUniform1f(sceneIndoorADSUniform.ssaoIntensityDeltaUniform, 0.7f);

	glUniform1f(sceneIndoorADSUniform.colorCorrectionUniform, 0.4f);

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

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_hasyaMask);
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

	translationMatrix = vmath::translate(1.650000f, -1.000000f, -2.500001f);
	scaleMatrix = vmath::scale(0.025f, 0.025f, 0.025f);
	rotationMatrix = vmath::rotate(-90.0f, 0.0f, 1.0f, 0.0f);

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	drawStaticModel(hasya_roomModel);

	//man


	//translationMatrix = mat4::identity();
	//rotationMatrix = mat4::identity();
	//modelMatrix = mat4::identity();
	//scaleMatrix = mat4::identity();
	//rotationMatrix_x = mat4::identity();
	//rotationMatrix_y = mat4::identity();
	//rotationMatrix_z = mat4::identity();


	//translationMatrix = vmath::translate(0.69f, -0.95f, -3.33f);
	//scaleMatrix = vmath::scale(0.87f, 0.87f, 0.87f);
	//rotationMatrix = vmath::rotate(41.16f, 0.0f, 1.0f, 0.0f);
	////update_transformations(&translationMatrix, &scaleMatrix, &rotationMatrix);

	//modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	//glUniformMatrix4fv(sceneIndoorADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	//glUniformMatrix4fv(sceneIndoorADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	//glUniformMatrix4fv(sceneIndoorADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	//drawStaticModel(manBabyModel);

	glUseProgram(0);

#endif // ENABLE_STATIC_MODELS

#ifdef ENABLE_DYNAMIC_MODELS
	glm::mat4 glm_modelMatrix;
	glm::mat4 glm_translateMatrix;
	glm::mat4 glm_rotateMatrix;
	glm::mat4 glm_scaleMatrix;

	glm_modelMatrix = glm::mat4(1.0f);
	/*glm_translateMatrix = glm::mat4(1.0f);
	glm_rotateMatrix = glm::mat4(1.0f);
	glm_scaleMatrix = glm::mat4(1.0f);*/

	sceneIndoorDynamicADSUniform = useADSDynamicShader();
	glm_translateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.69f, -0.95f, -3.33f));
	glm_scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.0078f, 0.0078f, 0.0078f));

	glm_rotateMatrix = glm::rotate(glm::mat4(1.0f), (41.16f * 3.1415f / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//update_transformations_glm(&glm_translateMatrix, &glm_scaleMatrix, NULL, NULL);
	glm_modelMatrix = glm_translateMatrix * glm_scaleMatrix * glm_rotateMatrix;

	glUniform4fv(sceneIndoorDynamicADSUniform.laUniform, 1, lightAmbient);
	glUniform4fv(sceneIndoorDynamicADSUniform.ldUniform, 1, lightDiffuse);
	glUniform4fv(sceneIndoorDynamicADSUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneIndoorDynamicADSUniform.lightPositionUniform, 1, lightPosition);
	glUniform4fv(sceneIndoorDynamicADSUniform.kaUniform, 1, materialAmbient);
	glUniform4fv(sceneIndoorDynamicADSUniform.kdUniform, 1, materialDiffuse);
	glUniform4fv(sceneIndoorDynamicADSUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneIndoorDynamicADSUniform.materialShininessUniform, materialShininess);

	glUniform1i(sceneIndoorDynamicADSUniform.fogEnableUniform, 0);
	glUniform1f(sceneIndoorDynamicADSUniform.densityUniform, density);
	glUniform1f(sceneIndoorDynamicADSUniform.gradientUniform, gradient);
	glUniform4fv(sceneIndoorDynamicADSUniform.skyFogColorUniform, 1, skyFogColor);
	glUniform1i(sceneIndoorDynamicADSUniform.uniform_enable_godRays, 1);
	glUniform1i(sceneIndoorDynamicADSUniform.godrays_blackpass_sphere, 0);
	glUniform1i(sceneIndoorDynamicADSUniform.actualSceneUniform, 1);
	glUniform1i(sceneIndoorDynamicADSUniform.depthSceneUniform, 0);
	glUniform1i(sceneIndoorDynamicADSUniform.depthQuadSceneUniform, 0);

	glUniform1f(sceneIndoorDynamicADSUniform.colorCorrectionUniform, 0.1f);

	glUniformMatrix4fv(sceneIndoorDynamicADSUniform.modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(glm_modelMatrix));
	glUniformMatrix4fv(sceneIndoorDynamicADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneIndoorDynamicADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	static bool replay_animation = true;
	if (replay_animation) // replay animation
	{
		LOG("Replaying model animation\n");
		reDrawDynamicModel(sceneIndoorDynamicADSUniform, hasyaManModel, 1.0f);
		replay_animation = false;
	}
	else
		drawDynamicModel(sceneIndoorDynamicADSUniform, hasyaManModel, 1.0f);
	glUseProgram(0);
	//glDisable(GL_TEXTURE_2D);
#endif 


}

void updateScene12_Hasya()
{
#ifdef ENABLE_CAMERA_ANIMATION

	if (cameraHasyaUpdate == 1)
	{


		cameraEyeX = preciselerp(cameraEyeX, 1.75f, 0.005f);
		cameraEyeY = preciselerp(cameraEyeY, 0.05f, 0.005f);
		cameraEyeZ = preciselerp(cameraEyeZ, 1.30f, 0.005f);

		cameraCenterX = preciselerp(cameraCenterX, 1.75f, 0.005f);
		cameraCenterY = preciselerp(cameraCenterY, 0.05f, 0.005f);
		cameraCenterZ = preciselerp(cameraCenterZ, -4.70f, 0.005f);

		if (cameraEyeZ >= 1.0f)
		{
			cameraHasyaUpdate = 2;
		}
	}
	if (cameraHasyaUpdate == 2)
	{

		cameraEyeX = preciselerp(cameraEyeX, 0.65f, 0.005f);
		cameraEyeY = preciselerp(cameraEyeY, -0.25f, 0.005f);
		cameraEyeZ = preciselerp(cameraEyeZ, -1.05f, 0.005f);


		cameraCenterX = preciselerp(cameraCenterX, 0.0f, 0.005f);
		cameraCenterY = preciselerp(cameraCenterY, -0.25f, 0.005f);
		cameraCenterZ = preciselerp(cameraCenterZ, -7.05f, 0.005f);
	}



#endif // ENABLE_CAMERA_ANIMATION
}

void uninitializeScene12_Hasya(void)
{
	//UNINIT models
	unloadStaticModel(&hasya_roomModel);
	unloadStaticModel(&manBabyModel);

#ifdef ENABLE_MASKSQUADS
	if (texture_hasyaMask)
	{
		glDeleteTextures(1, &texture_hasyaMask);
		texture_hasyaMask = 0;
	}
#endif


}

