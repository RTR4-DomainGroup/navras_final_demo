#include "../../inc/effects/CloudEffect.h"
#include "../../inc/Noise.h"
#include "../../inc/helper/geometry.h"
#include "../../inc/helper/camera.h"

extern mat4 perspectiveProjectionMatrix;

struct CloudNoiseUniform sceneCloudNoiseUniform;

float myScale = 1.0f;

float noiseScale = 0.5f;
bool noiseScaleIncrement = true;

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

GLuint noise_texture;

int initializeCloud(void)
{
	// function declarations
	GLuint CreateNoise3D(void);

	// code
	initializeQuad();

	// BLENDING
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//loadNoiseTexture();
	noise_texture = CreateNoise3D();

	return(0);
}

void displayCloud(void)
{
	// code
	glEnable(GL_TEXTURE_3D);

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
	glUniform4fv(sceneCloudNoiseUniform.lightPositionUniform, 1, lightPosition);

	glUniform3fv(sceneCloudNoiseUniform.kaUniform, 1, materialAmbient);
	glUniform3fv(sceneCloudNoiseUniform.kdUniform, 1, materialDiffuse);
	glUniform3fv(sceneCloudNoiseUniform.ksUniform, 1, materialSpecular);
	glUniform1f(sceneCloudNoiseUniform.materialShininessUniform, materialShininess);

	glUniform1f(sceneCloudNoiseUniform.scaleUniform, myScale);
	//glUniform3fv(sceneCloudNoiseUniform.skyColorUniform, 1, skyColor);
	glUniform3fv(sceneCloudNoiseUniform.cloudColorUniform, 1, cloudColor);
	glUniform1f(sceneCloudNoiseUniform.noiseScaleUniform, noiseScale);

	//glUniform1f(sceneCloudNoiseUniform.alphaBlendingUniform, alphaBlending);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noise_texture);

	displayQuad();

	glUseProgram(0);

	glDisable(GL_TEXTURE_3D);
}

void updateCloud(void)
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

void uninitializeCloud(void)
{
	// code
	uninitializeQuad();
	uninitializeNoiseTexture();
	if (noise_texture)
	{
		glDeleteTextures(1, &noise_texture);
		noise_texture = 0;
	}
}
