#include "../../inc/shaders/TerrainShader.h"

GLuint shaderProgramObj_terrain;
extern FILE* gpFile;

struct TerrainUniform terrainShaderUniform;

int initializeTerrainShader(void)
{
    // Code
    // Vertex Shader
    const GLchar* vertexShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
        "out VS_OUT"
        "{ \n" \
            "out vec2 tc; \n" \
        "} vs_out; \n" \
        "void main(void) \n" \
        "{ \n" \
            "vec4 vertices[] = vec4[](vec4(-0.5, 0.0, -0.5, 1.0), vec4(0.5, 0.0, -0.5, 1.0), vec4(-0.5, 0.0, 0.5, 1.0), vec4(0.5, 0.0, 0.5, 1.0)); \n" \
            "int x = gl_InstanceID & 63; \n" \
            "int y = gl_InstanceID >> 6; \n" \
            "vec2 offs = vec2(x, y); \n" \
            "vs_out.tc = (vertices[gl_VertexID].xz + offs + vec2(0.5)) / 64.0; \n" \
            "gl_Position = vertices[gl_VertexID] + vec4(float(x - 32), 0.0,	float(y - 32), 0.0); \n" \
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
                LOG("Terrain Shader Vertex Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeTerrainShader();
            }
        }
    }
    #pragma endregion


    const GLchar* tessallationControlShaderSourcecode =
        "#version 460 core \n" \
        "\n" \
        "layout(vertices = 4) out; \n" \
        "in VS_OUT \n" \
        "{ \n" \
            "vec2 tc; \n" \
        "} tcs_in[]; \n" \
        "out TCS_OUT \n" \
        "{ \n" \
            "vec2 tc; \n" \
        "} tcs_out[]; \n" \
        "uniform mat4 mvp_matrix; \n" \
        "void main(void) \n" \
        "{ \n" \
            "if (gl_InvocationID == 0) \n" \
            "{ \n"
                "vec4 p0 = mvp_matrix * gl_in[0].gl_Position; \n" \
                "vec4 p1 = mvp_matrix * gl_in[1].gl_Position; \n" \
                "vec4 p2 = mvp_matrix * gl_in[2].gl_Position; \n" \
                "vec4 p3 = mvp_matrix * gl_in[3].gl_Position; \n" \
                "p0 /= p0.w; \n" \
                "p1 /= p1.w; \n" \
                "p2 /= p2.w; \n" \
                "p3 /= p3.w; \n" \
                "if (p0.z <= 0.0 ||	p1.z <= 0.0 || p2.z <= 0.0 || p3.z <= 0.0) \n" \
                "{  \n" \
                    "gl_TessLevelOuter[0] = 0.0; \n" \
                    "gl_TessLevelOuter[1] = 0.0; \n" \
                    "gl_TessLevelOuter[2] = 0.0; \n" \
                    "gl_TessLevelOuter[3] = 0.0; \n" \
                "} \n" \
                "else \n" \
                "{ \n" \
                    "float l0 = length(p2.xy - p0.xy) * (16.0 + 1.0); \n" \
                    "float l1 = length(p3.xy - p2.xy) * (16.0 + 1.0); \n" \
                    "float l2 = length(p3.xy - p1.xy) * (16.0 + 1.0); \n" \
                    "float l3 = length(p1.xy - p0.xy) * (16.0 + 1.0); \n" \
                    "gl_TessLevelOuter[0] = 4.0; \n" \
                    "gl_TessLevelOuter[1] = 4.0; \n" \
                    "gl_TessLevelOuter[2] = 4.0; \n" \
                    "gl_TessLevelOuter[3] = 4.0; \n" \
                    "gl_TessLevelInner[0] = 2.0; \n" \
                    "gl_TessLevelInner[1] = 2.0; \n" \
                "} \n" \
            "} \n" \
            "gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; \n" \
            "tcs_out[gl_InvocationID].tc = tcs_in[gl_InvocationID].tc; \n" \
        "} \n";

    // Create the Vertex Shader object.
    GLuint tessallationControlShadderObject = glCreateShader(GL_TESS_CONTROL_SHADER);

    // Give the shader source to shader object.
    // Actually 3rd parameter is array if you have multiple shader source code
    // However, we have only one source code string
    glShaderSource(tessallationControlShadderObject, 1, (const GLchar**)&tessallationControlShaderSourcecode, NULL);

    // Compile the Shader source code for GPU format
    glCompileShader(tessallationControlShadderObject);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(tessallationControlShadderObject, GL_COMPILE_STATUS, &status);

