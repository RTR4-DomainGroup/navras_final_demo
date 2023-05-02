
#include "../../inc/shaders/BillboardingShader.h"

// Variable Declarations
GLuint billboardingShaderProgramObject;
BillboardingUniform billboardingUniform;

int initializeBillboardingShader(void)
{

	// Function Declarations
	void uninitializeBillboardingShader(void);

    // Code
    ZeroMemory((void*)&billboardingUniform, sizeof(BillboardingUniform));

	// Vertex Shader
	const GLchar* vertexShaderSourcecode =
    "#version 460 core \n" \
    "\n" \
    "in vec4 a_position; \n" \
    "in vec4 a_instancePosition; \n" \
    "in vec2 a_texcoord; \n" \

    "uniform mat4 u_modelMatrix; \n" \
    "uniform mat4 u_viewMatrix; \n" \
    "uniform mat4 u_projectionMatrix; \n" \
    "uniform int u_billboarding; \n" \
    "out vec2 a_texcoord_out; \n" \

    "void main(void) \n" \
    "{\n" \
    "    vec4 pos = (a_position + a_instancePosition); \n" \
    "    if(1 == u_billboarding) \n" \
    "    {\n" \
    "        vec3 right = vec3(u_viewMatrix[0][0], u_viewMatrix[1][0], u_viewMatrix[2][0]); \n" \
    "        vec3 up = vec3(u_viewMatrix[0][1], u_viewMatrix[1][1], u_viewMatrix[2][1]); \n" \
    "        pos = vec4((right * pos.x) + (up * pos.y), 1.0); \n" \
    "    }\n" \
    "    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * pos; \n" \
    "    a_texcoord_out = a_texcoord; \n" \
    "}\n";

	GLuint vertexShadderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShadderObject, 1, (const GLchar**)&vertexShaderSourcecode, NULL);

	glCompileShader(vertexShadderObject);

	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	glGetShaderiv(vertexShadderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {

		glGetShaderiv(vertexShadderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(vertexShadderObject, infoLogLength, &written, log);
				LOG("ADS Vertex Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeBillboardingShader();
			}
		}
	}


	// Fragment Shader
	const GLchar* fragmentShaderSourcecode =
    "#version 460 core \n" \
    "\n" \
    "in vec2 a_texcoord_out; \n" \
    // bind texture chi image 
    "uniform sampler2D u_textureSampler; \n" \
    "out vec4 FragColor; \n" \
    "void main(void) \n" \
    "{\n" \
    "   vec4 tex = texture(u_textureSampler, a_texcoord_out); \n" \
    "   FragColor = tex; \n" \
    "}\n";

	GLuint fragmentShadderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShadderObject, 1, (const GLchar**)&fragmentShaderSourcecode, NULL);

	glCompileShader(fragmentShadderObject);

	glGetShaderiv(fragmentShadderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {

		glGetShaderiv(fragmentShadderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(fragmentShadderObject, infoLogLength, &written, log);
				LOG("ADS Fragment Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeBillboardingShader();
			}
		}
	}

	// Shader Program Object
	billboardingShaderProgramObject = glCreateProgram();
	glAttachShader(billboardingShaderProgramObject, vertexShadderObject);
	glAttachShader(billboardingShaderProgramObject, fragmentShadderObject);
	glBindAttribLocation(billboardingShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(billboardingShaderProgramObject, DOMAIN_ATTRIBUTE_INSTANCE_POSITION, "a_instancePosition");
	glBindAttribLocation(billboardingShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

	glLinkProgram(billboardingShaderProgramObject);
	glGetProgramiv(billboardingShaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
	
		glGetShaderiv(billboardingShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(billboardingShaderProgramObject, infoLogLength, &written, log);
				LOG("ADS ShaderProgram Linking Log: %s\n", log);
				free(log);
				uninitializeBillboardingShader();
			}
		}
	}

	//

	billboardingUniform.modelMatrixUniform = glGetUniformLocation(billboardingShaderProgramObject, "u_modelMatrix");
	billboardingUniform.viewMatrixUniform = glGetUniformLocation(billboardingShaderProgramObject, "u_viewMatrix");
	billboardingUniform.projectionMatrixUniform = glGetUniformLocation(billboardingShaderProgramObject, "u_projectionMatrix");
	billboardingUniform.textureSamplerUniform = glGetUniformLocation(billboardingShaderProgramObject, "u_textureSampler");
	billboardingUniform.billboardingEnableUniform = glGetUniformLocation(billboardingShaderProgramObject, "u_billboarding");
    
	return(0);

}

struct BillboardingUniform useBillboardingShader(void)
{
    // Code
    glUseProgram(billboardingShaderProgramObject);
    return billboardingUniform;

}

void uninitializeBillboardingShader(void)
{

    // Code
    // Shader Uninitialization
	if (billboardingShaderProgramObject) {

		GLsizei numAttachedShader;
		glUseProgram(billboardingShaderProgramObject);
		glGetProgramiv(billboardingShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(billboardingShaderProgramObject, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(billboardingShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(billboardingShaderProgramObject);
		billboardingShaderProgramObject = 0;
	}
}

