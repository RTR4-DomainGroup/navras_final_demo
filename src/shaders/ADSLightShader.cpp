
#include "../../inc/shaders/ADSLightShader.h"

// Variable Declarations
GLuint adsShaderProgramObject;

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
		"uniform int u_fogEnable; \n" \

		"uniform int u_actualScene; \n" \
		"uniform int u_depthScene; \n" \
		"uniform int u_depthQuadScene; \n" \

		"uniform mat4 lightSpaceMatrix; \n" \
		"out VS_OUT{ \n" \
			"vec4 FragPos; \n" \
			"vec3 Normal; \n" \
			"vec2 TexCoords; \n" \
			"vec4 FragPosLightSpace; \n" \
		"} vs_out; \n" \

		"uniform float u_density; \n"	\
		"uniform float u_gradient; \n"	\

		"out vec3 transformedNormals; \n" \
		"out vec3 lightDirection; \n" \
		"out vec3 viewerVector;\n" \
		"out vec4 a_color_out;\n" \
		"out vec2 a_texcoord_out;\n" \
		"out float visibility; \n"		\

		"void main(void) \n" \
		"{ \n" \
			"if (u_actualScene == 1) \n" \
			"{ \n" \
				"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position; \n" \
				"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix); \n" \
				"transformedNormals = normalMatrix * a_normal; \n" \
				"lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz; \n" \
				"viewerVector = vec3(-eyeCoordinates); \n" \

				"vs_out.FragPos = u_modelMatrix * a_position; \n" \
				"vs_out.Normal = mat3(transpose(inverse(u_modelMatrix))) * a_normal; \n" \
				"vs_out.FragPosLightSpace = lightSpaceMatrix * vs_out.FragPos; \n" \

				"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position; \n" \
				"a_color_out = a_color;\n" \
				"a_texcoord_out = a_texcoord;\n" \

			"} \n" \
			

			"if(u_depthScene == 1) { \n" \

				"gl_Position = lightSpaceMatrix * u_modelMatrix * a_position; \n" \

			"} \n" \

			"if(u_depthQuadScene == 1) { \n" \

				"a_texcoord_out = a_texcoord;\n" \
				"gl_Position = a_position; \n" \

			"} \n" \

			"if (u_fogEnable == 1) \n" \
			"{ \n" \
				"vec4 positionRelativeToCamera = u_viewMatrix * u_modelMatrix * a_position; \n"		\
				"float distance = length(positionRelativeToCamera.xyz); \n"							\
				"visibility = exp(-pow((distance * u_density), u_gradient)); \n"					\
				"visibility = clamp(visibility, 0.0f, 1.0f); \n"									\
			"} \n" \
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
				LOG("ADS Vertex Shader Compilation Log: %s\n", log);
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
		"in float visibility; \n"		\

		"uniform vec4 u_la; \n" \
		"uniform vec4 u_ld; \n" \
		"uniform vec4 u_ls; \n" \
		"uniform vec4 u_ka; \n" \
		"uniform vec4 u_kd; \n" \
		"uniform vec4 u_ks; \n" \
		"uniform float u_materialShininess; \n" \
		"uniform int u_lightingEnable; \n" \
		"uniform int u_fogEnable; \n" \
		"uniform sampler2D u_texturesampler;\n" \
		"uniform vec4 u_skyFogColor; \n"	\
		"uniform int enable_godRays; \n" \
		"uniform int enable_sphere_color; \n" \

		"uniform int u_actualScene; \n" \
		"uniform int u_depthScene; \n" \
		"uniform int u_depthQuadScene; \n" \
		"in VS_OUT{ \n" \
			"vec4 FragPos; \n" \
			"vec3 Normal; \n" \
			"vec2 TexCoords; \n" \
			"vec4 FragPosLightSpace; \n" \
		"} fs_in; \n" \
		"uniform sampler2D shadowMap; \n" \
		"uniform vec4 u_lightPosition; \n" \
		
		// depthQUad
		"uniform sampler2D depthMap; \n" \
		"uniform float near_plane; \n" \
		"uniform float far_plane; \n" \

		"out vec4 FragColor; \n" \

		"float ShadowCalculation(vec4 fragPosLightSpace) \n" \
		"{ \n" \
			"vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; \n" \
			"projCoords = projCoords * 0.5 + 0.5; \n" \
			"float closestDepth = texture(shadowMap, projCoords.xy).r; \n" \
			"float currentDepth = projCoords.z; \n" \
			"vec3 normal = normalize(fs_in.Normal); \n" \
			"vec3 lightDir = normalize(u_lightPosition - fs_in.FragPos).xyz; \n" \
			"float bias = max(0.025 * (1.0 - dot(normal, lightDir)), 0.005); \n" \
			"float shadow = 0.0; \n" \
			"vec2 texelSize = 1.0 / textureSize(shadowMap, 0); \n" \
			"for (int x = -1; x <= 1; ++x) \n" \
			"{ \n" \
				"for (int y = -1; y <= 1; ++y) \n" \
				"{ \n" \
					"float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; \n" \
					"shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0; \n" \
				"} \n" \
			"} \n" \
			"shadow /= 9.0; \n" \

			"if (projCoords.z > 1.0) \n" \
			"shadow = 0.0; \n" \

			"return shadow; \n" \
		"} \n" \

		"float LinearizeDepth(float depth) \n" \
		"{ \n" \
			"float z = depth * 2.0 - 1.0; \n" \
			"return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane)); \n" \
		"} \n" \

		"void main(void) \n" \
		"{ \n" \
			"if (enable_godRays == 1) \n" \
			"{\n" \
				"if(u_actualScene == 1) { \n" \
					"vec4 phong_ads_light; \n" \
					"vec4 texColor = texture(u_texturesampler, a_texcoord_out); \n"		\
					"vec4 ambient = u_la * u_ka; \n" \
					"vec3 normalized_transformed_normals = normalize(transformedNormals); \n" \
					"vec3 normalized_light_direction = normalize(lightDirection); \n" \
					"vec4 diffuse = u_ld * u_kd * texColor * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0); \n" \
					"vec3 reflectionVector = reflect(-normalized_light_direction, normalized_transformed_normals); \n" \
					"vec3 normalized_viewer_vector = normalize(viewerVector); \n" \
					"vec4 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewer_vector), 0.0), u_materialShininess); \n" \
					
					"float shadow = ShadowCalculation(fs_in.FragPosLightSpace); \n" \
					"phong_ads_light = (ambient + (1.0 - shadow) * (diffuse + specular)); \n" \
					
					"FragColor = phong_ads_light; \n" \
					"if (u_fogEnable == 1) \n" \
					"{ \n" \
						"FragColor = mix(u_skyFogColor, phong_ads_light, visibility); \n" \
					"} \n" \
				"} \n" \

				"if(u_depthScene == 1) { \n" \

					"FragColor = vec4(1.0); \n" \

				"} \n" \

				"if(u_depthQuadScene == 1) { \n" \

					"float depthValue = texture(depthMap, a_texcoord_out).r; \n" \
					"FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0); \n" \
					//"FragColor = vec4(vec3(depthValue), 1.0); \n" \ /

				"} \n" \

			"}\n" \
			"else if(enable_sphere_color == 1)\n" \
			"{\n" \
				"FragColor = a_color_out; \n" \
			"}\n" \
			"else\n" \
			"{\n" \
				"FragColor = vec4(0.0, 0.0, 0.0, 1.0); \n" \
			"}\n" \
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
				LOG("ADS Fragment Shader Compilation Log: %s\n", log);
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
				LOG("ADS ShaderProgram Linking Log: %s\n", log);
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

	adsUniform.lightSpaceMatrixUniform = glGetUniformLocation(adsShaderProgramObject, "lightSpaceMatrix");
	adsUniform.shadowMapSamplerUniform = glGetUniformLocation(adsShaderProgramObject, "shadowMap");
	adsUniform.actualSceneUniform = glGetUniformLocation(adsShaderProgramObject, "u_actualScene");
	adsUniform.depthSceneUniform = glGetUniformLocation(adsShaderProgramObject, "u_depthScene");
	adsUniform.depthQuadSceneUniform = glGetUniformLocation(adsShaderProgramObject, "u_depthQuadScene");
	adsUniform.farUniform = glGetUniformLocation(adsShaderProgramObject, "far_plane");
	adsUniform.nearUniform = glGetUniformLocation(adsShaderProgramObject, "near_plane");
	adsUniform.depthTextureSamplerUniform = glGetUniformLocation(adsShaderProgramObject, "depthMap");

	adsUniform.gradientUniform = glGetUniformLocation(adsShaderProgramObject, "u_gradient");
	adsUniform.densityUniform = glGetUniformLocation(adsShaderProgramObject, "u_density");
	adsUniform.skyFogColorUniform = glGetUniformLocation(adsShaderProgramObject, "u_skyFogColor");
	adsUniform.fogEnableUniform = glGetUniformLocation(adsShaderProgramObject, "u_fogEnable");
	adsUniform.uniform_enable_godRays = glGetUniformLocation(adsShaderProgramObject, "enable_godRays");
	adsUniform.godrays_blackpass_sphere = glGetUniformLocation(adsShaderProgramObject, "enable_sphere_color");

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



