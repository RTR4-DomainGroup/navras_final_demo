// This File Will Be Replaced by Scene*.cpp

#include "../../inc/scenes/scenePlaceHolder.h"

#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/effects/SkyboxEffect.h"
#include "../../inc/effects/CloudEffect.h"
#include "../../inc/helper/common.h"
//#include "../../inc/Noise.h"

#include "../../inc/effects/ADSLights.h"
#include "../../inc/effects/Billboarding.h"

#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/common.h"

// #define ENABLE_CLOUD_NOISE
// #define ENABLE_TERRIAN
// #define ENABLE_SKYBOX
// #define ENABLE_CLOUD_NOISE
// #define ENABLE_TERRIAN
// #define ENABLE_SKYBOX
// #define ENABLE_STARFIELD
// #define ENABLE_ADSLIGHT
#define ENABLE_BILLBOARDING

// GLuint texture_Marble;
TEXTURE texture_grass;
TEXTURE texture_flower;

// struct ADSUniform sceneADSUniform;

struct TerrainUniform terrainUniform;

struct CloudNoiseUniform sceneCloudNoiseUniform;

struct TextureVariables terrainTextureVariables;

extern mat4 viewMatrix;

float myScale = 1.0f;
float noiseScale = 2.0f;
bool noiseScaleIncrement = true;


GLuint texture_Marble;

GLfloat LightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 100.0f, 1.0f };

GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialShininess = 50.0f;


GLfloat skyColor[] = { 0.0f, 0.0f, 0.8f, 0.0f };
GLfloat cloudColor[] = { 0.8f, 0.8f, 0.8f, 0.0f };

GLuint noise_texture;

extern mat4 perspectiveProjectionMatrix;


float displacementmap_depth;

GLfloat angleCube = 0.0f;

// Variables For Skybox
GLuint texture_skybox;
struct SkyboxUniform sceneSkyBoxUniform;

// Variables For Starfield
GLuint texture_star; 
double deltaTime;
struct StarfieldUniform sceneStarfieldUniform;

