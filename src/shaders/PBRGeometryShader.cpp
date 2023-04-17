
#include "../../inc/shaders/PBRGeometryShader.h"

// Variable Declarations
GLuint PBRGeometryShaderProgramObject;

extern FILE* gpFile;

PBRGeometryUniform pbrGeometryUniform;

int initializePBRGeometryShader(void)
{

	// Function Declarations
	void uninitializePBRGeometryShader(void);

	// Code
	ZeroMemory((void*)&pbrGeometryUniform, sizeof(PBRGeometryUniform));

	// Vertex Shader
	const GLchar* vertexShaderSourcecode =
		"#version 460 core \n" \
		"\n" \

		"in vec4 a_position; \n" \
		"in vec2 a_texCoord; \n" \
		"in vec3 a_normal; \n" \

		"uniform mat4 u_modelMatrix; \n" \
		"uniform mat4 u_viewMatrix; \n" \
		"uniform mat4 u_projectionMatrix; \n" \

		"out vec3 transformedNormals; \n" \
		"out vec2 a_texCoord_out; \n" \
		"out vec4 WorldPos; \n" \

		"void main(void) \n" \
		"{ \n" \

		"vec4 WorldPos = u_modelMatrix * a_position; \n" \
		"transformedNormals = vec3(u_modelMatrix) * a_normal; \n" \

		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position ; \n" \
		"a_texCoord_out = a_texCoord; \n" \

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
				LOG("PBRGeometry Vertex Shader Compilation Log: %s\n", log);
				free(log);
				uninitializePBRGeometryShader();

			}

		}

	}


	// Fragment Shader
	const GLchar* fragmentShaderSourcecode =
		"#version 460 core \n" \
		"\n" \

		"in vec2 a_texCoord_out; \n" \
		"in vec4 WorldPos; \n" \
		"in vec3 transformedNormals; \n" \

		"uniform sampler2D albedoMap; \n" \
		"uniform sampler2D normalMap; \n" \
		"uniform sampler2D metallicMap; \n" \
		"uniform sampler2D roughnessMap; \n" \
		"uniform sampler2D aoMap; \n" \

		"uniform vec3 camPos; \n" \

		"uniform vec4 u_lightPosition;"\

		"uniform vec3 lightColors;"\

		"const float PI = 3.14159265359; \n" \
		"out vec4 FragColor; \n" \

		//GET NORMAP FROM NORMAL MAP
		"vec3 getNormalFromMap() \n" \
		"{ \n" \
		"vec3 tangentNormal = texture(normalMap, a_texCoord_out).xyz * 2.0 - 1.0; \n" \

		"vec3 Q1 = dFdx(WorldPos.xyz); \n" \
		"vec3 Q2 = dFdy(WorldPos.xyz); \n" \
		"vec2 st1 = dFdx(a_texCoord_out); \n" \
		"vec2 st2 = dFdy(a_texCoord_out); \n" \

		"vec3 N = normalize(transformedNormals); \n" \
		"vec3 T = normalize(Q1 * st2.t - Q2 * st1.t); \n" \
		"vec3 B = -normalize(cross(N, T)); \n" \
		"mat3 TBN = mat3(T, B, N); \n" \

		"return normalize(TBN * tangentNormal); \n" \
		"} \n"
		//-----------------------------------------------------------------------------
		//DISTRIBUTIONGGX
		"float DistributionGGX(vec3 N, vec3 H, float roughness)\n" \
		"{ \n" \
		"float a = roughness * roughness; \n" \
		"float a2 = a * a; \n" \
		"float NdotH = max(dot(N, H), 0.0); \n" \
		"float NdotH2 = NdotH * NdotH; \n" \

		"float nom = a2; \n" \
		"float denom = (NdotH2 * (a2 - 1.0) + 1.0); \n" \
		"denom = PI * denom * denom; \n" \

		"return nom / denom; \n" \
		"} \n" \
		//-----------------------------------------------------------------------------
		"float GeometrySchlickGGX(float NdotV, float roughness)\n" \
		"{ \n" \
		"float r = (roughness + 1.0);\n" \
		"float k = (r * r) / 8.0; \n" \

		"float nom = NdotV; \n" \
		"float denom = NdotV * (1.0 - k) + k; \n" \

		"return nom / denom; \n" \
		"} \n" \
		// ----------------------------------------------------------------------------
		"float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)\n" \
		"{ \n" \
		"float NdotV = max(dot(N, V), 0.0);\n" \
		"float NdotL = max(dot(N, L), 0.0); \n" \
		"float ggx2 = GeometrySchlickGGX(NdotV, roughness);\n" \
		"float ggx1 = GeometrySchlickGGX(NdotL, roughness); \n" \

		"return ggx1 * ggx2; \n" \
		"} \n" \
		// ----------------------------------------------------------------------------
		"vec3 fresnelSchlick(float cosTheta, vec3 F0)\n" \
		"{ \n" \
		"return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);\n" \
		"} \n" \

		//----------------------------------------------------------------------------
		"void main(void) \n" \
		"{ \n" \

		"vec3 phong_ads_light; \n" \

		"vec3 albedo = pow(texture(albedoMap,a_texCoord_out).rgb, vec3(2.2)); \n" \
		"float metallic  = texture(metallicMap, a_texCoord_out).r; \n" \
		"float roughness = texture(roughnessMap, a_texCoord_out).r; \n" \
		"float ao        = texture(aoMap, a_texCoord_out).r; \n" \


		"vec3 N = getNormalFromMap(); \n" \
		"vec3 V = normalize(camPos - WorldPos.xyz); \n" \

		// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
		// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
		"vec3 F0 = vec3(0.04); \n" \
		"F0 = mix(F0, albedo, metallic);\n" \
		// reflectance equation
		"vec3 Lo = vec3(0.0); \n" \

		// calculate light radiance
		"vec3 L = normalize(vec3(u_lightPosition) - WorldPos.xyz); \n" \
		"vec3 H = normalize(V + L); \n" \
		"float distance = length(vec3(u_lightPosition) - WorldPos.xyz); \n" \
		"float attenuation = 1.0 / (distance * distance);\n" \
		"vec3 radiance = lightColors * attenuation; \n" \

		// Cook-Torrance BRDF
		"float NDF = DistributionGGX(N, H, roughness); \n" \
		"float G = GeometrySmith(N, V, L, roughness);\n" \
		"vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0); \n" \

		"vec3 numerator = NDF * G * F; \n" \
		// + 0.0001 to prevent divide by zero
		"float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;\n" \
		"vec3 specular = numerator / denominator; \n" \

		// kS is equal to Fresnel
		"vec3 kS = F; \n" \
		// for energy conservation, the diffuse and specular light can't
		// be above 1.0 (unless the surface emits light); to preserve this
		// relationship the diffuse component (kD) should equal 1.0 - kS.
		"vec3 kD = vec3(1.0) - kS; \n" \
		// multiply kD by the inverse metalness such that only non-metals 
		// have diffuse lighting, or a linear blend if partly metal (pure metals
		// have no diffuse light).
		"kD *= 1.0 - metallic; \n" \

		// scale light by NdotL
		"float NdotL = max(dot(N, L), 0.0); \n" \

		// add to outgoing radiance Lo
		"Lo += (kD * albedo / PI + specular) * radiance * NdotL; \n" \

		// this ambient lighting with environment lighting).
		"vec3 ambient = vec3(0.8) * albedo * ao;\n" \

		"vec3 color = ambient + Lo;\n" \

		// HDR tonemapping
		"color = color / (color + vec3(1.0));\n" \
		// gamma correct
		"color = pow(color, vec3(1.0 / 2.2));\n" \

		"FragColor = vec4(color, 1.0); \n" \

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
				LOG("PBRGeometry Fragment Shader Compilation Log: %s\n", log);
				free(log);
				uninitializePBRGeometryShader();

			}

		}

	}

	// Shader Program Object
	PBRGeometryShaderProgramObject = glCreateProgram();
	glAttachShader(PBRGeometryShaderProgramObject, vertexShadderObject);
	glAttachShader(PBRGeometryShaderProgramObject, fragmentShadderObject);
	glBindAttribLocation(PBRGeometryShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(PBRGeometryShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texCoord");
	glBindAttribLocation(PBRGeometryShaderProgramObject, DOMAIN_ATTRIBUTE_NORMAL, "a_normal");

	glLinkProgram(PBRGeometryShaderProgramObject);

	glGetProgramiv(PBRGeometryShaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {

		glGetShaderiv(PBRGeometryShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {

			log = (char*)malloc(infoLogLength);
			if (log != NULL) {

				GLsizei written;
				glGetShaderInfoLog(PBRGeometryShaderProgramObject, infoLogLength, &written, log);
				LOG("PBRGeometry ShaderProgram Linking Log: %s\n", log);
				free(log);
				uninitializePBRGeometryShader();

			}

		}

	}

	//
	pbrGeometryUniform.modelMatrixUniform = glGetUniformLocation(PBRGeometryShaderProgramObject, "u_modelMatrix");
	pbrGeometryUniform.viewMatrixUniform = glGetUniformLocation(PBRGeometryShaderProgramObject, "u_viewMatrix");
	pbrGeometryUniform.projectionMatrixUniform = glGetUniformLocation(PBRGeometryShaderProgramObject, "u_projectionMatrix");

	pbrGeometryUniform.lightPositionUniform = glGetUniformLocation(PBRGeometryShaderProgramObject, "u_lightPosition");
	pbrGeometryUniform.lightingEnableUniform = glGetUniformLocation(PBRGeometryShaderProgramObject, "u_lightingEnable");
	pbrGeometryUniform.lightColorUniform = glGetUniformLocation(PBRGeometryShaderProgramObject, "lightColors");
	
	pbrGeometryUniform.textureSamplerUniform_albedo = glGetUniformLocation(PBRGeometryShaderProgramObject, "albedoMap");
	pbrGeometryUniform.textureSamplerUniform_normal = glGetUniformLocation(PBRGeometryShaderProgramObject, "normalMap");
	pbrGeometryUniform.textureSamplerUniform_metallic = glGetUniformLocation(PBRGeometryShaderProgramObject, "metallicMap");
	pbrGeometryUniform.textureSamplerUniform_roughness = glGetUniformLocation(PBRGeometryShaderProgramObject, "roughnessMap");
	pbrGeometryUniform.textureSamplerUniform_ao = glGetUniformLocation(PBRGeometryShaderProgramObject, "aoMap");
	
	pbrGeometryUniform.cameraPositionUniform = glGetUniformLocation(PBRGeometryShaderProgramObject, "camPos");


	glUseProgram(PBRGeometryShaderProgramObject);
	//glUniform1i(pbrGeometryUniform.textureSamplerUniform, 0);
	
	glUniform1i(pbrGeometryUniform.textureSamplerUniform_albedo, 0);
	glUniform1i(pbrGeometryUniform.textureSamplerUniform_normal, 1);
	glUniform1i(pbrGeometryUniform.textureSamplerUniform_metallic, 2);
	glUniform1i(pbrGeometryUniform.textureSamplerUniform_roughness, 3);
	glUniform1i(pbrGeometryUniform.textureSamplerUniform_ao, 4);

	
	glUseProgram(0);


	return(0);

}

struct PBRGeometryUniform usePBRGeometryShader(void)
{

	// Code
	glUseProgram(PBRGeometryShaderProgramObject);
	return pbrGeometryUniform;

}

void uninitializePBRGeometryShader(void)
{

	// Code
	// Shader Uninitialization
	if (PBRGeometryShaderProgramObject) {

		GLsizei numAttachedShader;
		glUseProgram(PBRGeometryShaderProgramObject);
		glGetProgramiv(PBRGeometryShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(PBRGeometryShaderProgramObject, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {

			glDetachShader(PBRGeometryShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(PBRGeometryShaderProgramObject);
		PBRGeometryShaderProgramObject = 0;

	}

}



