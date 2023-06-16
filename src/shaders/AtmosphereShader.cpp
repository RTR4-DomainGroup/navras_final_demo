#include "../../inc/shaders/AtmosphereShader.h"

GLuint shaderProgramObj_atmos;

struct AtmosphereUniform atmosUniform;

int initializeAtmosphereShader(void)
{
    // Code
    // Vertex Shader
    const GLchar* vertexShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "in vec4 a_position; \n" \
        "in vec4 a_color; \n" \

        "uniform vec3 v3CameraPos; \n" \
        "uniform vec3 v3LightPos; \n" \
        "uniform vec3 v3InvWavelength; \n" \
        "uniform float fCameraHeight; \n" \
        "uniform float fCameraHeight2; \n" \
        "uniform float fOuterRadius; \n" \
        "uniform float fOuterRadius2; \n" \
        "uniform float fInnerRadius; \n" \
        "uniform float fInnerRadius2; \n" \
        "uniform float fKrESun; \n" \
        "uniform float fKmESun; \n" \
        "uniform float fKr4PI; \n" \
        "uniform float fKm4PI; \n" \
        "uniform float fScale; \n" \
        "uniform float fScaleDepth; \n" \
        "uniform float fScaleOverScaleDepth; \n" \

        "uniform mat4 u_modelMatrix; \n" \
        "uniform mat4 u_viewMatrix; \n" \
        "uniform mat4 u_projectionMatrix; \n" \

        "const int nSamples = 2; \n" \
        "const float fSamples = 2.0; \n" \

        "out vec3 v3Direction; \n" \

        "out vec3 vColor; \n" \
        "out vec3 vSecondaryColor; \n" \

        "float scale(float fCos) \n" \
        "{ \n" \
        "float x = 1.0 - fCos; \n" \
            "return fScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25)))); \n" \
        "} \n" \

        "void main(void) \n" \
        "{ \n" \

            "vec3 v3Pos = a_position.xyz; \n" \
            "vec3 v3Ray = v3Pos - v3CameraPos; \n" \
            "float fFar = length(v3Ray); \n" \
            "v3Ray /= fFar; \n" \

            "vec3 v3Start = v3CameraPos; \n" \
            "float fHeight = length(v3Start); \n" \
            "float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight)); \n" \
            "float fStartAngle = dot(v3Ray, v3Start) / fHeight; \n" \
            "float fStartOffset = fDepth*scale(fStartAngle); \n" \

            "float fSampleLength = fFar / fSamples; \n" \
            "float fScaledLength = fSampleLength * fScale; \n" \
            "vec3 v3SampleRay = v3Ray * fSampleLength; \n" \
            "vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5; \n" \

            "vec3 v3FrontColor = vec3(0.0, 0.0, 0.0); \n" \
            "for(int i=0; i<nSamples; i++){ \n" \
                "float fHeight = length(v3SamplePoint); \n" \
                "float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight)); \n" \
                "float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight; \n" \
                "float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight; \n" \
                "float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle))); \n" \
                "vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI)); \n" \
                "v3FrontColor += v3Attenuate * (fDepth * fScaledLength); \n" \

                "v3SamplePoint += v3SampleRay; \n" \
            "} \n" \

            "vSecondaryColor = v3FrontColor * fKmESun; \n" \
            "vColor = v3FrontColor * (v3InvWavelength * fKrESun); \n" \
            "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position; \n" \
            "v3Direction = v3CameraPos - v3Pos; \n" \

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
                LOG("God Rays Shader Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeAtmosphereShader();
                return(-1);
            }
        }
    }
    #pragma endregion

    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "uniform vec3 v3LightPos; \n" \
        "uniform float g; \n" \
        "uniform float g2; \n" \
        "in vec3 v3Direction; \n" \

        "in vec3 vColor; \n" \
        "in vec3 vSecondaryColor; \n" \

        "out vec4 FragColor; \n" \
        "void main(void) \n" \
        "{ \n" \
            "float fCos = dot(v3LightPos, v3Direction) / length(v3Direction); \n" \
            "float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5); \n" \
            "FragColor = vec4(vColor,  1.0) + fMiePhase * vec4(vSecondaryColor, 1.0); \n" \
            "if(FragColor.rgb == vec3(0.0, 0.0, 0.0)) discard; \n" \
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
                LOG("Fragment Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeAtmosphereShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    shaderProgramObj_atmos = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgramObj_atmos, vertexShaderObj);
    glAttachShader(shaderProgramObj_atmos, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(shaderProgramObj_atmos, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    glBindAttribLocation(shaderProgramObj_atmos, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

    // Link the program
    glLinkProgram(shaderProgramObj_atmos);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObj_atmos, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObj_atmos, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgramObj_atmos, infoLogLength, &written, log);
                LOG("Shader Program Link Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeAtmosphereShader();
                return(-1);
            }
        }
    }

    atmosUniform.modelMatrix = glGetUniformLocation(shaderProgramObj_atmos, "u_modelMatrix");
    atmosUniform.viewMatrix = glGetUniformLocation(shaderProgramObj_atmos, "u_viewMatrix");
    atmosUniform.projectionMatrix = glGetUniformLocation(shaderProgramObj_atmos, "u_projectionMatrix");
    atmosUniform.cameraPosUniform = glGetUniformLocation(shaderProgramObj_atmos, "v3CameraPos");
    atmosUniform.lightPosUniform = glGetUniformLocation(shaderProgramObj_atmos, "v3LightPos");
    atmosUniform.invWavelengthUniform = glGetUniformLocation(shaderProgramObj_atmos, "v3InvWavelength");
    atmosUniform.cameraHeightUniform = glGetUniformLocation(shaderProgramObj_atmos, "fCameraHeight");
    atmosUniform.cameraHeight2Uniform = glGetUniformLocation(shaderProgramObj_atmos, "fCameraHeight2");
    atmosUniform.innerRadiusUniform = glGetUniformLocation(shaderProgramObj_atmos, "fInnerRadius");
    atmosUniform.innerRadius2Uniform = glGetUniformLocation(shaderProgramObj_atmos, "fInnerRadius2");
    atmosUniform.outerRadiusUniform = glGetUniformLocation(shaderProgramObj_atmos, "fOuterRadius");
    atmosUniform.outerRadius2Uniform = glGetUniformLocation(shaderProgramObj_atmos, "fOuterRadius2");
    atmosUniform.KrESunUniform = glGetUniformLocation(shaderProgramObj_atmos, "fKrESun");
    atmosUniform.KmESunUniform = glGetUniformLocation(shaderProgramObj_atmos, "fKmESun");
    atmosUniform.Kr4PIUniform = glGetUniformLocation(shaderProgramObj_atmos, "fKr4PI");
    atmosUniform.Km4PIUniform = glGetUniformLocation(shaderProgramObj_atmos, "fKm4PI");
    atmosUniform.scaleUniform = glGetUniformLocation(shaderProgramObj_atmos, "fScale");
    atmosUniform.scaleDepthUniform = glGetUniformLocation(shaderProgramObj_atmos, "fScaleDepth");
    atmosUniform.scaleOverScaleDepthUniform = glGetUniformLocation(shaderProgramObj_atmos, "fScaleOverScaleDepth");
    atmosUniform.gUniform = glGetUniformLocation(shaderProgramObj_atmos, "g");
    atmosUniform.g2Uniform = glGetUniformLocation(shaderProgramObj_atmos, "g2");

	LOG("Shader initialized\n");
    return (0);
}

struct AtmosphereUniform useAtmosphereShader(void)
{
    glUseProgram(shaderProgramObj_atmos);
    return atmosUniform;
}

void uninitializeAtmosphereShader(void)
{
    if (shaderProgramObj_atmos)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_atmos);
		glGetProgramiv(shaderProgramObj_atmos, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_atmos, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_atmos, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_atmos);
		shaderProgramObj_atmos = 0;
    }
}