/// Steps
// 1. initialize(load) texture
// 2. initialize geometry
// 3. initialize geometry
int initializeScene_PlaceHolder(void)
{

    // Code.
#ifdef ENABLE_ADSLIGHT
    // Texture
	// if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
	if (LoadGLTexture_UsingSOIL(&texture_Marble, TEXTURE_DIR"marble.bmp") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}

	initializeADSLight();
#endif // ENABLE_ADSLIGHT

	// Code.
	// Texture
	//// if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
	// if (LoadGLTexture_UsingSOIL(&texture_Marble, TEXTURE_DIR"marble.bmp") == FALSE) {
	// 	//uninitialize();
	// 	LOG("LoadGLTexture FAILED!!!\n");
	// 	return(-1);
	// }
	// else
	// {
	// 	LOG("LoadGLTexture Marble Successfull = %u!!!\n", texture_Marble);
	// }

#ifdef ENABLE_TERRIAN
	displacementmap_depth = 15.0f;

	terrainTextureVariables.albedoPath = TEXTURE_DIR"terrain/DiffuseMapTerrain.jpg";
	terrainTextureVariables.displacementPath = TEXTURE_DIR"terrain/DisplacementMapTerrain.jpg";

	if (initializeTerrain(&terrainTextureVariables) != 0) 
	{

		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}

#endif

#ifdef ENABLE_SKYBOX
	if (initializeSkybox(&texture_skybox, TEXTURE_DIR"Skybox\\") != 0)
	{

		LOG("initializeSkybox() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("initializeSkybox() Successfull!!!\n");
	}
#endif

	// initialize Cloud Noise Shader

#ifdef ENABLE_CLOUD_NOISE
	noise_texture = initializeCloud();
	if (noise_texture == 0)
	{
		LOG("initializeCloud() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeCloud() Successfull!!!\n");
	}
#endif

    // initializeCube();
    // initializePyramid();
    // initializeQuad();
    // initializeTriangle();
     //initializeSphere();

#ifdef ENABLE_STARFIELD
	if (initializeStarfield(&texture_star, TEXTURE_DIR"Starfield/Star.png") != 0)
	{

		LOG("initializeStarfield() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeStarfield() Successfull!!!\n");
	}
#endif // ENABLE_STARFIELD
	
#ifdef ENABLE_BILLBOARDING	

	initializeBillboarding();	
	initializeQuad();

	// Load The Texture
	char imagefile[64] = {0};

	sprintf(imagefile, "%s", TEXTURE_DIR"\\billboarding\\grass.png");
	if (LoadGLTextureData_UsingSOIL(&texture_grass, imagefile) == GL_FALSE)
	{
        LOG("Texture loading failed for image %s\n", imagefile);
        return (-6);
    }

	sprintf(imagefile, "%s", TEXTURE_DIR"\\billboarding\\flower.png");
	if (LoadGLTextureData_UsingSOIL(&texture_flower, imagefile) == GL_FALSE)
	{
        LOG("Texture loading failed for image %s\n", imagefile);
        return (-6);
    }

#endif // ENABLE_BILLBOARDING

	return 0;
}

void displayScene_PlaceHolder(void)
{
	// Code
	// Here The Game STarts
	
	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();

	mat4 modelMatrix = mat4::identity();
	//mat4 viewMatrix = mat4::identity();
	
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
	// scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
	// rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
	// rotationMatrix_y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
	// rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
	// rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
	// modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

#ifdef ENABLE_ADSLIGHT
	struct ADSUniform sceneADSUniform;

    sceneADSUniform = useADSShader();
	glUniformMatrix4fv(sceneADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_Marble);
	
	// Sending texture Related Uniforms
    glUniform1i(sceneADSUniform.textureSamplerUniform, 0);

	// Sending Light Related Uniforms
	glUniform1i(sceneADSUniform.lightingEnableUniform, 1);
	glUniform4fv(sceneADSUniform.laUniform, 1, LightAmbient);
	glUniform4fv(sceneADSUniform.ldUniform, 1, LightDiffuse);
	glUniform4fv(sceneADSUniform.lsUniform, 1, LightSpecular);
	glUniform4fv(sceneADSUniform.lightPositionUniform, 1, LightPosition);
	glUniform4fv(sceneADSUniform.kaUniform, 1, MaterialAmbient);
	glUniform4fv(sceneADSUniform.kdUniform, 1, MaterialDiffuse);
	glUniform4fv(sceneADSUniform.ksUniform, 1, MaterialSpecular);
	glUniform1f(sceneADSUniform.materialShininessUniform, MaterialShininess);

	displayADSLight();
	// glBindTexture(GL_TEXTURE_2D, 0);

#endif // ENABLE_ADSLIGHT

	// Call Geometry over here 
	// displayCube();
	// displayTriangle();
    // displayQuad();
    // displayPyramid();
	// displaySphere();

#ifdef ENABLE_BILLBOARDING	

    BillboardingUniform billboardingUniform = useBillboardingShader();

    // Code

//////////////////////////////////////////
    // instanced quads with grass texture

    // translationMatrix = vmath::translate(0.0f, -5.0f, 0.0f);


	if(texture_grass.height > texture_grass.width)
		scaleMatrix = vmath::scale(texture_grass.width/(GLfloat)texture_grass.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_grass.height/(GLfloat)texture_grass.width, 1.0f);


    modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

    // send to shader
    glUniformMatrix4fv(
        billboardingUniform.modelMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        modelMatrix
    );
    glUniformMatrix4fv(
        billboardingUniform.viewMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        viewMatrix
    );
    glUniformMatrix4fv(
        billboardingUniform.projectionMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        perspectiveProjectionMatrix
    );


    glUniform1i(billboardingUniform.textureSamplerUniform, 0);

    // if(bBillboardingEnabled)
    glUniform1i(billboardingUniform.billboardingEnableUniform, 1);
    // else
    //     glUniform1i(billboardingUniform.billboardingEnableUniform, 0);


 
    glBindTexture(GL_TEXTURE_2D, texture_grass.id);

	displayBillboardingGrass();

    glBindTexture(GL_TEXTURE_2D, 0);

/// Flower

	if(texture_flower.height > texture_flower.width)
		scaleMatrix = vmath::scale(texture_flower.width/(GLfloat)texture_flower.height, 1.0f, 1.0f);
	else
		scaleMatrix = vmath::scale(1.0f, texture_flower.height/(GLfloat)texture_flower.width, 1.0f);

	translationMatrix = vmath::translate(1.5f, 0.0f, -6.0f);

    modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

    // send to shader
    glUniformMatrix4fv(
        billboardingUniform.modelMatrixUniform, // which uniform
        1, // 
        GL_FALSE,
        modelMatrix
    );

    glBindTexture(GL_TEXTURE_2D, texture_flower.id);

	displayBillboardingFlower();

    glBindTexture(GL_TEXTURE_2D, 0);

#endif // ENABLE_BILLBOARDING
	
#ifdef ENABLE_CLOUD_NOISE

	glEnable(GL_TEXTURE_3D);

	sceneCloudNoiseUniform = useCloudNoiseShader();

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 rotateX = mat4::identity();
	//mat4 viewMatrix = mat4::identity();

	//translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
	translationMatrix = vmath::translate(0.0f, 0.0f, -500.0f); // glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
	scaleMatrix = vmath::scale(800.0f, 450.0f, 1.0f);
	//rotateX = vmath::rotate(10.0f, 1.0f, 0.0f, 0.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotateX;

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
	glUniform3fv(sceneCloudNoiseUniform.skyColorUniform, 1, skyColor);
	glUniform3fv(sceneCloudNoiseUniform.cloudColorUniform, 1, cloudColor);
	glUniform1f(sceneCloudNoiseUniform.noiseScaleUniform, noiseScale);

	//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noise_texture);

	displayQuad();

	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);

#endif


#ifdef ENABLE_TERRIAN
	// Terrain

	terrainUniform = useTerrainShader();

	vmath::mat4 mv_matrix = viewMatrix * (translate(0.0f, -5.0f, -20.0f) * scale(1.0f, 1.0f, 1.0f));

	vmath::mat4 proj_matrix = perspectiveProjectionMatrix;

	glUniformMatrix4fv(terrainUniform.uniform_mv_matrix, 1, GL_FALSE, mv_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_proj_matrix, 1, GL_FALSE, proj_matrix);
	glUniformMatrix4fv(terrainUniform.uniform_mvp_matrix, 1, GL_FALSE, proj_matrix * mv_matrix);

	glUniform1f(terrainUniform.uniform_dmap_depth, displacementmap_depth);
	//glUniform1i(terrainUniform.uniform_enable_fog, enable_fog ? 1 : 0);
	glUniform1i(terrainUniform.uniform_enable_fog, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.displacement);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrainTextureVariables.albedo);


	displayTerrain();

	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

