
#include "../../inc/shaders/ADSLightShader.h"

// Variable Declarations
GLuint adsShaderProgramObject;

extern FILE* gpFile;

ADSUniform adsUniform;

int initializeADSShader(void)
{

	// Function Declarations
	void uninitializeADSShader(void);

    // Code
    ZeroMemory((void*)&adsUniform, sizeof(ADSUniform));

	// Vertex Shader
	const GLchar* vertexShaderSourcecode =
		"#version 460 core \n" \
		"\n" \
		"in vec4 a_position; \n" \
		"in vec4 a_color; \n" \
		"in vec2 a_texcoord; \n" \
		"in vec3 a_normal; \n"	\
		"uniform mat4 u_modelMatrix; \n" \
		"uniform mat4 u_viewMatrix; \n" \
		"uniform mat4 u_projectionMatrix; \n" \
		"uniform vec4 u_lightPosition; \n" \
		"uniform int u_lightingEnable; \n" \
		"out vec3 transformedNormals; \n" \
		"out vec3 lightDirection; \n" \
		"out vec3 viewerVector;\n" \
		"out vec4 a_color_out;\n" \
		"out vec2 a_texcoord_out;\n" \
		"void main(void) \n" \
		"{ \n" \
			"if (u_lightingEnable == 1) \n" \
			"{ \n" \
				"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position; \n" \
				"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix); \n" \
				"transformedNormals = normalMatrix * a_normal; \n" \
				"lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz; \n" \
				"viewerVector = vec3(-eyeCoordinates); \n" \
			"} \n" \
			"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position; \n" \
			"a_color_out = a_color;\n" \
			"a_texcoord_out = a_texcoord;\n" \
		"} \n";

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
				fprintf(gpFile, "ADS Vertex Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeADSShader();

			}

		}

	}


	// Fragment Shader
	const GLchar* fragmentShaderSourcecode =
		"#version 460 core \n" \
		"\n" \
		"in vec4 a_color_out;\n" \
		"in vec2 a_texcoord_out;\n" \
		"in vec3 transformedNormals; \n" \
		"in vec3 lightDirection; \n" \
		"in vec3 viewerVector;\n" \
		"uniform vec4 u_la; \n" \
		"uniform vec4 u_ld; \n" \
		"uniform vec4 u_ls; \n" \
		"uniform vec4 u_ka; \n" \
		"uniform vec4 u_kd; \n" \
		"uniform vec4 u_ks; \n" \
		"uniform float u_materialShininess; \n" \
		"uniform int u_lightingEnable; \n" \
		"uniform sampler2D u_texturesampler;\n" \
		"out vec4 FragColor; \n" \
		"void main(void) \n" \
		"{ \n" \
			"vec4 phong_ads_light; \n" \
			"vec4 texColor = texture(u_texturesampler, a_texcoord_out); \n"		\
			"vec4 ambient = u_la * u_ka; \n" \
			"vec3 normalized_transformed_normals = normalize(transformedNormals); \n" \
			"vec3 normalized_light_direction = normalize(lightDirection); \n" \
			"vec4 diffuse = u_ld * u_kd * texColor * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0); \n" \
			"vec3 reflectionVector = reflect(-normalized_light_direction, normalized_transformed_normals); \n" \
			"vec3 normalized_viewer_vector = normalize(viewerVector); \n" \
			"vec4 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewer_vector), 0.0), u_materialShininess); \n" \
			"phong_ads_light = ambient + diffuse + specular; \n" \
			"FragColor = phong_ads_light; \n" \
			/*"FragColor = vec4(phong_ads_light * vec3(a_color_out), 1.0); \n" \*/
		"} \n";

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
				fprintf(gpFile, "ADS Fragment Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeADSShader();

			}

		}

	}

	// Shader Program Object
	adsShaderProgramObject = glCreateProgram();
	glAttachShader(adsShaderProgramObject, vertexShadderObject);
	glAttachShader(adsShaderProgramObject, fragmentShadderObject);
	glBindAttribLocation(adsShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(adsShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");
	glBindAttribLocation(adsShaderProgramObject, DOMAIN_ATTRIBUTE_NORMAL, "a_normal");
	glBindAttribLocation(adsShaderProgramObject, DOMAIN_ATTRIBUTE_COLOR, "a_color");

	glLinkProgram(adsShaderProgramObject);
	
	glGetProgramiv(adsShaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
	
		glGetShaderiv(adsShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(adsShaderProgramObject, infoLogLength, &written, log);
				fprintf(gpFile, "ADS ShaderProgram Linking Log: %s\n", log);
				free(log);
				uninitializeADSShader();

			}

		}

	}

	//
	adsUniform.modelMatrixUniform = glGetUniformLocation(adsShaderProgramObject, "u_modelMatrix");
	adsUniform.viewMatrixUniform = glGetUniformLocation(adsShaderProgramObject, "u_viewMatrix");
	adsUniform.projectionMatrixUniform = glGetUniformLocation(adsShaderProgramObject, "u_projectionMatrix");
	adsUniform.laUniform = glGetUniformLocation(adsShaderProgramObject, "u_la");
	adsUniform.ldUniform = glGetUniformLocation(adsShaderProgramObject, "u_ld");
	adsUniform.lsUniform = glGetUniformLocation(adsShaderProgramObject, "u_ls");
	adsUniform.kaUniform = glGetUniformLocation(adsShaderProgramObject, "u_ka");
	adsUniform.kdUniform = glGetUniformLocation(adsShaderProgramObject, "u_kd");
	adsUniform.ksUniform = glGetUniformLocation(adsShaderProgramObject, "u_ks");
	adsUniform.lightPositionUniform = glGetUniformLocation(adsShaderProgramObject, "u_lightPosition");
	adsUniform.materialShininessUniform = glGetUniformLocation(adsShaderProgramObject, "u_materialShininess");
	adsUniform.lightingEnableUniform = glGetUniformLocation(adsShaderProgramObject, "u_lightingEnable");
	adsUniform.textureSamplerUniform = glGetUniformLocation(adsShaderProgramObject, "u_texturesampler");

    
	glUseProgram(adsShaderProgramObject);
    glUniform1i(adsUniform.textureSamplerUniform, 0);
	glUseProgram(0);


	return(0);

}

struct ADSUniform useADSShader(void)
{

    // Code
    glUseProgram(adsShaderProgramObject);
    return adsUniform;

}

void uninitializeADSShader(void)
{

    // Code
    // Shader Uninitialization
	if (adsShaderProgramObject) {

		GLsizei numAttachedShader;
		glUseProgram(adsShaderProgramObject);
		glGetProgramiv(adsShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(adsShaderProgramObject, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(adsShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(adsShaderProgramObject);
		adsShaderProgramObject = 0;

	}

}



