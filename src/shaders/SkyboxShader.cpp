#include "../../inc/shaders/SkyboxShader.h"

GLuint shaderProgramObj_skybox;
extern FILE* gpFile;

struct SkyboxUniform skyboxUniform;

int initializeSkyboxShader(void)
{
    // Code
	// ***** Vertex Shader ***** //	
	// ***** Writing Shader Code ***** //
	const GLchar* vertexShaderSrcCode =
		"#version 460 core \n"										                            \
		"\n"														                            \

		"in vec4 a_position; \n"									                            \
		"uniform mat4 u_modelMatrix; \n"							                            \
		"uniform mat4 u_viewMatrix; \n"								                            \
		"uniform mat4 u_projectionMatrix; \n"						                            \

		"out vec3 a_texcoord_out; \n"								                            \

		"void main(void) \n"										                            \
		"{ \n"														                            \
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position; \n"	\
			"a_texcoord_out = vec3(a_position); \n"					                            \
		"} \n";

    // Create the Vertex Shader object.
    GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);

    // Give the shader source to shader object.
    // Actually 3rd parameter is array if you have multiple shader source code
    // However, we have only one source code string
    glShaderSource(vertexShaderObj, 1, (const GLchar **)&vertexShaderSrcCode, NULL);

    // Compile the Shader source code for GPU format
    glCompileShader(vertexShaderObj);

    GLint status;
    GLint infoLogLength;
    char* log = NULL;

    glGetShaderiv(vertexShaderObj, GL_COMPILE_STATUS, &status);

    #pragma region 
    // If there is an error
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShaderObj, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShaderObj, infoLogLength, &written, log);
                LOG("Skybox Shader Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeSkyboxShader();
                return(-1);
            }
        }
    }
    #pragma endregion

	// ***** Writing Shader Code ***** //
	const GLchar* fragmentShaderSrcCode =
		"#version 460 core \n"												\
		"\n"																\

		"in vec3 a_texcoord_out; \n"										\
		"uniform samplerCube u_textureSampler; \n"							\
		"out vec4 FragColor; \n"											\

		"void main(void) \n"												\
		"{ \n"																\
			"FragColor = texture(u_textureSampler, a_texcoord_out); \n"		\
		"}";
    
     // Create the Fragment Shader object.
    GLuint fragementShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

    // Give the shader source to shader object.
    // Actually 3rd parameter is array if you have multiple shader source code
    // However, we have only one source code string
    glShaderSource(fragementShaderObj, 1, (const GLchar **)&fragmentShaderSrcCode, NULL);

    // Compile the Shader source code for GPU format
    glCompileShader(fragementShaderObj);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(fragementShaderObj, GL_COMPILE_STATUS, &status);

    // If there is an error
    if (status == GL_FALSE)
    {
        glGetShaderiv(fragementShaderObj, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(fragementShaderObj, infoLogLength, &written, log);
                LOG("Fragment Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeSkyboxShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    shaderProgramObj_skybox = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgramObj_skybox, vertexShaderObj);
    glAttachShader(shaderProgramObj_skybox, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(shaderProgramObj_skybox, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    glBindAttribLocation(shaderProgramObj_skybox, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");
     
    // Link the program
    glLinkProgram(shaderProgramObj_skybox);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObj_skybox, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObj_skybox, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgramObj_skybox, infoLogLength, &written, log);
                LOG("Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeSkyboxShader();
                return(-1);
            }
        }
    }

    skyboxUniform.modelMatrix = glGetUniformLocation(shaderProgramObj_skybox, "u_modelMatrix");
    skyboxUniform.viewMatrix = glGetUniformLocation(shaderProgramObj_skybox, "u_viewMatrix");
    skyboxUniform.projectionMatrix = glGetUniformLocation(shaderProgramObj_skybox, "u_projectionMatrix");
    skyboxUniform.textureSamplerUniform = glGetUniformLocation(shaderProgramObj_skybox, "u_textureSampler");

    glUseProgram(shaderProgramObj_skybox);
    glUniform1i(skyboxUniform.textureSamplerUniform, 0);
    glUseProgram(0);

    return (0);
}

struct SkyboxUniform useSkyboxShader(void)
{
    glUseProgram(shaderProgramObj_skybox);
    return skyboxUniform;
}

void uninitializeSkyboxShader(void)
{
    if (shaderProgramObj_skybox)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_skybox);
		glGetProgramiv(shaderProgramObj_skybox, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_skybox, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_skybox, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_skybox);
		shaderProgramObj_skybox = 0;
    }
}