#endif


#ifdef ENABLE_SKYBOX

	sceneSkyBoxUniform = useSkyboxShader();

	// Transformations
	// mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	// mat4 scaleMatrix = mat4::identity();
	// mat4 modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -10.0f);					// glTranslatef() is replaced by this line.
	scaleMatrix = vmath::scale(30.0f, 30.0f, 30.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneSkyBoxUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneSkyBoxUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneSkyBoxUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

	displaySkybox(texture_skybox);
	glUseProgram(0);
#endif


#ifdef ENABLE_STARFIELD

	sceneStarfieldUniform = useStarfieldShader();

	float time = (float)deltaTime;

	time = time * 0.05f;
	time = time - floor(time);

	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);

	glUniform1f(sceneStarfieldUniform.timeUniform, time);

	displayStarfield(texture_star);
	glUseProgram(0);

#endif // ENABLE_STARFIELD
	
	// Un-use ShaderProgramObject
	glUseProgram(0);
}

void updateScene_PlaceHolder(void)
{

	// Code
#ifdef ENABLE_ADSLIGHT
    angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
	{
		angleCube -= 360.0f;
	}
	updateADSLight();
#endif // ENABLE_ADSLIGHT
	


#ifdef ENABLE_STARFIELD
	deltaTime = updateStarfield(deltaTime);
#endif // ENABLE_STARFIELD


#ifdef ENABLE_CLOUD_NOISE
	// update Cloud
	updateCloud(noiseScaleIncrement, noiseScale, 0.0001f);
#endif


#ifdef ENABLE_BILLBOARDING
	updateBillboarding();
#endif

}

void uninitializeScene_PlaceHolder(void)
{
	// Code

	// uninitializeTerrain();
    // uninitializeSphere();
    // uninitializeTriangle();
    // uninitializeQuad();
    // uninitializePyramid();
    uninitializeCube();

	
#ifdef ENABLE_BILLBOARDING
	uninitializeBillboarding();
	    // texture
    if(texture_grass.id)
    {
        glDeleteTextures(1, &texture_grass.id);
        texture_grass.id = 0;
    }
#endif

#ifdef ENABLE_STARFIELD
	uninitializeStarfield(texture_star);
#endif // ENABLE_STARFIELD

#ifdef ENABLE_SKYBOX
	uninitialiseSkybox(texture_skybox);
#endif // ENABLE_SKYBOX

#ifdef ENABLE_TERRIAN
	uninitializeTerrain(&terrainTextureVariables);
#endif

#ifdef ENABLE_CLOUD_NOISE

	
	uninitializeCloud();
	if (noise_texture)
	{
		glDeleteTextures(1, &noise_texture);
		noise_texture = 0;
	}
#endif

	// uninitializeSphere();
#ifdef ENABLE_ADSLIGHT
	if (texture_Marble)
	{
		glDeleteTextures(1, &texture_Marble);
		texture_Marble = NULL;
	}

#endif // ENABLE_ADSLIGHT

}
