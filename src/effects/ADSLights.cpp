#include "../../inc/helper/vmath.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/geometry.h"

#include "../../inc/shaders/ADSLightShader.h"

GLuint texture_Marble;
struct ADSUniform sceneADSUniform;


extern mat4 viewMatrix;

GLfloat LightAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 100.0f, 1.0f };

GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat MaterialShininess = 50.0f;

GLfloat angleCube;

extern mat4 perspectiveProjectionMatrix;

int initializeADSLight() {
    // Texture
	// if (LoadGLTexture(&texture_Marble, MAKEINTRESOURCE(IDBITMAP_MARBLE)) == FALSE) {
	if (LoadGLTexture(&texture_Marble, TEXTURE_DIR"marble.bmp") == FALSE) {
		//uninitialize();
		LOG("LoadGLTexture FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull = %u!!!\n", texture_Marble);
	}
}


void displayADSLight() {
    sceneADSUniform = useADSShader();

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





	// glBindTexture(GL_TEXTURE_2D, 0);


}

void updateADSLight()
{
    angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
	{
		angleCube -= 360.0f;
	}
}


void uninitializeADSLight()
{
	if(texture_Marble)
	{
		glDeleteTextures(1, &texture_Marble);
		texture_Marble = NULL;
	}
}