#pragma region 
    // If there is an error
    if (status == GL_FALSE)
    {
        glGetShaderiv(tessallationControlShadderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char*)malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tessallationControlShadderObject, infoLogLength, &written, log);
                LOG("Terrain Shader TC Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeTerrainShader();
            }
        }
    }
#pragma endregion

    const GLchar* tessallationEvaluationShaderSourcecode =
        "#version 460 core \n" \
        "\n" \
        "layout (quads, fractional_odd_spacing) in; \n" \
        "uniform sampler2D tex_displacement; \n" \
        "uniform mat4 mv_matrix; \n" \
        "uniform mat4 mvp_matrix; \n" \
        "uniform mat4 proj_matrix; \n" \
        "uniform float displacementmap_depth; \n" \
        "in TCS_OUT \n" \
        "{ \n" \
            "vec2 tc; \n" \
        "} tes_in[]; \n" \
        "out TES_OUT \n" \
        "{ \n" \
            "vec2 tc; \n" \
            "vec3 world_coord; \n" \
            "vec3 eye_coord; \n" \
        "} tes_out; \n" \
        "void main(void) \n" \
        "{ \n" \
            "vec2 tc1 = mix(tes_in[0].tc, tes_in[1].tc, gl_TessCoord.x); \n" \
            "vec2 tc2 = mix(tes_in[2].tc, tes_in[3].tc, gl_TessCoord.x); \n" \
            "vec2 tc = mix(tc2, tc1, gl_TessCoord.y); \n" \
            "vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x); \n" \
            "vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x); \n" \
            "vec4 p = mix(p2, p1, gl_TessCoord.y); \n" \
            "p.y += texture(tex_displacement, tc).r * displacementmap_depth; \n" \
            "vec4 P_eye = mv_matrix * p; \n" \
            "tes_out.tc = tc; \n" \
            "tes_out.world_coord = p.xyz; \n" \
            "tes_out.eye_coord = P_eye.xyz; \n" \
            "gl_Position = mvp_matrix * p; \n" \
        "} \n";

    // Create the Vertex Shader object.
    GLuint tessallationEvaluationShadderObject = glCreateShader(GL_TESS_EVALUATION_SHADER);

    // Give the shader source to shader object.
    // Actually 3rd parameter is array if you have multiple shader source code
    // However, we have only one source code string
    glShaderSource(tessallationEvaluationShadderObject, 1, (const GLchar**)&tessallationEvaluationShaderSourcecode, NULL);

    // Compile the Shader source code for GPU format
    glCompileShader(tessallationEvaluationShadderObject);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetShaderiv(tessallationEvaluationShadderObject, GL_COMPILE_STATUS, &status);

