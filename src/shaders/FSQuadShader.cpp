#include "../../inc/shaders/FSQuadShader.h"

GLuint fsQuadShaderProgramObject;
struct FSQuadUniform fsQuadUniform;

int initializeFSQuadShader(void)
{
    // Code
    ZeroMemory((void*)&fsQuadUniform, sizeof(FSQuadUniform));
    // Vertex Shader
    const GLchar* vertexShaderSrcCode = 
        "#version 460 core" \
        "\n" \
        "in vec4 a_position;" \
        "\n" \
        "in vec2 a_texcoord;" \
        "\n" \
        "out vec2 a_texcoord_out;" \
        "\n" \
        "void main(void)" \
        "\n" \
        "{" \
            "gl_Position =  a_position;" \
            "a_texcoord_out = a_texcoord;" \
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
                LOG("FSQuad Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeFSQuadShader();
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
        "uniform float u_blurMixDelta;"
        "\n" \
        "uniform int u_singleTexture = 0; \n" \
        "uniform sampler2D u_textureSampler0;" \
        "\n" \
        "uniform sampler2D u_textureSampler1;" \
        "\n" \
        "uniform sampler2D u_textureSampler2;" \
        "\n" \
        "uniform sampler2D u_textureSampler3;" \
        "\n" \
        "uniform int maskOrFont = 0;" \
        "\n" \
        "out vec4 FragColor;" \
        "\n" \

        "float LinearizeDepth(float depth) \n" \
        "{ \n" \
        "float z = depth * 2.0 - 1.0; \n" \
        "return (2.0 * 0.1 * 100.0) / (100.0 + 0.1 - z * (100.0 - 0.1)); \n" \
        "} \n" \

        "void main(void)" \
        "{" \
            "if(u_singleTexture == 1)\n" \
            "{\n" \
                "FragColor = texture(u_textureSampler0, a_texcoord_out);\n" \
                    "if(maskOrFont == 1){\n" \
                        "if(FragColor.rgb == vec3(0.0, 0.0, 0.0)) discard;\n" \
                    "} \n" \
                    "else{ \n" \
                        "if(FragColor.rgb == vec3(0.0, 0.0, 1.0)) discard; \n" \
                    "} \n" \
             "}\n" \
            "else if(u_singleTexture == 2)\n" \
            "{\n" \
                "FragColor = ((texture(u_textureSampler0, a_texcoord_out) + texture(u_textureSampler1, a_texcoord_out)) + texture(u_textureSampler2, a_texcoord_out));" \
            "\n" \
            "}\n" \
            "else if(u_singleTexture == 3)\n" \
            "{\n" \
                "FragColor = mix(texture(u_textureSampler0, a_texcoord_out), texture(u_textureSampler1, a_texcoord_out), u_blurMixDelta);" \
            "\n" \
            "}\n" \
            "else\n" \
            "{\n" \
                "FragColor = (texture(u_textureSampler0, a_texcoord_out) + texture(u_textureSampler1, a_texcoord_out));" \
                "\n" \
            "}\n" \

            /*"float depthValue = texture(u_textureSampler0, a_texcoord_out).r;" \
            "FragColor = vec4(vec3(LinearizeDepth(depthValue) / 100.0), 1.0); \n" \*/
            //"FragColor = vec4(vec3(depthValue), 1.0); \n" \/
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
                LOG("FSQuad Fragment Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeFSQuadShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    fsQuadShaderProgramObject = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(fsQuadShaderProgramObject, vertexShaderObj);
    glAttachShader(fsQuadShaderProgramObject, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(fsQuadShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    glBindAttribLocation(fsQuadShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");
    

    // Link the program
    glLinkProgram(fsQuadShaderProgramObject);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(fsQuadShaderProgramObject, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(fsQuadShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(fsQuadShaderProgramObject, infoLogLength, &written, log);
                LOG("FSQuad Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeFSQuadShader();
                return(-1);
            }
        }
    }

    fsQuadUniform.singleTexture = glGetUniformLocation(
        fsQuadShaderProgramObject, "u_singleTexture");
    fsQuadUniform.textureSamplerUniform1 = glGetUniformLocation(
        fsQuadShaderProgramObject, "u_textureSampler0");
    fsQuadUniform.textureSamplerUniform2 = glGetUniformLocation(
        fsQuadShaderProgramObject, "u_textureSampler1");
    fsQuadUniform.textureSamplerUniform3 = glGetUniformLocation(
        fsQuadShaderProgramObject, "u_textureSampler2");
    fsQuadUniform.textureSamplerUniform4 = glGetUniformLocation(
        fsQuadShaderProgramObject, "u_textureSampler3");
    fsQuadUniform.intensity  = glGetUniformLocation(fsQuadShaderProgramObject, "u_blurMixDelta");
    fsQuadUniform.maskOrFont = glGetUniformLocation(fsQuadShaderProgramObject, "maskOrFont");

    return 0;
}

struct FSQuadUniform useFSQuadShader(void)
{
    glUseProgram(fsQuadShaderProgramObject);
    return fsQuadUniform;
}

void uninitializeFSQuadShader(void)
{
    // Code    
    if (fsQuadShaderProgramObject)
    {
        GLsizei numAttachedShader;
		glUseProgram(fsQuadShaderProgramObject);
		glGetProgramiv(fsQuadShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(fsQuadShaderProgramObject, 
        numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(fsQuadShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(fsQuadShaderProgramObject);
		fsQuadShaderProgramObject = 0;
    }
}
