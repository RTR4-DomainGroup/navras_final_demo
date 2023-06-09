#include "../../inc/shaders/SSAOShader.h"

GLuint shaderProgramObj_ssao;


int initializeSSAOShader(void)
{
    // Code
    // Vertex Shader
    const GLchar* vertexShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "out VS_OUT	\n" \
        "{\n" \
        "	vec3 E; \n" \
        "} vs_out; \n" \
        "void main(void)\n" \
        "{\n" \
        "    const vec4 vertices[] = vec4[]( vec4(-1.0, -1.0, 0.5, 1.0), \n" \
        "                                    vec4( 1.0, -1.0, 0.5, 1.0),\n" \
        "                                    vec4(-1.0,  1.0, 0.5, 1.0),\n" \
        "                                    vec4( 1.0,  1.0, 0.5, 1.0) );\n" \
        "    gl_Position = vertices[gl_VertexID];\n" \
        "    vs_out.E = vertices[gl_VertexID].xyz; ;\n" \
        "}\n";

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
                LOG("SSAO Shader Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeSSAOShader();
                return(-1);
            }
        }
    }
    #pragma endregion


    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "uniform sampler2D sColor;												\n" \
        "uniform sampler2D sNormalDepth;										\n" \
        "																		\n" \
        "out vec4 color;														\n" \
        "																		\n" \
        "float ssao_level = 0.30;										\n" \
        "float object_level = 1.50;										\n" \
        "float ssao_radius = 50.0;										\n" \
        "bool weight_by_angle = true;									\n" \
        "uint point_count = 8;											\n" \
        "bool randomize_points = true;									\n" \
        "																		\n" \
        "uniform SAMPLE_POINTS													\n" \
        "{																		\n" \
        "	vec4 pos[256];														\n" \
        "	vec4 random_vectors[256];											\n" \
        "} points;																\n" \
        "																		\n" \
        "void main(void)														\n" \
        "{																		\n" \
        "	vec2 P = gl_FragCoord.xy / textureSize(sNormalDepth, 0);			\n" \
        "	vec4 ND = texture2D(sNormalDepth, P);	    						\n" \
        "	vec3 N = ND.xyz;													\n" \
        "	float my_depth = ND.w;												\n" \
        "																		\n" \
        "	int i;																\n" \
        "	int j;																\n" \
        "	int n;																\n" \
        "																		\n" \
        "	float occ = 0.0;													\n" \
        "	float total = 0.0;													\n" \
        "																		\n" \
        "	n = (int(gl_FragCoord.x * 7123.2315 + 125.232) *					\n" \
        "		int(gl_FragCoord.y * 3137.1519 + 234.8)) ^						\n" \
        "		int(my_depth);													\n" \
        "	vec4 v = points.random_vectors[n & 255];							\n" \
        "																		\n" \
        "	float r = (v.r + 3.0) * 0.1;										\n" \
        "	if (!randomize_points)												\n" \
        "		r = 0.5;														\n" \
        "																		\n" \
        "	for (i = 0; i < point_count; i++)									\n" \
        "	{																	\n" \
        "		vec3 dir = points.pos[i].xyz;									\n" \
        "																		\n" \
        "		if (dot(N, dir) < 0.0)											\n" \
        "			dir = -dir;													\n" \
        "																		\n" \
        "		float f = 0.0;													\n" \
        "		float z = my_depth;												\n" \
        "																		\n" \
        "		total += 4.0;													\n" \
        "																		\n" \
        "		for (j = 0; j < 4; j++)											\n" \
        "		{																\n" \
        "			f += r;														\n" \
        "			z -= dir.z * f;												\n" \
        "																		\n" \
        "			float their_depth =											\n" \
        "				textureLod(sNormalDepth,								\n" \
        "					(P + dir.xy * f * ssao_radius), 0).w;				\n" \
        "																		\n" \
        "			float d = abs(their_depth - my_depth);						\n" \
        "			d *= d;														\n" \
        "																		\n" \
        "			if ((z - their_depth) > 0.0)								\n" \
        "			{															\n" \
        "				occ += 4.0 / (1.0 + d);									\n" \
        "			}															\n" \
        "		}																\n" \
        "	}																	\n" \
        "																		\n" \
        "	float ao_amount = (1.0 - occ / total);								\n" \
        "																		\n" \
        "	vec4 object_color = texture2D(sColor, P);						    \n" \
        "																		\n" \
        "	color = object_level * object_color +								\n" \
        "		mix(vec4(0.0), vec4(ao_amount), ssao_level);					\n" \
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
                LOG("Fragment Shader SSAO Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeSSAOShader();
                return(-1);
            }
        }
    }

    // Shader Program Object
    shaderProgramObj_ssao = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgramObj_ssao, vertexShaderObj);
    glAttachShader(shaderProgramObj_ssao, fragementShaderObj);

    // Link the program
    glLinkProgram(shaderProgramObj_ssao);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObj_ssao, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObj_ssao, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgramObj_ssao, infoLogLength, &written, log);
                LOG("SSAO Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeSSAOShader();
                return(-1);
            }
        }
    }

    return (0);
}

void useSSAOShader(void)
{
    glUseProgram(shaderProgramObj_ssao);
}

void uninitializeSSAOShader(void)
{
    if (shaderProgramObj_ssao)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_ssao);
		glGetProgramiv(shaderProgramObj_ssao, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_ssao, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_ssao, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_ssao);
        shaderProgramObj_ssao = 0;
    }
    
}
