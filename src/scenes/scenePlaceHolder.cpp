// This File Will Be Replaced by Scene*.cpp

#include "../../inc/scenes/scenePlaceHolder.h"
#include "../../inc/texture_loader.h"
#include "../../inc/Noise.h"

GLuint texture_Marble;

//struct ADSUniform sceneADSUniform;

struct CloudNoiseUniform sceneCloudNoiseUniform;

//GLfloat LightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat LightSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
//GLfloat LightPosition[] = { 0.0f, 0.0f, 100.0f, 1.0f };
//
//GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//GLfloat MaterialShininess = 128.0f;

// Cloud related light and color (for example)
GLfloat lightAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

GLfloat materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat materialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShininess = 128.0f;

GLfloat skyColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat cloudColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//GLfloat angleCube;

float myScale = 1.0f;

float noiseScale = 0.5f;
BOOL noiseScaleIncrement = TRUE;

GLuint noise_texture;

extern mat4 perspectiveProjectionMatrix;

extern FILE* gpFile;

//int initializeInterleaved(void)
//{
//
//    // Code.
//    // Texture
//	if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
//		//uninitialize();
//		fprintf(gpFile, "LoadGLTexture FAILED!!!\n");
//		return(-1);
//
//	}
//	else
//	{
//		fprintf(gpFile, "LoadGLTexture Successfull = %u!!!\n", texture_Marble);
//	}
//
//	//
//	//ZeroMemory(&sceneADSUniform, sizeof(struct ADSUniform));
//
//	return 0;
//
//}

//void displayInterleaved(void)
//{
//
//	// Code
//	sceneADSUniform = useADSShader();
//
//	// Here The Game STarts
//	// Triangle
//	// Transformations
//	mat4 translationMatrix = mat4::identity();
//	mat4 rotationMatrix = mat4::identity();
//	mat4 modelMatrix = mat4::identity();
//	mat4 viewMatrix = mat4::identity();
//
//	// Square
//	// Transformations
//	translationMatrix = mat4::identity();
//	mat4 scaleMatrix = mat4::identity();
//	rotationMatrix = mat4::identity();
//	mat4 rotationMatrix_x = mat4::identity();
//	mat4 rotationMatrix_y = mat4::identity();
//	mat4 rotationMatrix_z = mat4::identity();
//
//	translationMatrix = vmath::translate(0.0f, 0.0f, -6.0f);
//
//	//scaleMatrix = vmath::scale(0.75f, 0.75f, 0.75f);
//
//	rotationMatrix_x = vmath::rotate(angleCube, 1.0f, 0.0f, 0.0f);
//
//	rotationMatrix_y = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);
//
//	rotationMatrix_z = vmath::rotate(angleCube, 0.0f, 0.0f, 1.0f);
//
//	rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
//
//	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
//
//	glUniformMatrix4fv(sceneADSUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
//	glUniformMatrix4fv(sceneADSUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
//	glUniformMatrix4fv(sceneADSUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture_Marble);
//	
//    //glUniform1i(sceneADSUniform.textureSamplerUniform, 0);
//
//	// Sending Light Related Uniforms
//	glUniform1i(sceneADSUniform.lightingEnableUniform, 1);
//
//	glUniform4fv(sceneADSUniform.laUniform, 1, LightAmbient);
//	glUniform4fv(sceneADSUniform.ldUniform, 1, LightDiffuse);
//	glUniform4fv(sceneADSUniform.lsUniform, 1, LightSpecular);
//
//	glUniform4fv(sceneADSUniform.lightPositionUniform, 1, LightPosition);
//
//
//	glUniform4fv(sceneADSUniform.kaUniform, 1, MaterialAmbient);
//	glUniform4fv(sceneADSUniform.kdUniform, 1, MaterialDiffuse);
//	glUniform4fv(sceneADSUniform.ksUniform, 1, MaterialSpecular);
//
//	glUniform1f(sceneADSUniform.materialShininessUniform, MaterialShininess);
//
//
//	// Call Geometry over here 
	//displayCube();
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//
//	// Un-use ShaderProgramObject
//	glUseProgram(0);	
//
//}

//void updateInterleaved(void) 
//{
//
//	// Code
//	angleCube = angleCube + 1.0f;
//	if (angleCube >= 360.0f)
//	{
//		angleCube -= 360.0f;
//	}
//
//}

//void uninitializeInterleaved(void)
//{
//
//	// Code
//	if(texture_Marble)
//	{
//		glDeleteTextures(1, &texture_Marble);
//		texture_Marble = NULL;
//	}
//
//}

int initializeCloudNoise(void)
{
	// code

	glEnable(GL_TEXTURE_3D);

	//loadNoiseTexture();
	noise_texture = CreateNoise3D();

	return(0);
}

void displayCloudNoise(void)
{
	// code
	sceneCloudNoiseUniform = useCloudNoiseShader();

	mat4 translationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line.
	scaleMatrix = vmath::scale(1.777778f, 1.0f, 1.0f);
	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(sceneCloudNoiseUniform.modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneCloudNoiseUniform.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneCloudNoiseUniform.projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform3fv(sceneCloudNoiseUniform.laUniform, 1, lightAmbient);
	glUniform3fv(sceneCloudNoiseUniform.ldUniform, 1, lightDiffuse);
	glUniform3fv(sceneCloudNoiseUniform.lsUniform, 1, lightSpecular);
	glUniform4fv(sceneCloudNoiseUniform.lighPositionUniform, 1, lightPosition);

	glUniform3fv(sceneCloudNoiseUniform.kaUniform, 1, materialAmbient);
	glUniform3fv(sceneCloudNoiseUniform.kdUniform, 1, materialDiffuse);
	glUniform3fv(sceneCloudNoiseUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneCloudNoiseUniform.matrailShininessUniform, materialShininess);

	glUniform1f(sceneCloudNoiseUniform.scaleUniform, myScale);
	glUniform3fv(sceneCloudNoiseUniform.skyColorUniform, 1, skyColor);
	glUniform3fv(sceneCloudNoiseUniform.cloudColorUniform, 1, cloudColor);
	glUniform1f(sceneCloudNoiseUniform.noiseScaleUniform, noiseScale);

	//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noise_texture);
	glUniform1i(sceneCloudNoiseUniform.noiseSamplerUniform, 0);

	displayCloudQuad();

	// Unuse the shaderProgramObject
	glUseProgram(0);
}

void updateCloudNoise(void)
{
	// code
	if (noiseScaleIncrement)
	{
		noiseScale = noiseScale + 0.0001f;
		if (noiseScale > 0.5f)
		{
			noiseScaleIncrement = false;
		}
	}
	else
	{
		noiseScale = noiseScale - 0.0001f;
		if (noiseScale < 0.01f)
		{
			noiseScaleIncrement = true;
		}
	}
}

void uninitializeCloudNoise(void)
{
	// code
	uninitializeNoiseTexture();
	uninitializeGeometry();
}
