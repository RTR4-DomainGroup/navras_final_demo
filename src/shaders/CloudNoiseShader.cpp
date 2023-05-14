#include "../../inc/shaders/CloudNoiseShader.h"

// variable declarations
GLuint cloudNoiseShaderProgramObject;

CloudNoiseUniform cloudNoiseUniform;

extern bool cloudErodeToggle;

int intializeCloudNoiseShader(void)
{
	// Function Declarations
	void uninitializeCloudNoiseShader(void);

	// Code
	ZeroMemory((void*)&cloudNoiseUniform, sizeof(CloudNoiseUniform));

	// Vertex Shader
	const GLchar* vertexShaderSourcecode =
		"#version 460 core\n" \
	 	"\n" \
	 	"in vec4 a_position;\n" \
	 	"in vec3 a_normal;\n" \
		"in vec2 a_texcoord;\n" \
	 	"uniform mat4 u_modelMatrix;\n" \
	 	"uniform mat4 u_viewMatrix;\n" \
	 	"uniform mat4 u_projectionMatrix;\n" \
	 	"uniform vec4 u_lightPosition;\n" \
	 	"uniform float u_scale;\n" \
	 	"out float light_intensity;\n" \
	 	"out vec3 MCposition;\n" \
	 	"out vec3 transformedNormals;\n" \
	 	"out vec3 lightDirection;\n" \
	 	"out vec3 viewerVector;\n" \
		"out vec2 a_texcoord_out;\n" \
	 	"void main(void)\n" \
	 	"{\n" \
	 		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;\n" \
			
	 		"MCposition = a_position.xyz * u_scale;\n" \
			
	 		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);\n" \
	 		"transformedNormals = normalize(vec3(normalMatrix * a_normal));\n" \
	 		"lightDirection = vec3(u_lightPosition - eyeCoordinates);\n" \
	 		"viewerVector = -eyeCoordinates.xyz;\n" \
	 
	 		"light_intensity = dot(normalize(vec3(u_lightPosition - eyeCoordinates)), transformedNormals);\n" \
	 		"light_intensity *= 1.5;\n" \
	 
	 		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;\n" \
			"a_texcoord_out = a_texcoord;\n" \
	 	"}\n";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourcecode, NULL);

	glCompileShader(vertexShaderObject);

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
				LOG("Cloud Noise Vertex Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeCloudNoiseShader();
			}
		}
	}

	// Fragment Shader
	const GLchar* fragmentShaderSourcecode =
		"#version 460 core\n" \
		"\n" \
		"in float light_intensity;\n" \
		"in vec3 MCposition;\n" \
		"in vec3 transformedNormals;\n" \
		"in vec3 lightDirection;\n" \
		"in vec3 viewerVector;\n" \
		"in vec2 a_texcoord_out;\n" \
		"uniform vec3 u_la;\n" \
		"uniform vec3 u_ld;\n" \
		"uniform vec3 u_ls;\n" \
		"uniform vec3 u_ka;\n" \
		"uniform vec3 u_kd;\n" \
		"uniform vec3 u_ks;\n" \
		"uniform float u_materialShininess;\n" \
		"uniform sampler2D u_textureSampler;\n" \
		"uniform sampler3D u_noiseSampler;\n" \
		"uniform vec3 u_skyColor;\n" \
		"uniform vec3 u_cloudColor;\n" \
		"uniform float u_noiseScale;\n" \
		"uniform bool enable_godRays = true; \n" \
		"uniform int u_cloudErodeToggle; \n" \
		"out vec4 fragColor;\n" \
		"void main(void)\n" \
		"{\n" \
			"vec3 phong_ads_color;\n" \
			"vec3 ambient = u_la * u_ka;\n" \
			"vec3 color;\n" \
			"vec3 diffuse;\n" \
			"vec3 normalized_transformed_normals = normalize(transformedNormals);\n" \
			"vec3 normalized_lightDirection = normalize(lightDirection);\n" \
			"if (u_cloudErodeToggle == 1)\n" \
			"{\n" \
				"diffuse = u_ld * u_kd * max(dot(normalized_lightDirection, normalized_transformed_normals), 0.0);\n" \
			"}\n" \
			"else\n" \
			"{\n" \
				/*"diffuse = u_ld * texture(u_textureSampler, a_texcoord_out).rgb * max(dot(normalized_lightDirection, normalized_transformed_normals), 0.0);\n" \*/
			"}\n" \
			"vec3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformed_normals);\n" \
			"vec3 normalizedViewerVector = normalize(viewerVector);\n" \
			"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalizedViewerVector), 0.0), u_materialShininess);\n" \
			"phong_ads_color = ambient + diffuse + specular;\n" \
			"if (u_cloudErodeToggle == 1)\n" \
			"{\n" \
				"vec4 noisevec = texture(u_noiseSampler, MCposition * u_noiseScale);\n" \
				"float intensity = (noisevec[0] + noisevec[1] + noisevec[2] + noisevec[3] + 0.03125) * 1.5;\n" \
				"color = mix(u_skyColor, u_cloudColor, intensity) * phong_ads_color;\n" \
			"}\n" \
			"else\n" \
			"{\n" \
				/*"vec4 noisevec = texture(u_noiseSampler, 1.2 * (vec3(0.5) + MCposition));\n" \
				"float intensity = 0.75 * (noisevec.x + noisevec.y + noisevec.z + noisevec.w);\n" \
				"intensity = 1.95 * abs(2.0 * intensity - 1.0);\n" \
				"intensity = clamp(intensity, 0.0, 1.0);\n" \
				"if (intensity < fract(0.5 - u_offset.x - u_offset.y - u_offset.z)) discard;\n" \
				"vec3 tex = texture(u_textureSampler, a_texcoord_out).rgb;\n" \
				"color = tex * phong_ads_color;\n" \
				"color *= light_intensity;\n" \*/
			"}\n" \
			"if (enable_godRays) \n" \
			"{\n" \
				"fragColor = vec4(color, 1.0);\n" \
			"}\n"
			"else\n" \
			"{\n" \
				"fragColor = vec4(0.0, 0.0, 0.0, 1.0);\n" \
			"}\n" \
		"}\n";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourcecode, NULL);

	glCompileShader(fragmentShaderObject);

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
				LOG("Cloud Noise Fragment Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeCloudNoiseShader();
			}
		}
	}

	// Shader Program Object
	cloudNoiseShaderProgramObject = glCreateProgram();
	glAttachShader(cloudNoiseShaderProgramObject, vertexShaderObject);
	glAttachShader(cloudNoiseShaderProgramObject, fragmentShaderObject);
	glBindAttribLocation(cloudNoiseShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(cloudNoiseShaderProgramObject, DOMAIN_ATTRIBUTE_NORMAL, "a_normal");
	glBindAttribLocation(cloudNoiseShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");


	glLinkProgram(cloudNoiseShaderProgramObject);

	glGetProgramiv(cloudNoiseShaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(cloudNoiseShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) 
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(cloudNoiseShaderProgramObject, infoLogLength, &written, log);
				LOG("Cloud Noise ShaderProgram Linking Log: %s\n", log);
				free(log);
				uninitializeCloudNoiseShader();
			}
		}
	}

	//
	cloudNoiseUniform.modelMatrixUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_modelMatrix");
	cloudNoiseUniform.viewMatrixUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_viewMatrix");
	cloudNoiseUniform.projectionMatrixUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_projectionMatrix");

	cloudNoiseUniform.laUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_la");
	cloudNoiseUniform.ldUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_ld");
	cloudNoiseUniform.lsUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_ls");
	cloudNoiseUniform.lightPositionUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_lightPosition");

	cloudNoiseUniform.kaUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_ka");
	cloudNoiseUniform.kdUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_kd");
	cloudNoiseUniform.ksUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_ks");
	cloudNoiseUniform.materialShininessUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_materialShininess");

	cloudNoiseUniform.noiseSamplerUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_noiseSampler");
	cloudNoiseUniform.textureSamplerUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_textureSampler");
	cloudNoiseUniform.skyColorUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_skyColor");
	cloudNoiseUniform.cloudColorUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_cloudColor");

	cloudNoiseUniform.offsetUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_offset");
	cloudNoiseUniform.cloudErodeToggleUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_cloudErodeToggle");

	cloudNoiseUniform.scaleUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_scale");
	cloudNoiseUniform.noiseScaleUniform = glGetUniformLocation(cloudNoiseShaderProgramObject, "u_noiseScale");
	cloudNoiseUniform.uniform_enable_godRays = glGetUniformLocation(cloudNoiseShaderProgramObject, "enable_godRays");

	glUseProgram(cloudNoiseShaderProgramObject);

	if (cloudErodeToggle == true)
	{
		glUniform1i(cloudNoiseUniform.noiseSamplerUniform, 0);
	}
	else
	{
		glUniform1i(cloudNoiseUniform.textureSamplerUniform, 0);
		glUniform1i(cloudNoiseUniform.noiseSamplerUniform, 1);
	}

	glUseProgram(0);

	return(0);
}

struct CloudNoiseUniform useCloudNoiseShader(void)
{
	// code
	glUseProgram(cloudNoiseShaderProgramObject);
	return cloudNoiseUniform;
}

void uninitializeCloudNoiseShader(void)
{
	// code
	// Shader Uninitialization
	if (cloudNoiseShaderProgramObject)
	{
		GLsizei numAttachedShader;
		glUseProgram(cloudNoiseShaderProgramObject);
		glGetProgramiv(cloudNoiseShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(cloudNoiseShaderProgramObject, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++)
		{
			glDetachShader(cloudNoiseShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(cloudNoiseShaderProgramObject);
		cloudNoiseShaderProgramObject = 0;
	}
}
