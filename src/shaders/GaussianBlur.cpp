#include "../../inc/shaders/GaussianBlurShader.h"

GaussianBlurUniform gaussianBlurUniform;
GLuint shaderProgram_gaussianBlur;

int initialize_gaussianBlur(void)
{
    const GLchar* vertexShader_SrcCode = 
       "#version 460 core" \
        "\n" \
        "in vec4 a_position;" \
        "\n" \
        "in vec2 a_texcoord; \n" \
        "\n" \
        "out vec2 a_texcoord_out;\n" \
        "void main()" \
        "\n" \
        "{"
            "\n" \
            "gl_Position = a_position;" \
            "\n" \
            "a_texcoord_out = a_texcoord;" \
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
                LOG("Horrizontal Blur Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitialize_gaussianBlurShader();
                return(-1);
            }
        }
    }

    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
       "#version 460 core" \
        "\n" \
        "in vec2 a_texcoord_out;" \
        "\n" \
        "out vec4 FragColor;" \
        "\n" \
        "uniform sampler2D u_blurTexture;" \
        "\n" \
        "uniform vec2 u_textureSize;" \
        "\n" \
        "uniform float u_time;" \
        "\n" \
        // Precomputed kernel size
        "const int kernelSize = 11;" \
        "\n" \
        // Gaussian kernel: pre-computed weights for a kernel size of 11
        "float kernel[11] = float[11](0.001328, 0.004787, 0.012747, 0.026994, 0.044368, 0.057669, 0.057669, 0.044368, 0.026994, 0.012747, 0.004787);" \
        "\n" \
        "void main()" \
        "\n" \
        "{" \
            "\n" \
            // Get the current pixel color
            "vec4 currentColor = texture(u_blurTexture, a_texcoord_out);" \
            // Compute the offset for each pixel convolution
            "vec2 texelSize = 1.0 / u_textureSize;" \
            "\n" \
            "vec2 offset[kernelSize];" \
            "\n" \
            "for (int i = 0; i < kernelSize; i++)" \
            "{" \
                "\n" \
                "offset[i] = vec2(float(i - kernelSize / 2) * texelSize.x, float(i - kernelSize / 2) * texelSize.y);" \
                "\n" \
            "}" \
            "\n" \
            // Perform the Gaussian blur using the precomputed kernel
            "vec4 blurColor = vec4(0.0);" \
            "for (int i = 0; i < kernelSize; i++)" \
            "{" \
                "\n" \
                "for (int j = 0; j < kernelSize; j++)" \
                "\n" \
                "{" \
                    // Sample texture at each offset and apply a weight according to the corresponding kernel value
                    "\n" \
                    "blurColor += texture(u_blurTexture, a_texcoord_out + offset[i] + offset[j]) * kernel[i] * kernel[j];" \
                    "\n" \
                "}" \
                "\n" \
            "}" \
            // Compute the transition weight using time
            "\n" \
            "float weight = abs(sin(u_time + length(a_texcoord_out - vec2(0.5)) * 2.0));" \
            "\n" \
            // Mix the current and blurred colors using the transition weight
            "\n" \
            "FragColor = mix(currentColor, blurColor, weight);" \
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
                LOG("Horrizontal Blur Fragment Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitialize_gaussianBlurShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    shaderProgram_gaussianBlur = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgram_gaussianBlur, vertexShader_Obj);
    glAttachShader(shaderProgram_gaussianBlur, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(shaderProgram_gaussianBlur, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    glBindAttribLocation(shaderProgram_gaussianBlur, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");
    // Link the program
    glLinkProgram(shaderProgram_gaussianBlur);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgram_gaussianBlur, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgram_gaussianBlur, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgram_gaussianBlur, infoLogLength, &written, log);
                LOG("Horrizontal Blur Shader Program Link Log: %s\n", log);
                free(log);
                log = NULL;
                return(-1);
            }
        }
    }
    gaussianBlurUniform.time = glGetUniformLocation(shaderProgram_gaussianBlur, "u_time");
    gaussianBlurUniform.hblurTexSamplerUniform = glGetUniformLocation(shaderProgram_gaussianBlur,"u_blurTexture");
    gaussianBlurUniform.resolution = glGetUniformLocation(shaderProgram_gaussianBlur,"u_textureSize");
    return (0);
}

GaussianBlurUniform useGaussianBlurShader(void)
{
    glUseProgram(shaderProgram_gaussianBlur);
    return gaussianBlurUniform;
}

void uninitialize_gaussianBlurShader(void)
{
    if (shaderProgram_gaussianBlur)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgram_gaussianBlur);
		glGetProgramiv(shaderProgram_gaussianBlur, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgram_gaussianBlur, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgram_gaussianBlur, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgram_gaussianBlur);
		shaderProgram_gaussianBlur = 0;
    }
}
