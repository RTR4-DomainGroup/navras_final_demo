#include "../../inc/shaders/GodraysShader.h"

GLuint shaderProgramObj_godrays;
extern FILE* gpFile;

struct GodraysUniform godRaysUniform;


int initializeGodraysShader(void)
{
    // Code
    // Vertex Shader
    const GLchar* vertexShaderSrcCode = 
        "#version 460 core" \
        "\n" \
        "in vec4 a_position;" \
        "\n" \
        "in vec2 a_texcoord;" \
        "\n" \
        "out vec2 texcoord;" \
        "\n" \
        "uniform mat4 u_modelMatrix;" \
        "\n" \
        "uniform mat4 u_viewMatrix;" \
        "\n" \
        "uniform mat4 u_projectionMatrix;" \
        "\n" \
        "void main(void)" \
        "\n" \
        "{" \
            "gl_Position = a_position;" \
            "\n" \
            "texcoord = a_texcoord;" \
            "\n" \
        "}";

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
                fprintf(gpFile, "God Rays Shader Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeGodraysShader();
            }
        }
    }
    #pragma endregion

    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
        "#version 460 core" \
        "\n" \
        "const int NUM_SAMPLES = 100;" \
        "\n" \
        "in vec2 texcoord;" \
        "\n" \
        "uniform float u_exposure;" \
        "\n" \
        "uniform float u_decay;" \
        "\n" \
        "uniform float u_density;" \
        "\n" \
        "uniform float u_weight;" \
        "\n" \
        "uniform mat4 u_modelMatrix;" \
        "\n" \
        "uniform mat4 u_viewMatrix;" \
        "\n" \
        "uniform mat4 u_projectionMatrix;" \
        "\n" \
        "uniform vec4 u_lightPositionOnScreen;" \
        "\n" \
        "uniform sampler2D u_godraysampler;" \
        "\n" \
        "out vec4 FragColor;" \
        "\n" \
        "vec4 lightPosition;" \
        "\n" \
        "void main(void)" \
        "{\n" \
            "vec2 textCoo = texcoord;" \
            "\n" \
            "float illuminationDecay = 1.0;" \
            "\n" \
            
            "lightPosition =  u_projectionMatrix * u_viewMatrix * u_modelMatrix * u_lightPositionOnScreen;"
            "lightPosition /= lightPosition.w;" \
            "\n" \
            "lightPosition += vec4(1.0);" \
            "\n" \
            "lightPosition *= 0.5;" \
            "vec2 deltaTextCoord = vec2(texcoord.st - lightPosition.xy);" \
            "\n" \
            "deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * u_density;" \
            "\n"
            "vec4 myLightSample;" \
            "\n" \
            "for(int i=0; i < NUM_SAMPLES ; i++)" \
            "{\n" \
                "textCoo -= deltaTextCoord;" \
                "\n" \
                "myLightSample = texture(u_godraysampler, textCoo);" \
                "\n" \
                "myLightSample *= illuminationDecay * u_weight;" \
                "\n" \
                "FragColor += myLightSample;" \
                "\n" \
                "illuminationDecay *= u_decay;" \
                "\n" \
            "}"
            "FragColor *= u_exposure;" \
            //"FragColor = vec4(FragColor.rgb, 1.0);"
            "\n" \
        "}";
    
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
                fprintf(gpFile, "Fragment Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeGodraysShader();
            }
        }
    }

    // Shader Program Object
    shaderProgramObj_godrays = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgramObj_godrays, vertexShaderObj);
    glAttachShader(shaderProgramObj_godrays, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(shaderProgramObj_godrays, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    glBindAttribLocation(shaderProgramObj_godrays, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

    // Link the program
    glLinkProgram(shaderProgramObj_godrays);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObj_godrays, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObj_godrays, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgramObj_godrays, infoLogLength, &written, log);
                fprintf(gpFile, "Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeGodraysShader();
            }
        }
    }

    godRaysUniform.modelMatrix = glGetUniformLocation(shaderProgramObj_godrays, "u_modelMatrix");
    godRaysUniform.viewMatrix = glGetUniformLocation(shaderProgramObj_godrays, "u_viewMatrix");
    godRaysUniform.projectionMatrix = glGetUniformLocation(shaderProgramObj_godrays, "u_projectionMatrix");
    godRaysUniform.decay = glGetUniformLocation(shaderProgramObj_godrays, "u_decay");
    godRaysUniform.density = glGetUniformLocation(shaderProgramObj_godrays, "u_density");
    godRaysUniform.weight = glGetUniformLocation(shaderProgramObj_godrays, "u_weight");
    godRaysUniform.exposure = glGetUniformLocation(shaderProgramObj_godrays, "u_exposure");
    godRaysUniform.godraysampler = glGetUniformLocation(shaderProgramObj_godrays, "u_godraysampler");
    godRaysUniform.lightPositionOnScreen = glGetUniformLocation(shaderProgramObj_godrays, "u_lightPositionOnScreen");

    return (0);
}

struct GodraysUniform useGodRaysShader(void)
{
    glUseProgram(shaderProgramObj_godrays);
    return godRaysUniform;
}

void uninitializeGodraysShader(void)
{
    if (shaderProgramObj_godrays)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_godrays);
		glGetProgramiv(shaderProgramObj_godrays, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_godrays, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_godrays, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_godrays);
		shaderProgramObj_godrays = 0;
    }
    
}