
#include "../../inc/shaders/ADSLightDynamicShader.h"

// Variable Declarations
GLuint adsDynamicShaderProgramObject;
ADSDynamicUniform adsDynamicUniform;

int initializeADSDynamicShader(void)
{

	// Function Declarations
	void uninitializeADSDynamicShader(void);

    // Code
    ZeroMemory((void*)&adsDynamicUniform, sizeof(ADSDynamicUniform));

	// Vertex Shader
	const GLchar* vertexShaderSourcecode =
		"#version 460 core \n" \
		"\n" \
		"in vec4 a_position; \n" \
		"in vec4 a_color; \n" \
		"in vec2 a_texcoord; \n" \
		"in vec3 a_normal; \n"	\
		"in ivec4 a_BoneIds; \n" \
		"in vec4 a_BoneWeight; \n" \

		"uniform mat4 u_modelMatrix; \n" \
		"uniform mat4 u_viewMatrix; \n" \
		"uniform mat4 u_projectionMatrix; \n" \

		"uniform vec4 u_lightPosition; \n" \

		/*skeletal anim*/
		"const int MAX_BONES = 100; \n" \
		"const int MAX_BONE_INFLUENCE = 4; \n" \
		"uniform mat4 u_finalBonesMatrices[MAX_BONES]; \n" \

		"out vec3 transformedNormals; \n" \
		"out vec3 lightDirection; \n" \
		"out vec3 viewerVector;\n" \
		"out vec4 a_color_out;\n" \
		"out vec2 a_texcoord_out;\n" \

		"void main(void) \n" \
		"{ \n" \
			
		"	vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position; \n" \
		"	mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix); \n" \
		"	transformedNormals = normalMatrix * a_normal; \n" \
		"	lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz; \n" \
		"	viewerVector = vec3(-eyeCoordinates); \n" \
			
		/*skeletal anim*/
		"	vec4 totalPosition = vec4(0.0); \n" \

		"	for(int i = 0; i < MAX_BONE_INFLUENCE; i++) \n" \
		"	{ \n" \

		"		if(a_BoneIds[i] == -1) \n" \
		"			continue; \n" \
		"		if(a_BoneIds[i] >= MAX_BONES) \n" \
		"		{ \n" \
		"			totalPosition = a_position; \n" \
		"			break; \n" \
		"		} \n" \

		"		vec4 localPosition = u_finalBonesMatrices[a_BoneIds[i]] * a_position; \n" \
		"		totalPosition += localPosition * a_BoneWeight[i]; \n" \
		"		vec3 localNormal = mat3(u_finalBonesMatrices[a_BoneIds[i]]) * a_normal; \n" \

		"	} \n" \

		"	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * totalPosition; \n" \

		"	a_color_out = a_color;\n" \
		"	a_texcoord_out = a_texcoord;\n" \
		"} \n";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourcecode, NULL);

	glCompileShader(vertexShaderObject);

	GLint status;
	GLint infoLogLength;
	char* log = NULL;

	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {

		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				LOG("ADS Vertex Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeADSDynamicShader();

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
		
		"uniform sampler2D u_texturesampler;\n" \

		"out vec4 FragColor; \n" \

		"void main(void) \n" \
		"{ \n" \
		"	vec4 phong_ads_light; \n" \
		"	vec4 texColor = texture(u_texturesampler, a_texcoord_out); \n" \
		
		"	vec4 ambient = u_la * u_ka; \n" \

		"	vec3 normalized_transformed_normals = normalize(transformedNormals); \n" \
		"	vec3 normalized_light_direction = normalize(lightDirection); \n" \
		"	vec4 diffuse = u_ld * u_kd * texColor * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0); \n" \
		
		"	vec3 reflectionVector = reflect(-normalized_light_direction, normalized_transformed_normals); \n" \
		"	vec3 normalized_viewer_vector = normalize(viewerVector); \n" \
		"	vec4 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewer_vector), 0.0), u_materialShininess); \n" \
		
		"	phong_ads_light = ambient + diffuse + specular; \n" \
		
		"	FragColor = phong_ads_light; \n" \
			/*"FragColor = vec4(phong_ads_light * vec3(a_color_out), 1.0); \n" \*/
		"} \n";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourcecode, NULL);

	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {

		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				LOG("ADS Fragment Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeADSDynamicShader();

			}

		}

	}

	// Shader Program Object
	adsDynamicShaderProgramObject = glCreateProgram();
	glAttachShader(adsDynamicShaderProgramObject, vertexShaderObject);
	glAttachShader(adsDynamicShaderProgramObject, fragmentShaderObject);
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_NORMAL, "a_normal");
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_COLOR, "a_color");
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_BONE_ID, "a_BoneIds");
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_BONE_WEIGHT, "a_BoneWeight");

	glLinkProgram(adsDynamicShaderProgramObject);
	
	glGetProgramiv(adsDynamicShaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
	
		glGetShaderiv(adsDynamicShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(adsDynamicShaderProgramObject, infoLogLength, &written, log);
				LOG("ADS ShaderProgram Linking Log: %s\n", log);
				free(log);
				uninitializeADSDynamicShader();

			}

		}

	}

	//
	adsDynamicUniform.modelMatrixUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_modelMatrix");
	adsDynamicUniform.viewMatrixUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_viewMatrix");
	adsDynamicUniform.projectionMatrixUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_projectionMatrix");
	
	adsDynamicUniform.laUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_la");
	adsDynamicUniform.ldUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_ld");
	adsDynamicUniform.lsUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_ls");
	adsDynamicUniform.kaUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_ka");
	adsDynamicUniform.kdUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_kd");
	adsDynamicUniform.ksUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_ks");
	adsDynamicUniform.lightPositionUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_lightPosition");
	adsDynamicUniform.materialShininessUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_materialShininess");
	
	adsDynamicUniform.textureSamplerUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_texturesampler");

	for (int i = 0; i < MAX_BONES; i++)
	{
		byte str[100];
		sprintf_s((char* const)str, 100, "u_finalBonesMatrices[%d]", i);
		adsDynamicUniform.finalBonesMatricesUniform[i] = glGetUniformLocation(adsDynamicShaderProgramObject, (const GLchar*)str);
	}

	glUseProgram(adsDynamicShaderProgramObject);
    glUniform1i(adsDynamicUniform.textureSamplerUniform, 0);
	glUseProgram(0);


	return(0);

}

struct ADSDynamicUniform useADSDynamicShader(void)
{
    // Code
    glUseProgram(adsDynamicShaderProgramObject);
    return adsDynamicUniform;
}

void uninitializeADSDynamicShader(void)
{

    // Code
    // Shader Uninitialization
	if (adsDynamicShaderProgramObject) {

		GLsizei numAttachedShader;
		glUseProgram(adsDynamicShaderProgramObject);
		glGetProgramiv(adsDynamicShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(adsDynamicShaderProgramObject, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(adsDynamicShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(adsDynamicShaderProgramObject);
		adsDynamicShaderProgramObject = 0;

	}

}



