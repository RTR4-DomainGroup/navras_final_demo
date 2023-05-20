#include "../../inc/shaders/StarfieldShader.h"

GLuint shaderProgramObj_starfield;

struct StarfieldUniform starfieldUniform;


int initializeStarfieldShader(void)
{
    // Code
	// ***** Vertex Shader ***** //	
	// ***** Writing Shader Code ***** //
	const GLchar* vertexShaderSourceCode =
		"#version 460 core \n"																	\
		"\n"																					\

		"in vec4 a_position; \n"																\
		"in vec4 a_color; \n"																	\
		"in vec2 a_texcoord; \n"																\

		"uniform float u_time; \n"																\

		"uniform mat4 u_modelMatrix; \n"														\
		"uniform mat4 u_viewMatrix; \n"															\
		"uniform mat4 u_projectionMatrix; \n"													\

		"out vec4 a_color_out; \n"																\
		"out vec2 a_texcoord_out; \n"															\

		"void main(void) \n"																	\
		"{ \n"																					\
			"vec4 newVertex = a_position; \n"													\
			"newVertex.z = newVertex.z +  u_time;\n"											\
			"newVertex.z = fract(newVertex.z); \n"												\
			"float size = (20.0 * newVertex.z * newVertex.z); \n"								\
			"a_color_out = smoothstep(4.0, 7.0, size) * a_color; \n"							\
			"newVertex.z = (999.9 * newVertex.z) - 1000.0; \n"									\

			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * newVertex; \n"	\
			"a_texcoord_out = a_texcoord; \n"													\
			"gl_PointSize = size; \n"															\
		"} \n";

	// ***** Creating Shader Object ***** //
	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// ***** Giving Shader Code To Shader Object ***** //
	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	// ***** Compile The Shader ***** //
	glCompileShader(vertexShaderObject);

	// ***** Variables To Check The Shader Compilation Error ***** //
	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	// ***** Error Checking For Shader Compilation ***** //
	// ***** Getting Compilation Status ***** //
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// ***** Getting Length Of Log of Compilation Status ***** //
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			// ***** Allocate Memory To Hold The Compilation Error Log ***** //
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				// ***** Get The Compilation Error Log Into Allocated Buffer ***** //
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				LOG("Vertex Shader Compilation Log : %s \n");
				// ***** Free Allocate Buffer ***** //
				free(log);
				// ***** Exit The Application Due To Error In Shader ***** //
				uninitializeStarfieldShader();
			}
		}
	}

	// ***** Fragment Shader ***** //	
	// ***** Writing Shader Code ***** //
	const GLchar* fragmentShaderSourceCode =
		"#version 460 core \n"															\
		"\n"																			\

		"flat in vec4 a_color_out; \n"													\
		"uniform sampler2D u_textureSampler; \n"										\
		"in vec2 a_texcoord_out; \n"													\
		"uniform bool enable_godRays = true; \n" \
		"out vec4 FragColor; \n"														\

		"void main(void) \n"															\
		"{ \n"																			\
			"if (enable_godRays) \n" \
			"{" \
				"FragColor = texture(u_textureSampler, gl_PointCoord); \n"	\
			"}" \
			"else" \
			"{\n" \
				"FragColor = vec4(0.0, 0.0, 0.0, 1.0); \n"	\
			"}\n" \
			
		"}";

	// ***** Creating Shader Object ***** //
	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// ***** Giving Shader Code To Shader Object ***** //
	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	// ***** Compile The Shader ***** //
	glCompileShader(fragmentShaderObject);

	// ***** Variables To Check The Shader Compilation Error ***** //
	status = 0;
	infoLogLength = 0;
	log = NULL;

	// ***** Error Checking For Shader Compilation ***** //
	// ***** Getting Compilation Status ***** //
	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		// ***** Getting Length Of Log of Compilation Status ***** //
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			// ***** Allocate Memory To Hold The Compilation Error Log ***** //
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				// ***** Get The Compilation Error Log Into Allocated Buffer ***** //
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				LOG("Fragment Shader Compilation Log : %s \n");
				// ***** Free Allocate Buffer ***** //
				free(log);
				// ***** Exit The Application Due To Error In Shader ***** //
				uninitializeStarfieldShader();
			}
		}
	}

	// ***** Creating Shader Program Object ***** //
	shaderProgramObj_starfield = glCreateProgram();

	// ***** Attach Created Shaders To Shader Program Object ***** //
	glAttachShader(shaderProgramObj_starfield, vertexShaderObject);
	glAttachShader(shaderProgramObj_starfield, fragmentShaderObject);
	glBindAttribLocation(shaderProgramObj_starfield, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObj_starfield, DOMAIN_ATTRIBUTE_COLOR, "a_color");
	glBindAttribLocation(shaderProgramObj_starfield, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

	// ***** Link Shader Program Object ***** //
	glLinkProgram(shaderProgramObj_starfield);

	// ***** Variables To Check The Shader Linking Error ***** //
	status = 0;
	infoLogLength = 0;
	log = NULL;

	// ***** Error Checking For Shader Linking ***** //
	// ***** Getting Linking Status ***** //
	glGetProgramiv(shaderProgramObj_starfield, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		// ***** Getting Length Of Log of Link Status ***** //
		glGetProgramiv(shaderProgramObj_starfield, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			// ***** Allocate Memory To Hold The Link Error Log ***** //
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				// ***** Get The Link Error Log Into Allocated Buffer ***** //
				glGetProgramInfoLog(shaderProgramObj_starfield, infoLogLength, &written, log);
				LOG("Shader Program Link Log : %s \n");
				// ***** Free Allocate Buffer ***** //
				free(log);
				// ***** Exit The Application Due To Error In Shader ***** //
				uninitializeStarfieldShader();
			}
		}
	}

    starfieldUniform.modelMatrix = glGetUniformLocation(shaderProgramObj_starfield, "u_modelMatrix");
    starfieldUniform.viewMatrix = glGetUniformLocation(shaderProgramObj_starfield, "u_viewMatrix");
    starfieldUniform.projectionMatrix = glGetUniformLocation(shaderProgramObj_starfield, "u_projectionMatrix");
	starfieldUniform.timeUniform = glGetUniformLocation(shaderProgramObj_starfield, "u_time");
	starfieldUniform.textureSamplerUniform = glGetUniformLocation(shaderProgramObj_starfield, "u_textureSampler");
	starfieldUniform.uniform_enable_godRays = glGetUniformLocation(shaderProgramObj_starfield, "enable_godRays");
    return (0);
}

struct StarfieldUniform useStarfieldShader(void)
{
    glUseProgram(shaderProgramObj_starfield);
    return starfieldUniform;
}

void uninitializeStarfieldShader(void)
{
    if (shaderProgramObj_starfield)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_starfield);
		glGetProgramiv(shaderProgramObj_starfield, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_starfield, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_starfield, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_starfield);
		shaderProgramObj_starfield = 0;
    }
    
}