#include "../../inc/shaders/VerticalBlur.h"

GLuint shaderProgram_verticalBlur;
struct VerticalBlurUniform vBlurUniform;

int initialize_verticalBlur()
{
    const GLchar* vertexShader_SrcCode = 
       "#version 460 core" \
        "\n" \
        "in vec4 a_position;" \
        "\n" \
        "in vec3 a_normal;" \
        "\n" \
        "uniform mat4 u_modelMatrix;" \
        "\n" \
        "uniform mat4 u_viewMatrix;" \
        "\n" \
        "uniform mat4 u_projectionMatrix;" \
        "\n" \
        "uniform float u_targetHeight;" \
        "\n" \
        "out vec2 blurTextureCoords[11];\n" \
        "\n" \
        "void main(void)" \
        "\n" \
        "{\n" \
            "gl_Position = a_position;" \
            "\n" \
            "vec2 centerTexCoords = a_position.xy * 0.5 + 0.5;" \
            "\n" \
            "float pixelsize = 1.0 / u_targetHeight;" \
            "\n" \
            "for (int i = -5; i<=5; i++)" \
            "\n" \
            "{"
                "\n" \
                "blurTextureCoords[i+5] = centerTexCoords + vec2(0.0, pixelsize * i);" \
                "\n" \
            "}"
        "}\n";

    // Create the Vertex Shader object.
    GLuint vertexShader_Obj = glCreateShader(GL_VERTEX_SHADER);

    // Give the shader source to shader object.
    // Actually 3rd parameter is array if you have multiple shader source code
    // However, we have only one source code string
    glShaderSource(vertexShader_Obj, 1, (const GLchar **)&vertexShader_SrcCode, NULL);

    // Compile the Shader source code for GPU format
    glCompileShader(vertexShader_Obj);

    int status = 0;
    int infoLogLength = 0;
    char* log = NULL;

    glGetShaderiv(vertexShader_Obj, GL_COMPILE_STATUS, &status);

    // If there is an error
    if (status == GL_FALSE)
    {
        glGetShaderiv(vertexShader_Obj, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(vertexShader_Obj, infoLogLength, &written, log);
                LOG("Vertical Blur vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
            }
        }
    }

    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
       "#version 460 core" \
        "\n" \
        "in vec2 blurTextureCoords[11];\n" \
        "uniform sampler2D u_blurTexture;" \
        "\n" \
        "out vec4 FragColor;" \
        "\n" \
        "void main(void)\n" \
        "{\n" \
            "FragColor = vec4(0.0, 0.0, 0.0, 1.0);" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[0]) * 0.0093;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[1]) * 0.028002;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[2]) * 0.065984;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[3]) * 0.121703;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[4]) * 0.175713;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[5]) * 0.198596;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[6]) * 0.175713;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[7]) * 0.121703;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[8]) * 0.065984;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[9]) * 0.028002;" \
            "\n" \
            "FragColor += texture(u_blurTexture, blurTextureCoords[10]) * 0.0093;" \
        "}\n";
    
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
                LOG("Vertical blur Fragment Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
            }
        }
    }

    // Shader Program Object
    shaderProgram_verticalBlur = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgram_verticalBlur, vertexShader_Obj);
    glAttachShader(shaderProgram_verticalBlur, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(shaderProgram_verticalBlur, DOMAIN_ATTRIBUTE_POSITION, "a_position");

    // Link the program
    glLinkProgram(shaderProgram_verticalBlur);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgram_verticalBlur, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgram_verticalBlur, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgram_verticalBlur, infoLogLength, &written, log);
                LOG("Horrizontal Blur Shader Program Link Log: %s\n", log);
                free(log);
            }
        }
    }    

    vBlurUniform.targetHeight = glGetUniformLocation(shaderProgram_verticalBlur, "u_targetHeight");
    vBlurUniform.vblurTexSamplerUniform = glGetUniformLocation(shaderProgram_verticalBlur,"u_blurTexture");
    return (0);
}

VerticalBlurUniform useVerticalBlurShader(void)
{
    glUseProgram(shaderProgram_verticalBlur);
    return vBlurUniform;
}

void uninitialize_verticalBlurShader(void)
{
    if (shaderProgram_verticalBlur)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgram_verticalBlur);
		glGetProgramiv(shaderProgram_verticalBlur, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgram_verticalBlur, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgram_verticalBlur, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgram_verticalBlur);
		shaderProgram_verticalBlur = 0;
    }
}
