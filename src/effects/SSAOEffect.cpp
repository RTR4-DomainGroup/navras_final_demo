#include "../../inc/effects/SSAOEffect.h"

GLuint      ssao_points_buffer;
GLuint      ssao_quad_vao;


int initializeSSAO(void) {
	
	glGenVertexArrays(1, &ssao_quad_vao);
	glBindVertexArray(ssao_quad_vao);

	int i;
	SAMPLE_POINTS point_data;

	for (i = 0; i < 256; i++)
	{
		do
		{
			point_data.point[i][0] = random_float() * 2.0f - 1.0f;
			point_data.point[i][1] = random_float() * 2.0f - 1.0f;
			point_data.point[i][2] = random_float(); //  * 2.0f - 1.0f;
			point_data.point[i][3] = 0.0f;
		} while (length(point_data.point[i]) > 1.0f);
		normalize(point_data.point[i]);
	}
	for (i = 0; i < 256; i++)
	{
		point_data.random_vectors[i][0] = random_float();
		point_data.random_vectors[i][1] = random_float();
		point_data.random_vectors[i][2] = random_float();
		point_data.random_vectors[i][3] = random_float();
	}

	glGenBuffers(1, &ssao_points_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, ssao_points_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(SAMPLE_POINTS), &point_data, GL_STATIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(0);


	return 0;
}

void displaySSAO(struct SSAOFrameBufferStruct* frameBufferDetails) {

	// Code
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, ssao_points_buffer);

	useSSAOShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBufferDetails->fbo_textures[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, frameBufferDetails->fbo_textures[1]);

	glBindVertexArray(ssao_quad_vao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(0);

	glUseProgram(0);
	
	//glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

}

void uninitializeSSAO(void) {

	//uninitializeSSAOShader();
	
}


float random_float(void)
{
	float res;
	unsigned int tmp;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}
