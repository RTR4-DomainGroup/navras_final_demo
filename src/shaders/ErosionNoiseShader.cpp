#include "../../inc/shaders/ErosionNoiseShader.h"

// variable declarations
GLuint erosionNoiseShaderProgramObject;

ErosionNoiseUniform erosionNoiseUniform;

int intializeErosionNoiseShader(void)
{
	// Function Declarations
	void uninitializeErosionNoiseShader(void);

	// Code
	ZeroMemory((void*)&erosionNoiseUniform, sizeof(ErosionNoiseUniform));

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
		"float dotval = max(dot(normalize(vec3(u_lightPosition - eyeCoordinates)), transformedNormals), 0.0);"
		"light_intensity = dotval * 1.5;\n" \

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
				LOG("Erosion Noise Vertex Shader Compilation Log: %s\n", log);
				free(log);
				log = NULL;
				uninitializeErosionNoiseShader();
				return(-1);
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
		"uniform vec3 u_offset;\n" \
		"out vec4 fragColor;\n" \
		"void main(void)\n" \
		"{\n" \
		"vec3 phong_ads_color;\n" \
		"vec3 ambient = u_la * u_ka;\n" \
		"vec3 normalized_transformed_normals = normalize(transformedNormals);\n" \
		"vec3 normalized_lightDirection = normalize(lightDirection);\n" \
		"vec3 diffuse = u_ld * texture(u_textureSampler, a_texcoord_out).rgb * max(dot(normalized_lightDirection, normalized_transformed_normals), 0.0);\n" \
		"vec3 reflectionVector = reflect(-normalized_lightDirection, normalized_transformed_normals);\n" \
		"vec3 normalizedViewerVector = normalize(viewerVector);\n" \
		"vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector, normalizedViewerVector), 0.0), u_materialShininess);\n"\
		"phong_ads_color = ambient + diffuse + specular;\n" \

		"vec4 noisevec = texture(u_noiseSampler, 1.2 * (vec3(0.5) + MCposition));\n" \
		"float intensity = 0.75 * (noisevec.x + noisevec.y + noisevec.z + noisevec.w);\n" \
		"intensity = 1.95 * abs(2.0 * intensity - 1.0);\n" \
		"intensity = clamp(intensity, 0.0, 1.0);\n" \
		"if (intensity < fract(0.5 - u_offset.x - u_offset.y - u_offset.z)) discard;\n" \
		"vec3 tex = texture(u_textureSampler, a_texcoord_out).rgb;\n" \
		"vec3 color = tex * phong_ads_color;\n" \
		"color *= light_intensity;\n" \
		/*"color = clamp(color, 0.0, 1.0);\n" \*/
		"fragColor = vec4(color, 1.0);\n" \
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
				LOG("Erosion Noise Fragment Shader Compilation Log: %s\n", log);
				free(log);
				uninitializeErosionNoiseShader();
				log = NULL;
				return(-1);
			}
		}
	}

	// Shader Program Object
	erosionNoiseShaderProgramObject = glCreateProgram();
	glAttachShader(erosionNoiseShaderProgramObject, vertexShaderObject);
	glAttachShader(erosionNoiseShaderProgramObject, fragmentShaderObject);
	glBindAttribLocation(erosionNoiseShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
	glBindAttribLocation(erosionNoiseShaderProgramObject, DOMAIN_ATTRIBUTE_NORMAL, "a_normal");
	glBindAttribLocation(erosionNoiseShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

	glLinkProgram(erosionNoiseShaderProgramObject);

	glGetProgramiv(erosionNoiseShaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		glGetShaderiv(erosionNoiseShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(erosionNoiseShaderProgramObject, infoLogLength, &written, log);
				LOG("Erosion Noise ShaderProgram Linking Log: %s\n", log);
				free(log);
				uninitializeErosionNoiseShader();
				return(-1);
			}
		}
	}

	//
	erosionNoiseUniform.modelMatrixUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_modelMatrix");
	erosionNoiseUniform.viewMatrixUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_viewMatrix");
	erosionNoiseUniform.projectionMatrixUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_projectionMatrix");

	erosionNoiseUniform.textureSamplerUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_textureSampler");

	erosionNoiseUniform.laUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_la");
	erosionNoiseUniform.ldUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_ld");
	erosionNoiseUniform.lsUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_ls");
	erosionNoiseUniform.lightPositionUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_lightPosition");

	erosionNoiseUniform.kaUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_ka");
	erosionNoiseUniform.kdUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_kd");
	erosionNoiseUniform.ksUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_ks");
	erosionNoiseUniform.materialShininessUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_materialShininess");

	erosionNoiseUniform.noiseSamplerUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_noiseSampler");
	erosionNoiseUniform.offsetUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_offset");

	erosionNoiseUniform.scaleUniform = glGetUniformLocation(erosionNoiseShaderProgramObject, "u_scale");
	
	//erosionNoiseUniform.uniform_enable_godRays = glGetUniformLocation(erosionNoiseShaderProgramObject, "enable_godRays");

	glUseProgram(erosionNoiseShaderProgramObject);
	// some code may come here
	//glUniform1i(erosionNoiseUniform.textureSamplerUniform, 0);
	//glUniform1i(erosionNoiseUniform.noiseSamplerUniform, 1);

	glUseProgram(0);

	return(0);
}

struct ErosionNoiseUniform useErosionNoiseShader(void)
{
	// code
	glUseProgram(erosionNoiseShaderProgramObject);
	return erosionNoiseUniform;
}

GLuint getErosionNoiseShaderProgramObject(void)
{
	return erosionNoiseShaderProgramObject;
}

void uninitializeErosionNoiseShader(void)
{
	// code
	// Shader Uninitialization
	if (erosionNoiseShaderProgramObject)
	{
		GLsizei numAttachedShader;
		glUseProgram(erosionNoiseShaderProgramObject);
		glGetProgramiv(erosionNoiseShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(erosionNoiseShaderProgramObject, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++)
		{
			glDetachShader(erosionNoiseShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(erosionNoiseShaderProgramObject);
		erosionNoiseShaderProgramObject = 0;
	}
}
