// #include <windows.h>
#include "../../inc/shaders/ADSLightDynamicShader.h"

typedef unsigned char byte;

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
		"in vec3 a_tangent; \n" \
		"in vec3 a_bitangent; \n" \
		"in ivec4 a_BoneIds; \n" \
		"in vec4 a_BoneWeight; \n" \

		"uniform mat4 u_modelMatrix; \n" \
		"uniform mat4 u_viewMatrix; \n" \
		"uniform mat4 u_projectionMatrix; \n" \

		"uniform vec4 u_lightPosition; \n" \

		// Shadow
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
		
		//fog
		"uniform int u_fogEnable; \n" \
		"uniform float u_density; \n"	\
		"uniform float u_gradient; \n"	\
		"out float visibility; \n"		\

		/*skeletal anim*/
		"const int MAX_BONES = 100; \n" \
		"const int MAX_BONE_INFLUENCE = 4; \n" \
		"uniform mat4 u_finalBonesMatrices[MAX_BONES]; \n" \

		"out vec3 transformedNormals; \n" \
		"out vec3 lightDirection; \n" \
		"out vec3 viewerVector;\n" \
		"out vec4 a_color_out;\n" \
		"out vec2 a_texcoord_out;\n" \

		//normal mapping
		"vec4 FragPos; \n" \
		"out vec3 a_eyeDirection_out; \n" \
		"out vec3 a_lightDirection_out; \n" \
		"out vec3 a_fragPosNM_out; \n" \
		"uniform vec3 viewPosition; \n"	\

		"void main(void) \n" \
		"{ \n" \
		
		"	vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position; \n" \
		"	mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix); \n" \
		"	transformedNormals = normalMatrix * a_normal; \n" \
		"	lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz; \n" \
		"	viewerVector = vec3(-eyeCoordinates); \n" \

		//Normal Mapping
		"vs_out.FragPos = u_modelMatrix * a_position; \n" \
		"mat3 normalMatrix_nm = mat3(transpose(inverse(u_modelMatrix))); \n" \
		"vec3 N = normalize(normalMatrix_nm * a_normal); \n" \
		"vec3 T = normalize(normalMatrix_nm * a_tangent); \n" \
		"T = normalize(T- dot(T,N) * N); \n" \
		"vec3 B = cross(N,T); \n" \
		"mat3 TBN = transpose(mat3(T,B,N)); \n" \
		"a_lightDirection_out = TBN * vec3(u_lightPosition) ; \n" \
		"a_eyeDirection_out =  TBN *  viewPosition; \n" \
		"a_fragPosNM_out =TBN *  vs_out.FragPos.xyz; \n" \

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

		"if (u_actualScene == 1) \n" \
		"{ \n" \
		"	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * totalPosition; \n" \

		"	if (u_fogEnable == 1) \n" \
		"	{ \n" \
		"		vec4 positionRelativeToCamera = u_viewMatrix * u_modelMatrix * a_position; \n"		\
		"		float distance = length(positionRelativeToCamera.xyz); \n"							\
		"		visibility = exp(-pow((distance * u_density), u_gradient)); \n"					\
		"		visibility = clamp(visibility, 0.0f, 1.0f); \n"									\
		"	} \n" \

			// Shadow
			"vs_out.Normal = mat3(transpose(inverse(u_modelMatrix))) * a_normal; \n" \
			"vs_out.FragPosLightSpace = lightSpaceMatrix * vs_out.FragPos; \n" \

		"	a_color_out = a_color;\n" \
		"	a_texcoord_out = a_texcoord;\n" \
		"} \n" \

		"if(u_depthScene == 1) { \n" \

		"gl_Position = lightSpaceMatrix * u_modelMatrix * totalPosition; \n" \

		"} \n" \
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
		"in float visibility; \n"		\

		"uniform vec4 u_la; \n" \
		"uniform vec4 u_ld; \n" \
		"uniform vec4 u_ls; \n" \
		"uniform vec4 u_ka; \n" \
		"uniform vec4 u_kd; \n" \
		"uniform vec4 u_ks; \n" \
		"uniform float u_materialShininess; \n" \
		
		"uniform sampler2D u_texturesampler;\n" \

		"uniform int u_fogEnable; \n" \
		"uniform vec4 u_skyFogColor; \n"	\
		"uniform int enable_godRays; \n" \
		"uniform int enable_sphere_color; \n" \

		//normal mapping
		"in vec3 a_eyeDirection_out; \n" \
		"in vec3 a_lightDirection_out; \n" \
		"in vec3 a_fragPosNM_out; \n" \
		"uniform sampler2D texture_normal; \n" \

		// Shadow
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

		"uniform float u_colorCorrection; \n" \

		"out vec4 FragColor; \n" \
		"out vec4 normal_depth; \n" \

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
			"shadow /= 12.0; \n" \

			"if (projCoords.z > 1.0) \n" \
			"shadow = 0.0; \n" \

			"return shadow; \n" \
		"} \n" \

		"void main(void) \n" \
		"{ \n" \
		"	if (enable_godRays == 1) \n" \
		"	{\n" \
				"if(u_actualScene == 1) { \n" \
		"		vec4 phong_ads_light; \n" \
		"		vec4 texColor = texture(u_texturesampler, a_texcoord_out); \n" \

		        //read normals from normal map and normalize it
		"       vec3 normalizedNormals = normalize(texture(texture_normal,a_texcoord_out).rgb*2.0 - vec3(1.0)); \n" \
				//ambient
		"		vec4 ambient = 0.0 * texColor; \n" \
				
				//diffuse
		"       vec3 normalized_lightDirection = normalize(a_lightDirection_out - a_fragPosNM_out ); \n" \
		"       vec4 diffuse = u_ld * texColor * max(dot(normalized_lightDirection,normalizedNormals), 0.0); \n" \

		//"		vec3 normalized_transformed_normals = normalize(transformedNormals); \n" \
		//"		vec3 normalized_light_direction = normalize(lightDirection); \n" \
		//"		vec4 diffuse = u_ld * u_kd * texColor * max(dot(normalized_light_direction, normalized_transformed_normals), 0.0); \n" \
		
		//"		vec3 reflectionVector = reflect(-normalized_light_direction, normalized_transformed_normals); \n" \
		//"		vec3 normalized_viewer_vector = normalize(viewerVector); \n" \
		//"		vec4 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalized_viewer_vector), 0.0), u_materialShininess); \n" \
		
					//specular
		"       vec3 normalized_viewerVector = normalize(a_eyeDirection_out - a_fragPosNM_out); \n" \
		"       vec3 reflectionVector = reflect(-normalized_lightDirection, normalizedNormals); \n" \
		"       vec3 halfwayDir = normalize(normalized_lightDirection + normalized_viewerVector); \n" \
	
		"       vec4 specular = u_ls * pow(max(dot(normalizedNormals, halfwayDir), 0.0), u_materialShininess); \n" \
			"float shadow = ShadowCalculation(fs_in.FragPosLightSpace); \n" \
		"		phong_ads_light = ambient + (1.0 - shadow) * (diffuse + specular); \n" \
		
		"		FragColor = (texColor - vec4(u_colorCorrection)) + phong_ads_light; \n" \

		"		if (u_fogEnable == 1) \n" \
		"		{ \n" \
		"			FragColor = mix(u_skyFogColor, phong_ads_light, visibility); \n" \
		"		} \n" \
				"if(u_depthScene == 1) { \n" \

				"FragColor = vec4(1.0); \n" \

				"} \n" \
		"		} \n" \
		"	} \n" \

		"	else if(enable_sphere_color == 1)\n" \
		"	{\n" \
		"		FragColor = a_color_out; \n" \
		"	}\n" \
		"	else\n" \
		"	{\n" \
		"		FragColor = vec4(0.0, 0.0, 0.0, 1.0); \n" \
		"	}\n" \
		"	FragColor = mix(vec4(0.0), FragColor, 0.7);\n" \
		"	normal_depth = vec4(normalize( transformedNormals ), viewerVector.z); \n" \
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
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_TANGENT, "a_tangent");
	glBindAttribLocation(adsDynamicShaderProgramObject, DOMAIN_ATTRIBUTE_BITANGENT, "a_bitangent");

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

	//For Normal Mapping
	adsDynamicUniform.viewpositionUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "viewPosition");
	adsDynamicUniform.textureSamplerUniform_normal = glGetUniformLocation(adsDynamicShaderProgramObject, "texture_normal");


	adsDynamicUniform.gradientUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_gradient");
	adsDynamicUniform.densityUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_density");
	adsDynamicUniform.skyFogColorUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_skyFogColor");
	adsDynamicUniform.fogEnableUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_fogEnable");
	adsDynamicUniform.uniform_enable_godRays = glGetUniformLocation(adsDynamicShaderProgramObject, "enable_godRays");
	adsDynamicUniform.godrays_blackpass_sphere = glGetUniformLocation(adsDynamicShaderProgramObject, "enable_sphere_color");


	// Shadow
	adsDynamicUniform.lightSpaceMatrixUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "lightSpaceMatrix");
	adsDynamicUniform.shadowMapSamplerUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "shadowMap");
	adsDynamicUniform.actualSceneUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_actualScene");
	adsDynamicUniform.depthSceneUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_depthScene");
	adsDynamicUniform.depthQuadSceneUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_depthQuadScene");
	adsDynamicUniform.depthTextureSamplerUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "depthMap");
	
	adsDynamicUniform.colorCorrectionUniform = glGetUniformLocation(adsDynamicShaderProgramObject, "u_colorCorrection");

	for (int i = 0; i < MAX_BONES; i++)
	{
		byte str[100];
		snprintf((char* const)str, 100, "u_finalBonesMatrices[%d]", i);
		adsDynamicUniform.finalBonesMatricesUniform[i] = glGetUniformLocation(adsDynamicShaderProgramObject, (const GLchar*)str);
	}

	glUseProgram(adsDynamicShaderProgramObject);
    glUniform1i(adsDynamicUniform.textureSamplerUniform, 0);
	glUniform1i(adsDynamicUniform.shadowMapSamplerUniform, 1);
	glUniform1i(adsDynamicUniform.textureSamplerUniform_normal, 2);   //don't change
	glUseProgram(0);

	LOG("Shader initialized\n");
	return(0);
}

struct ADSDynamicUniform useADSDynamicShader(void)
{
    // Code
    glUseProgram(adsDynamicShaderProgramObject);
    return adsDynamicUniform;
}

GLuint getDynamicShaderProgramObject(void) 
{
	return adsDynamicShaderProgramObject;
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



