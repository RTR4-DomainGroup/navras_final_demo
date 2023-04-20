#include "../../inc/effects/StarfieldEffect.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"

#define NUM_STARS 2000

struct StarfieldUniform sceneStarfieldUniform;

GLuint texture_star;

GLuint star_vao;						// Vertex Array Object
GLuint star_buffer;						// Vertex Buffer Object

extern mat4 viewMatrix;
extern mat4 perspectiveProjectionMatrix;

double currentTime;
static unsigned int seed = 0x13371337;

static inline float random_float()
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

int initializeScene_Starfield() 
{
	
	if (LoadGLTexture_UsingSOIL(&texture_star, "res/textures/Starfield/Star.png") == FALSE)
	{
		LOG("LoadGLTexture texture_star For Starfield FAILED!!!\n");
		return(-1);
	}
	else
	{
		LOG("LoadGLTexture Successfull for Starfield = %u!!!\n", texture_star);
	}

	glGenVertexArrays(1, &star_vao);
	glBindVertexArray(star_vao);

	struct star_t
	{
		vmath::vec3     position;
		vmath::vec3     color;
	};

	glGenBuffers(1, &star_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, star_buffer);
	glBufferData(GL_ARRAY_BUFFER, NUM_STARS * sizeof(star_t), NULL, GL_STATIC_DRAW);

	star_t* star = (star_t*)glMapBufferRange(GL_ARRAY_BUFFER, 0, NUM_STARS * sizeof(star_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	int i;

	for (i = 0; i < 1000; i++)
	{
		star[i].position[0] = (random_float() * 2.0f - 1.0f) * 200.0f;
		star[i].position[1] = (random_float() * 2.0f - 1.0f) * 200.0f;
		star[i].position[2] = random_float();
		star[i].color[0] = 0.8f + random_float() * 0.2f;
		star[i].color[1] = 0.8f + random_float() * 0.2f;
		star[i].color[2] = 0.8f + random_float() * 0.2f;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(star_t), (void*)sizeof(vmath::vec3));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glEnable(GL_POINT_SPRITE);

	return 0;

}

void displayScene_Starfield()
{

	// Code
	sceneStarfieldUniform = useStarfieldShader();

	float time = (float)currentTime;

	time = time * 0.05f;
	time = time - floor(time);

	// Transformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -3.0f);					// glTranslatef() is replaced by this line.
	//scaleMatrix = vmath::scale(12.0f, 12.0f, 12.0f);
	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;				// ORDER IS VERY IMPORTANT

	glUniformMatrix4fv(sceneStarfieldUniform.modelMatrix, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.viewMatrix, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sceneStarfieldUniform.projectionMatrix, 1, GL_FALSE, perspectiveProjectionMatrix);

	glUniform1f(sceneStarfieldUniform.timeUniform, time);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_star);
	glUniform1i(sceneStarfieldUniform.textureSamplerUniform, 0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindVertexArray(star_vao);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glDrawArrays(GL_POINTS, 0, NUM_STARS);
	glBindVertexArray(0);

	glUseProgram(0);

}

void updateScene_Starfield(void)
{
	// Code
	currentTime = currentTime + 0.05;

}

void uninitializeScene_Starfield() {

	if (texture_star)
	{
		glDeleteTextures(1, &texture_star);
		texture_star = NULL;
	}

	// Delete/Unintilization of Vertex Buffer Object
	if (star_buffer)
	{
		glDeleteBuffers(1, &star_buffer);
		star_buffer = 0;
	}

	// Delete/Unintilization of Vertex Array Object
	if (star_vao)
	{
		glDeleteVertexArrays(1, &star_vao);
		star_vao = 0;
	}

}
