
// #include<windows.h>
#include<stdio.h>
#include <stdlib.h>
#include "../../inc/shaders/FontShader.h"

GLuint fontShaderProgramObject;

GLuint mvpMatrixUniform;
GLuint textureSamplerUniform;
GLuint textColorUniform;

extern mat4 perspectiveProjectionMatrix;

struct FontUniform fontShaderUniform;

int initializeFontShader(void)
{
    // Code
    // Vertex Shadder
	const GLchar* vertexShaderSourceCode =
		"#version 460 core \n" \
		"\n" \
		"in vec4 a_position; \n" \
		"in vec2 a_texcoord; \n" \
		"uniform mat4 u_mvpMatrix; \n" \
		"out vec2 a_texcoord_out; \n" \

		"void main(void) \n" \
		"{ \n" \
			"gl_Position = u_mvpMatrix * vec4(a_position.xy, 0.0, 1.0); \n" \
			"a_texcoord_out = a_position.zw; \n" \
		"} \n";

	// Create vertex shadder object
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// Give shadder code to shadder object
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);		
	// variables for shadder
	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				LOG("Vertex Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeFontShader();
			}
		}
	}

	// Fragment Shader
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core \n" \
		"\n" \
		"in vec2 a_texcoord_out; \n" \

		"uniform sampler2D u_textureSampler; \n" \
		"uniform vec4 textColor; \n" \

		"out vec4 FragColor; \n" \

		"void main(void) \n" \
		"{ \n" \
			"vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_textureSampler, a_texcoord_out).r); \n" \
			"FragColor = textColor * sampled; \n" \
		"} \n";

	// Create Fragment Shadder
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// Give shadder code to shadder object
	glShaderSource(fragmentShaderObject, 1, (const char**)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	// reinitialise variables for shadder
	status = 0;
	infoLogLength = 0;
	log = NULL;
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) 
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				LOG("Fragment Shader Conpilation Log: %s\n", log);
				free(log);
				uninitializeFontShader();
			}
		}
	}


	// Shader Program Object
	fontShaderProgramObject = glCreateProgram();

	glAttachShader(fontShaderProgramObject, vertexShaderObject);
	glAttachShader(fontShaderProgramObject, fragmentShaderObject);

	// pre link binding
	glBindAttribLocation(fontShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(fontShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

	// Link shader object
	glLinkProgram(fontShaderProgramObject);

	// reinitialise variables for shadder
	status = 0;
	infoLogLength = 0;
	log = NULL;

	glGetProgramiv(fontShaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetProgramiv(fontShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(fontShaderProgramObject, infoLogLength, &written, log);
				LOG("Shader Program Link Log: %s\n", log);
				free(log);
				uninitializeFontShader();
			}
		}
	}

	// Post Linkink
	fontShaderUniform.uniform_mvp_matrix = glGetUniformLocation(fontShaderProgramObject, "u_mvpMatrix");
	fontShaderUniform.textureSamplerUniform = glGetUniformLocation(fontShaderProgramObject, "u_textureSampler");
	fontShaderUniform.textColorUniform = glGetUniformLocation(fontShaderProgramObject, "textColor");

	LOG("Shader initialized\n");
	return 0;
}

struct FontUniform useFontShader(void)
{
    glUseProgram(fontShaderProgramObject);
    return fontShaderUniform;
}

void uninitializeFontShader(void)
{
    if (fontShaderProgramObject)
    {
        GLsizei numAttachedShader;
		glUseProgram(fontShaderProgramObject);
		glGetProgramiv(fontShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(fontShaderProgramObject, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(fontShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(fontShaderProgramObject);
		fontShaderProgramObject = 0;
    }
}
