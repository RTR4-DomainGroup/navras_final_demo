// This File Will Be Replaced by Scene*.cpp

#include "../../inc/scenes/scenePlaceHolder.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/effects/TerrainEffect.h"
#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/helper/camera.h"

GLuint texture_Marble;
GLuint texture_albedo;
GLuint texture_normal;
GLuint texture_metallic;
GLuint texture_roughness;
GLuint texture_ao;

struct ADSUniform sceneADSUniform;
struct PBRGeometryUniform scenePBRUniform;
struct Camera camera;
struct TerrainUniform terrainUniform1;

extern mat4 viewMatrix;

GLfloat LightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 100.0f, 1.0f };
GLfloat LightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialShininess = 50.0f;

GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

extern FILE* gpFile;

int initializeScene_PlaceHolder(void)
{

    // Code.
    // Texture
	//if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
	//	//uninitialize();
	//	LOG("LoadGLTexture FAILED!!!\n");
	//	return(-1);
	//
	//}
	//else
	//{
	//	LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	//}
	if (LoadGLTexture_UsingSOIL(&texture_albedo,"res/textures/pbr/albedo.png") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_normal,"res/textures/pbr/albedo.png") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_metallic,"res/textures/pbr/albedo.png") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_roughness,"res/textures/pbr/albedo.png") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}
	
	if (LoadGLTexture_UsingSOIL(&texture_ao,"res/textures/pbr/albedo.png") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}

	//initializeTerrainShader();

	/*if (initializeTerrain() != 0) {
	
		LOG("initializeTerrain() FAILED!!!\n");
		return(-1);

	}
	else
	{
		LOG("initializeTerrain() Successfull!!!\n");
	}*/

    // initializeCube();
    // initializePyramid();
    // initializeQuad();
    // initializeTriangle();
     initializeSphere();

	
	if (initializeStarfield() != 0) 
	{

		LOG("initializeScene_Starfield() FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("initializeScene_Starfield() Successfull!!!\n");
	}
	

	//
	//ZeroMemory(&sceneADSUniform, sizeof(struct ADSUniform));

	return 0;

}

void displayScene_PlaceHolder(void)
{

	// Code
	//sceneADSUniform = useADSShader();
	scenePBRUniform = usePBRGeometryShader();

	// Here The Game STarts
	// Triangle
	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	//mat4 viewMatrix = mat4::identity();

	// Square
	// Transformations
	translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	rotationMatrix = mat4::identity();
	mat4 rotationMatrix_x = mat4::identity();
	mat4 rotationMatrix_y = mat4::identity();
	mat4 rotationMatrix_z = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);

	scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);

	rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);

	rotationMatrix_y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);

	rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);

	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;

	glUniformMatrix4fv(scenePBRUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(scenePBRUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(scenePBRUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture_Marble);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_albedo);
	glUniform1i(scenePBRUniform.textureSamplerUniform_albedo, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture_normal);
	glUniform1i(scenePBRUniform.textureSamplerUniform_normal, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, texture_metallic);
	glUniform1i(scenePBRUniform.textureSamplerUniform_metallic, 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, texture_roughness);
	glUniform1i(scenePBRUniform.textureSamplerUniform_roughness, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture_ao);
	glUniform1i(scenePBRUniform.textureSamplerUniform_ao, 4);

    //glUniform1i(sceneADSUniform.textureSamplerUniform, 0);

	// Sending Light Related Uniforms
	glUniform1i(scenePBRUniform.lightingEnableUniform, 1);

	//glUniform4fv(scenePBRUniform.laUniform, 1, LightAmbient);
	//glUniform4fv(scenePBRUniform.ldUniform, 1, LightDiffuse);
	//glUniform4fv(scenePBRUniform.lsUniform, 1, LightSpecular);

	glUniform4fv(scenePBRUniform.lightPositionUniform, 1, LightPosition);
	glUniform3fv(scenePBRUniform.cameraPositionUniform, 1, camera.eye);
	glUniform4fv(scenePBRUniform.lightColorUniform, 1, LightColor);


	//glUniform4fv(scenePBRUniform.kaUniform, 1, MaterialAmbient);
	//glUniform4fv(scenePBRUniform.kdUniform, 1, MaterialDiffuse);
	//glUniform4fv(scenePBRUniform.ksUniform, 1, MaterialSpecular);
	//
	//glUniform1f(scenePBRUniform.materialShininessUniform, MaterialShininess);


	// Call Geometry over here 
	// displayCube();
	// displayTriangle();
    // displayQuad();
    // displayPyramid();
	displaySphere();


	// glBindTexture(GL_TEXTURE_2D, 0);

	// Un-use ShaderProgramObject
	glUseProgram(0);	

	displayTerrain();
	
	


}

void updateScene_PlaceHolder(void)
{

	// Code

	updateStarfield();

	angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
	{
		angleCube -= 360.0f;
	}

}

void uninitializeScene_PlaceHolder(void)
{

	// Code
	uninitializeStarfield();
	//uninitializeTerrain();
    //uninitializeSphere();
    // uninitializeTriangle();
    // uninitializeQuad();
    // uninitializePyramid();
    // uninitializeCube();

	if(texture_Marble)
	{
		glDeleteTextures(1, &texture_Marble);
		texture_Marble = NULL;
	}

	
}

