#include "../../inc/shaders/WaterShader.h"

GLuint shaderProgramObj_water;

struct WaterUniform waterShaderUniform;

int initializeWaterShader(void)
{
    // Code
    // Vertex Shader
    const GLchar* vertexShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "in vec4 a_position; \n" \
        "in vec2 a_texcoord; \n" \
        "out vec4 clipSpace; \n" \
        "out vec3 toCameraVector; \n" \

        "uniform mat4 u_modelMatrix; \n" \
        "uniform mat4 u_viewMatrix; \n" \
        "uniform mat4 u_projectionMatrix; \n" \

        "uniform vec3 u_cameraPosition; \n" \

        "out vec2 a_texcoord_out;\n" \

        "uniform vec4 u_plane;\n" \
        "const float tiling = 8.0;\n" \
        "void main(void) \n" \
        "{ \n" \
            "gl_ClipDistance[0] = dot(u_modelMatrix * a_position, u_plane); \n" \
            "vec4 worldPosition =  u_modelMatrix * a_position;\n" \
            "clipSpace = u_projectionMatrix * u_viewMatrix * worldPosition; \n" \
            "gl_Position = clipSpace; \n" \
            "a_texcoord_out = a_texcoord * tiling;\n" \
            "toCameraVector = u_cameraPosition - worldPosition.xyz;\n" \
            /*"a_texcoord_out = vec2(a_position.x/2.0 + 0.5, a_position.y/2.0 + 0.5) * tiling;\n" \*/
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
                LOG("WATER Shader Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeWaterShader();
                return(-1);
            }
        }
    }
    #pragma endregion


    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "in vec2 a_texcoord_out;\n" \
        "in vec4 clipSpace;\n" \
        "in vec3 toCameraVector;\n" \

        "uniform sampler2D u_texturesampler;\n" \
        "uniform sampler2D u_textureReflection;\n" \
        "uniform sampler2D u_textureRefraction;\n" \
        "uniform sampler2D dudvMap;\n" \

        "uniform float u_moveFactor; \n" \
        "uniform bool enable_godRays = true; \n" \
        "uniform float waveStrength; \n" \
        "uniform vec4 waterColor; \n" \
        "out vec4 FragColor; \n" \

        "void main(void) \n" \
        "{ \n" \
            "vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5; \n" \
            "vec2 refractTexCoords = vec2(ndc.x, ndc.y); \n" \
            "vec2 reflectTexCoords = vec2(ndc.x, -ndc.y); \n" \
            "vec3 phong_ads_light; \n" \
            "vec2 distortion1 = (texture(dudvMap, vec2(a_texcoord_out.x + u_moveFactor ,a_texcoord_out.y)).rg * 2.0 - 1.0) * waveStrength; \n" \
            "vec2 distortion2 = (texture(dudvMap, vec2(-a_texcoord_out.x + u_moveFactor ,a_texcoord_out.y+ u_moveFactor)).rg * 2.0 - 1.0) * waveStrength; \n" \

            "distortion1 = distortion1 + distortion2; \n" \

            "refractTexCoords += distortion1; \n" \
            "refractTexCoords = clamp(refractTexCoords, 0.001, 0.999); \n" \

            "reflectTexCoords += distortion1; \n" \
            "reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999); \n" \
            "reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001); \n" \

            "vec4 texture_color; \n" \
            "vec4 texture_Reflection; \n" \
            "vec4 texture_Refraction; \n" \
         
            "texture_Reflection = texture(u_textureReflection, reflectTexCoords); \n" \
            "texture_Refraction = texture(u_textureRefraction, refractTexCoords); \n" \

            "vec3 viewVector = normalize(toCameraVector); \n" \
            "float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0)); \n" \
            /*"refractiveFactor = pow(refractiveFactor, 10.0); \n" \*/

            "texture_color = mix(texture_Reflection, texture_Refraction, 0.1); \n" \
            "texture_color = mix(texture_color, waterColor, 0.2); \n" \
            "if (enable_godRays) \n" \
            "{" \
                "FragColor = texture_color; \n" \
            "}" \
            "else\n" \
            "{\n"  \
                "FragColor = vec4(0.0, 0.0, 0.0, 1.0);\n" \
            "}\n" \
        "} \n";
    
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
                LOG("Fragment Shader WATER Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeWaterShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    shaderProgramObj_water = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgramObj_water, vertexShaderObj);
    glAttachShader(shaderProgramObj_water, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(shaderProgramObj_water, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    glBindAttribLocation(shaderProgramObj_water, DOMAIN_ATTRIBUTE_NORMAL, "a_normal");
    glBindAttribLocation(shaderProgramObj_water, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

    // Link the program
    glLinkProgram(shaderProgramObj_water);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObj_water, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObj_water, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgramObj_water, infoLogLength, &written, log);
                LOG("Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeWaterShader();
                return(-1);
            }
        }
    }

    waterShaderUniform.modelMatrixUniform = glGetUniformLocation(shaderProgramObj_water, "u_modelMatrix");
    waterShaderUniform.viewMatrixUniform = glGetUniformLocation(shaderProgramObj_water, "u_viewMatrix");
    waterShaderUniform.projectionMatrixUniform = glGetUniformLocation(shaderProgramObj_water, "u_projectionMatrix");
    waterShaderUniform.reflectionTextureSamplerUniform = glGetUniformLocation(shaderProgramObj_water, "u_textureReflection");
    waterShaderUniform.refractionTextureSamplerUniform = glGetUniformLocation(shaderProgramObj_water, "u_textureRefraction");
    waterShaderUniform.dudvTextureSamplerUniform = glGetUniformLocation(shaderProgramObj_water, "dudvMap");
    waterShaderUniform.moveFactorUniform = glGetUniformLocation(shaderProgramObj_water, "u_moveFactor");
    waterShaderUniform.planeUniform = glGetUniformLocation(shaderProgramObj_water, "u_plane");
    waterShaderUniform.cameraPositionUniform = glGetUniformLocation(shaderProgramObj_water, "u_cameraPosition");
    waterShaderUniform.uniform_enable_godRays = glGetUniformLocation(shaderProgramObj_water, "enable_godRays");
    waterShaderUniform.uniform_waveStrength = glGetUniformLocation(shaderProgramObj_water, "waveStrength");
    waterShaderUniform.uniform_watercolor = glGetUniformLocation(shaderProgramObj_water, "waterColor");

    glUseProgram(shaderProgramObj_water);
    glUniform1i(waterShaderUniform.reflectionTextureSamplerUniform, 0);
    glUniform1i(waterShaderUniform.refractionTextureSamplerUniform, 1);
    glUniform1i(waterShaderUniform.dudvTextureSamplerUniform, 2);
    glUseProgram(0);

    return (0);
}

struct WaterUniform useWaterShader(void)
{
    glUseProgram(shaderProgramObj_water);
    return waterShaderUniform;
}

void uninitializeWaterShader(void)
{
    if (shaderProgramObj_water)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_water);
		glGetProgramiv(shaderProgramObj_water, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_water, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_water, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_water);
		shaderProgramObj_water = 0;
    }
    
}
