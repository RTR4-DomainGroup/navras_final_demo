#include "../../inc/shaders/FSVQuadShader.h"

GLuint fsvQuadShaderProgramObject;
struct FSVQuadUniform fsvQuadUniform;

int initializeFSVQuadShader(void)
{
    // Code
    ZeroMemory((void*)&fsvQuadUniform, sizeof(FSVQuadUniform));
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
                uninitializeFSVQuadShader();
                return(-1);
            }
        }
    }
LOG("fragmentShaderSrcCode in initializeFSQuadShader().\n");
    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
        "#version 460 core" \
        "\n" \
        "in vec2 a_texcoord_out;" \
        "\n" \
        "uniform sampler2D u_textureSampler0;" \
        "\n" \
        "uniform sampler2D u_textureSampler1;" \
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
            "FragColor = texture(u_textureSampler0, a_texcoord_out) + texture(u_textureSampler1, a_texcoord_out);" \
          /*"float depthValue = texture(u_textureSampler0, a_texcoord_out).r;" \
            "FragColor = vec4(vec3(LinearizeDepth(depthValue) / 100.0), 1.0); \n" \*/
            //"FragColor = vec4(vec3(depthValue), 1.0); \n" \/
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
                LOG("FSQuad Fragment Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeFSVQuadShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    fsvQuadShaderProgramObject = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(fsvQuadShaderProgramObject, vertexShaderObj);
    glAttachShader(fsvQuadShaderProgramObject, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(fsvQuadShaderProgramObject, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    glBindAttribLocation(fsvQuadShaderProgramObject, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

    // Link the program
    glLinkProgram(fsvQuadShaderProgramObject);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(fsvQuadShaderProgramObject, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(fsvQuadShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(fsvQuadShaderProgramObject, infoLogLength, &written, log);
                LOG("FSQuad Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeFSVQuadShader();
                return(-1);
            }
        }
    }

    fsvQuadUniform.textureSamplerUniform1 = glGetUniformLocation(
        fsvQuadShaderProgramObject, "u_textureSampler0");
    fsvQuadUniform.textureSamplerUniform2 = glGetUniformLocation(
        fsvQuadShaderProgramObject, "u_textureSampler1");
	LOG("Shader initialized\n");

    return 0;
}

struct FSVQuadUniform useFSVQuadShader(void)
{
    glUseProgram(fsvQuadShaderProgramObject);
    return fsvQuadUniform;
}

void uninitializeFSVQuadShader(void)
{
    // Code
    if (fsvQuadShaderProgramObject)
    {
        GLsizei numAttachedShader;
		glUseProgram(fsvQuadShaderProgramObject);
		glGetProgramiv(fsvQuadShaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(fsvQuadShaderProgramObject, 
        numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(fsvQuadShaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(fsvQuadShaderProgramObject);
		fsvQuadShaderProgramObject = 0;
    }
}
