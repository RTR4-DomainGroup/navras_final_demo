#include "../../inc/shaders/ParticleShader.h"

GLuint shaderProgramObj_particle;
GLuint computeShaderProgramObject;

extern mat4 perspectiveProjectionMatrix;

struct ParticleComputeUniform particleComputeShaderUniform;
struct ParticleUniform particleShaderUniform;

int initializeParticleShader(void)
{
    // Code
    // Compute Shader
    const GLchar* compute_shader_source =
        "#version 460 core \n" \
        "\n" \
        "layout(std140, binding = 0) uniform attractor_block \n" \
        "{\n" \
        "	vec4 attractor[64];\n" \
        "};\n" \
        "layout(local_size_x = 1024) in;\n" \
        "layout(rgba32f, binding = 0) uniform imageBuffer velocity_buffer;\n" \
        "layout(rgba32f, binding = 1) uniform imageBuffer position_buffer;\n" \
        "uniform float dt = 1.0;\n" \
        "void main(void)\n" \
        "{\n" \
        "	vec4 vel = imageLoad(velocity_buffer, int(gl_GlobalInvocationID.x));\n" \
        "	vec4 pos = imageLoad(position_buffer, int(gl_GlobalInvocationID.x));\n" \
        "	int i;\n" \
        "	pos.xyz += vel.xyz * dt;\n" \
        "	pos.w -= 0.001 * dt;\n" \
        "	for (i = 0; i < 64; i++)\n" \
        "	{\n" \
        "		vec3 dist = (attractor[i].xyz - pos.xyz);\n" \
        "		vel.xyz += dt * dt * dt * attractor[i].w * normalize(dist) / (dot(dist, dist) + 10.0);\n" \
        "	}\n" \
        "	if (pos.w <= 0.0)\n" \
        "	{\n" \
        "		pos.xyz = -pos.xyz * 0.01;\n" \
        "		vel.xyz *= 0.01;\n" \
        "		pos.w += 1.0f;\n" \
        "	}\n" \
        "	imageStore(position_buffer, int(gl_GlobalInvocationID.x), pos);\n" \
        "	imageStore(velocity_buffer, int(gl_GlobalInvocationID.x), vel);\n" \
        "} \n";

    GLuint computeShaderObject = glCreateShader(GL_COMPUTE_SHADER);

    glShaderSource(computeShaderObject, 1, (const GLchar**)&compute_shader_source, NULL);

    glCompileShader(computeShaderObject);

    GLint status;
    GLint infoLogLength;
    char* log = NULL;

    glGetShaderiv(computeShaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {

        glGetShaderiv(computeShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {

            log = (char*)malloc(infoLogLength);
            if (log != NULL) {

                GLsizei written;
                glGetShaderInfoLog(computeShaderObject, infoLogLength, &written, log);
                LOG("Compute Shader Compilation Log: %s\n", log);
                free(log);
                uninitializeParticleShader();

            }

        }

    }

    computeShaderProgramObject = glCreateProgram();
    glAttachShader(computeShaderProgramObject, computeShaderObject);
    glLinkProgram(computeShaderProgramObject);

    glGetProgramiv(computeShaderProgramObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {

        glGetShaderiv(computeShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0) {

            log = (char*)malloc(infoLogLength);
            if (log != NULL) {

                GLsizei written;
                glGetShaderInfoLog(computeShaderProgramObject, infoLogLength, &written, log);
                LOG("Compute ShaderProgram Linking Log: %s\n", log);
                free(log);
                uninitializeParticleShader();

            }

        }

    }

    particleComputeShaderUniform.dt_location = glGetUniformLocation(computeShaderProgramObject, "dt");
    
    // Vertex Shader
    const GLchar* vertexShaderSrcCode =
        "#version 460 core \n" \
        "\n" \
        "in vec4 a_position; \n" \
        "uniform mat4 u_mvpMatrix; \n" \
        "out float intensity; \n" \
        "void main(void) \n" \
        "{ \n" \
            "intensity = a_position.w;\n"
            "gl_Position = u_mvpMatrix * vec4(a_position.xyz, 1.0);\n"
        "} \n";

    // Create the Vertex Shader object.
    GLuint vertexShaderObj = glCreateShader(GL_VERTEX_SHADER);

    // Give the shader source to shader object.
    // Actually 3rd parameter is array if you have multiple shader source code
    // However, we have only one source code string
    glShaderSource(vertexShaderObj, 1, (const GLchar **)&vertexShaderSrcCode, NULL);

    // Compile the Shader source code for GPU format
    glCompileShader(vertexShaderObj);

    status = 0;
    infoLogLength = 0;
    log = NULL;

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
                LOG("Particle Shader Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeParticleShader();
                return(-1);
            }
        }
    }
    #pragma endregion

    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "in float intensity;\n"
        "out vec4 FragColor; \n" \
        "void main(void) \n" \
        "{ \n" \
        "FragColor = mix(vec4(0.0f, 0.2f, 1.0f, 1.0f), vec4(0.2f, 0.05f, 0.0f, 1.0f), intensity); \n"
        "} \n";
    
     // Create the Fragment Shader object.
    GLuint fragementShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

    // Give the shader source to shader object.
    // Actually 3rd parameter is array if you have multiple shader source code
    // However, we have only one source code string
    glShaderSource(fragementShaderObj, 1, (const GLchar**)&fragmentShaderSrcCode, NULL);

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
                LOG("Fragment Shader Terrain Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeParticleShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    shaderProgramObj_particle = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgramObj_particle, vertexShaderObj);
    glAttachShader(shaderProgramObj_particle, fragementShaderObj);

    // Pre-linked binding of Shader program object
    glBindAttribLocation(shaderProgramObj_particle, DOMAIN_ATTRIBUTE_POSITION, "a_position");

    // Link the program
    glLinkProgram(shaderProgramObj_particle);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObj_particle, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObj_particle, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgramObj_particle, infoLogLength, &written, log);
                LOG("Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeParticleShader();
                return(-1);
            }
        }
    }

    particleShaderUniform.uniform_mvp_matrix = glGetUniformLocation(shaderProgramObj_particle, "u_mvpMatrix");

    return (0);
}

struct ParticleUniform useParticleShader(void)
{
    glUseProgram(shaderProgramObj_particle);
    return particleShaderUniform;
}

struct ParticleComputeUniform useParticleComputeShader(void)
{
    glUseProgram(computeShaderProgramObject);
    return particleComputeShaderUniform;
}

void uninitializeParticleShader(void)
{
    if (shaderProgramObj_particle)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_particle);
		glGetProgramiv(shaderProgramObj_particle, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_particle, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_particle, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_particle);
		shaderProgramObj_particle = 0;
    }
}