#pragma region 
    // If there is an error
    if (status == GL_FALSE)
    {
        glGetShaderiv(tessallationEvaluationShadderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
        if (infoLogLength > 0)
        {
            log = (char*)malloc(infoLogLength);
            if (log != NULL)
            {
                GLsizei written;
                glGetShaderInfoLog(tessallationEvaluationShadderObject, infoLogLength, &written, log);
                LOG("Terrain Shader TE Shader Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeTerrainShader();
            }
        }
    }
#pragma endregion


    // Fragement Shader
    const GLchar* fragmentShaderSrcCode = 
        "#version 460 core \n" \
        "\n" \
            "layout (binding = 1) uniform sampler2D tex_color; \n" \
            "uniform bool enable_fog = true; \n" \
            "uniform vec4 fog_color = vec4(0.7, 0.8, 0.9, 0.0); \n" \
        "in TES_OUT \n" \
        "{ \n" \
            "vec2 tc; \n" \
            "vec3 world_coord; \n" \
            "vec3 eye_coord; \n" \
        "} fs_in; \n" \
        "vec4 fog(vec4 c) \n" \
        "{ \n" \
            "float z = length(fs_in.eye_coord); \n" \
            "float de = 0.025 * smoothstep(0.0, 6.0, 10.0 - fs_in.world_coord.y); \n" \
            "float di = 0.045 * (smoothstep(0.0, 40.0, 20.0 - fs_in.world_coord.y)); \n" \
            "float extinction = exp(-z * de); \n" \
            "float inscattering = exp(-z * di); \n" \
            "return c * extinction + fog_color * (1.0 - inscattering); \n" \
        "} \n" \
        "out vec4 FragColor; \n" \
        "void main(void) \n" \
        "{ \n" \
            "vec4 landscape = texture(tex_color, fs_in.tc); \n" \
            "if (enable_fog) \n" \
            "{ \n" \
             "FragColor = fog(landscape); \n" \
            "} \n" \
            "else \n" \
            "{ \n" \
                "FragColor = landscape; \n" \
            "} \n" \
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
                LOG("Fragment Shader Terrain Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeTerrainShader();
            }
        }
    }

    // Shader Program Object
    shaderProgramObj_terrain = glCreateProgram();
    
    // Attach desired shader object to the program object
    glAttachShader(shaderProgramObj_terrain, vertexShaderObj);
    glAttachShader(shaderProgramObj_terrain, tessallationControlShadderObject);
    glAttachShader(shaderProgramObj_terrain, tessallationEvaluationShadderObject);
    glAttachShader(shaderProgramObj_terrain, fragementShaderObj);

    // Pre-linked binding of Shader program object
    //glBindAttribLocation(shaderProgramObj_terrain, DOMAIN_ATTRIBUTE_POSITION, "a_position");
    //glBindAttribLocation(shaderProgramObj_terrain, DOMAIN_ATTRIBUTE_TEXTURE0, "a_texcoord");

    // Link the program
    glLinkProgram(shaderProgramObj_terrain);

    status = 0;
    infoLogLength = 0;
    log = NULL;

    glGetProgramiv(shaderProgramObj_terrain, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        glGetProgramiv(shaderProgramObj_terrain, GL_INFO_LOG_LENGTH, &infoLogLength);

        if (infoLogLength > 0)
        {
            log = (char*) malloc(infoLogLength);

            if (log != NULL)
            {
                GLsizei written;

                glGetProgramInfoLog(shaderProgramObj_terrain, infoLogLength, &written, log);
                LOG("Shader Program Link Log: %s\n", log);
                free(log);
                uninitializeTerrainShader();
            }
        }
    }

    terrainShaderUniform.uniform_mv_matrix = glGetUniformLocation(shaderProgramObj_terrain, "mv_matrix");
    terrainShaderUniform.uniform_mvp_matrix = glGetUniformLocation(shaderProgramObj_terrain, "mvp_matrix");
    terrainShaderUniform.uniform_proj_matrix = glGetUniformLocation(shaderProgramObj_terrain, "proj_matrix");
    terrainShaderUniform.uniform_dmap_depth = glGetUniformLocation(shaderProgramObj_terrain, "displacementmap_depth");
    terrainShaderUniform.uniform_enable_fog = glGetUniformLocation(shaderProgramObj_terrain, "enable_fog");
    terrainShaderUniform.textureSamplerUniform1 = glGetUniformLocation(shaderProgramObj_terrain, "tex_displacement");
    terrainShaderUniform.textureSamplerUniform2 = glGetUniformLocation(shaderProgramObj_terrain, "tex_color");

    glUseProgram(shaderProgramObj_terrain);
    glUniform1i(terrainShaderUniform.textureSamplerUniform1, 0);
    glUniform1i(terrainShaderUniform.textureSamplerUniform2, 1);
    glUseProgram(0);

    return (0);
}

struct TerrainUniform useTerrainShader(void)
{
    glUseProgram(shaderProgramObj_terrain);
    return terrainShaderUniform;
}

void uninitializeTerrainShader(void)
{
    if (shaderProgramObj_terrain)
    {
        GLsizei numAttachedShader;
		glUseProgram(shaderProgramObj_terrain);
		glGetProgramiv(shaderProgramObj_terrain, GL_ATTACHED_SHADERS, &numAttachedShader);
		GLuint* shaderObjects = NULL;
		shaderObjects = (GLuint*)malloc(numAttachedShader * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObj_terrain, numAttachedShader, &numAttachedShader, shaderObjects);
		for (GLsizei i = 0; i < numAttachedShader; i++) {
		
			glDetachShader(shaderProgramObj_terrain, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;

		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);
		glDeleteProgram(shaderProgramObj_terrain);
		shaderProgramObj_terrain = 0;
    }
    
}
