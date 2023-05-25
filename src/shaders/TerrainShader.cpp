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
        "in vec4 a_position; \n"		\
        "uniform float u_density; \n"	\
        "uniform float u_gradient; \n"	\
        "uniform mat4 mv_matrix; \n"    \
        "uniform int u_fogEnable; \n" \
        "out float visibility; \n"		\
        
        //Normal maaping structure        
        "in vec2 a_texCoord; \n" \

        "uniform vec4 u_lightPosition; \n" \
        "uniform mat4 u_modelMatrix; \n" \
        "uniform mat4 u_viewMatrix; \n" \
        "uniform mat4 u_projectionMatrix; \n" \

        "out VSNM_OUT"
        "{ \n" \
        "out vec3 lightDirection; \n" \
        "out vec3 viewerVector ; \n" \
        "out vec2 a_texCoord_out; \n" \
        "} vsnm_out; \n" \
        
        "void main(void) \n" \
        "{ \n" \

            //normal mapping
            "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position; \n" \
            "vsnm_out.lightDirection = normalize(vec3(u_lightPosition) - vec3(eyeCoordinates.xyz)); \n" \
            "vsnm_out.viewerVector = (-eyeCoordinates.xyz); \n" \

            //Terrain
            "vec4 vertices[] = vec4[](vec4(-0.5, 0.0, -0.5, 1.0), vec4(0.5, 0.0, -0.5, 1.0), vec4(-0.5, 0.0, 0.5, 1.0), vec4(0.5, 0.0, 0.5, 1.0)); \n" \
            "int x = gl_InstanceID & 63; \n" \
            "int y = gl_InstanceID >> 6; \n" \
            "vec2 offs = vec2(x, y); \n" \
            "vs_out.tc = (vertices[gl_VertexID].xz + offs + vec2(0.5)) / 64.0; \n" \
            
            //normal mapping
            "vsnm_out.a_texCoord_out = vs_out.tc; \n" \
            "gl_Position = vertices[gl_VertexID] + vec4(float(x - 32), 0.0,	float(y - 32), 0.0); \n" \

			"if (u_fogEnable == 1) \n" \
			"{ \n" \
				"vec4 positionRelativeToCamera = mv_matrix * a_position; \n"		\
				"float distance = length(positionRelativeToCamera.xyz); \n"			\
				"visibility = exp(-pow((distance * u_density), u_gradient)); \n"	\
				"visibility = clamp(visibility, 0.0f, 1.0f); \n"					\
			"} \n" \
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
        "uniform mat4 mvp_matrix; \n"   \
        "in float visibility[]; \n"     \
        "out float visibility_tc[]; \n"   \

        //normal mapping
        "in VSNM_OUT"
        "{ \n" \
            "vec3 lightDirection; \n" \
            "vec3 viewerVector ; \n" \
            "vec2 a_texCoord_out; \n" \
        "} vsnm_in[]; \n" \

        "out TCNM_OUT"
        "{ \n" \
        "out vec3 lightDirection; \n" \
        "out vec3 viewerVector ; \n" \
        "out vec2 a_texCoord_out; \n" \
        "} tcnm_out[]; \n" \

        "void main(void) \n" \
        "{ \n" \

            //normal mapping
            "tcnm_out[gl_InvocationID].lightDirection = vsnm_in[gl_InvocationID].lightDirection; \n" \
            "tcnm_out[gl_InvocationID].viewerVector   = vsnm_in[gl_InvocationID].viewerVector; \n" \
            "tcnm_out[gl_InvocationID].a_texCoord_out = vsnm_in[gl_InvocationID].a_texCoord_out; \n" \

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
                    "gl_TessLevelOuter[0] = 50.0; \n" \
                    "gl_TessLevelOuter[1] = 50.0; \n" \
                    "gl_TessLevelOuter[2] = 50.0; \n" \
                    "gl_TessLevelOuter[3] = 50.0; \n" \
                    "gl_TessLevelInner[0] = 50.0; \n" \
                    "gl_TessLevelInner[1] = 50.0; \n" \
                "} \n" \
            "} \n" \
            "gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; \n" \
            "tcs_out[gl_InvocationID].tc = tcs_in[gl_InvocationID].tc; \n" \
            "visibility_tc[gl_InvocationID] = visibility[0]; \n" \
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
        "in float visibility_tc[]; \n"   \
        "out float visibility_tes; \n"   \
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

        //normal mapping
        "uniform vec4 u_lightPosition; \n" \

        "in TCNM_OUT"
        "{ \n" \
            "vec3 lightDirection; \n" \
            "vec3 viewerVector ; \n" \
            "vec2 a_texCoord_out; \n" \
        "} tcnm_out[]; \n" \

        "out TENM_OUT"
        "{ \n" \
        "out vec3 lightDirection; \n" \
        "out vec3 viewerVector ; \n" \
        "out vec2 a_texCoord_out; \n" \
        "} tenm_out; \n" \

        // Shadow
        "uniform mat4 u_modelMatrix; \n" \
        "uniform mat4 lightSpaceMatrix; \n" \
        "out TESM_OUT{ \n" \
            "vec4 FragPos; \n" \
            "vec4 FragPosLightSpace; \n" \
        "} tesm_out; \n" \

        "uniform int u_actualScene; \n" \
        "uniform int u_depthScene; \n" \

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
            "visibility_tes = visibility_tc[0]; \n"   \

            //normal mapping
            "tenm_out.lightDirection = normalize(vec3(u_lightPosition) - vec3(P_eye.xyz)); \n" \
            "tenm_out.viewerVector   = (-P_eye.xyz); \n" \
            "tenm_out.a_texCoord_out = tc; \n" \

            // shadow
            "tesm_out.FragPos = u_modelMatrix * p; \n" \
            "tesm_out.FragPosLightSpace = lightSpaceMatrix * tesm_out.FragPos; \n" \
            "if(u_depthScene == 1) { \n" \

                "gl_Position = lightSpaceMatrix * u_modelMatrix * p; \n" \

            "} \n" \


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
                return(-1);
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
            "uniform bool enable_godRays = true; \n" \
            "uniform vec4 fog_color = vec4(0.7, 0.8, 0.9, 0.0); \n" \
            "in float visibility_tes; \n"   \
            "uniform vec4 u_skyFogColor; \n"	\
    		"uniform int u_fogEnable; \n" \
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

        //normal mapping
        "in TENM_OUT"
        "{ \n" \
            "vec3 lightDirection; \n" \
            "vec3 viewerVector ; \n" \
            "vec2 a_texCoord_out; \n" \
        "} tenm_out; \n" \

        "vec3 T ; \n" \
        "vec3 B ; \n" \
        "vec3 normalizedNormals ; \n" \

        "uniform vec3 u_la; \n" \
        "uniform vec3 u_ld; \n" \
        "uniform vec3 u_ls; \n" \

        "uniform float u_materialShininess; \n" \
        "uniform sampler2D u_textureSampler_diffuseMap; \n" \
        "uniform sampler2D u_textureSampler_normalMap; \n" \

        //GET NORMAP FROM NORMAL MAP
        "vec3 getNormalFromMap() \n" \
        "{ \n" \
        "normalizedNormals = normalize(texture(u_textureSampler_normalMap, tenm_out.a_texCoord_out).xyz * 2.0 - vec3(1.0)); \n" \

        "vec3 Q1 = dFdx(fs_in.world_coord); \n" \
        "vec3 Q2 = dFdy(fs_in.world_coord); \n" \
        "vec2 st1 = dFdx(tenm_out.a_texCoord_out); \n" \
        "vec2 st2 = dFdy(tenm_out.a_texCoord_out); \n" \

        "vec3 N = normalize(normalizedNormals); \n" \
        "T = normalize(Q1 * st2.t - Q2 * st1.t); \n" \
        "B = -normalize(cross(N, T)); \n" \
        "mat3 TBN = mat3(T, B, N); \n" \

        "return normalize(TBN * normalizedNormals); \n" \
        "} \n" \

        // Shadow
        "uniform mat4 u_modelMatrix; \n" \
        "uniform mat4 lightSpaceMatrix; \n" \
        "in TESM_OUT{ \n" \
            "vec4 FragPos; \n" \
            "vec4 FragPosLightSpace; \n" \
        "} fsm_in; \n" \

        "vec3 Normal; \n" \
        "uniform sampler2D shadowMap; \n" \
        "uniform vec4 u_lightPosition; \n" \

        "uniform int u_actualScene; \n" \
        "uniform int u_depthScene; \n" \

        "float ShadowCalculation(vec4 fragPosLightSpace) \n" \
        "{ \n" \
            // Shadow
            "Normal = mat3(transpose(inverse(u_modelMatrix))) * normalizedNormals; \n" \
            "vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w; \n" \
            "projCoords = projCoords * 0.5 + 0.5; \n" \
            "float closestDepth = texture(shadowMap, projCoords.xy).r; \n" \
            "float currentDepth = projCoords.z; \n" \
            "vec3 normal = normalize(Normal); \n" \
            "vec3 lightDir = normalize(u_lightPosition - fsm_in.FragPos).xyz; \n" \
            "float bias = max(0.025 * (1.0 - dot(normal, lightDir)), 0.005); \n" \
            "float shadow = 0.0; \n" \
            "vec2 texelSize = 1.0 / textureSize(shadowMap, 0); \n" \
            "for (int x = -1; x <= 1; ++x) \n" \
            "{ \n" \
                "for (int y = -1; y <= 1; ++y) \n" \
                "{ \n" \
                    "float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; \n" \
                    "shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0; \n" \
                "} \n" \
            "} \n" \
            "shadow /= 9.0; \n" \

            "if (projCoords.z > 1.0) \n" \
                "shadow = 0.0; \n" \

            "return shadow; \n" \
        "} \n" \

        "void main(void) \n" \
        "{ \n" \

            "vec3 phong_ads_light; \n" \

            //normal mapping
            "getNormalFromMap(); \n" \
            
            "vec3 norm_lightDirection = normalize(tenm_out.lightDirection); \n" \
            "vec3 a_lightDirection_out = normalize(vec3(dot(norm_lightDirection,T), dot(norm_lightDirection,B), dot(norm_lightDirection,normalizedNormals))); \n" \
            
            "vec3 norm_viewerVector = normalize(tenm_out.viewerVector); \n" \
            "vec3 a_eyeDirection_out = normalize(vec3(dot(norm_viewerVector,T), dot(norm_viewerVector,B), dot(norm_viewerVector,normalizedNormals))); \n" \
         
            "vec3 ambient = 0.8 * (texture(tex_color,tenm_out.a_texCoord_out).rgb); \n" \
            "vec3 normalized_viewerVector = a_eyeDirection_out; \n" \
            "vec3 normalized_lightDirection =a_lightDirection_out; \n" \

            "vec3 reflectionVector = reflect(-normalized_lightDirection,normalizedNormals); \n" \
            "vec3 diffuse_light_color =u_ld * max(dot(normalizedNormals,normalized_lightDirection),0.0) * texture(tex_color, tenm_out.a_texCoord_out).rgb; \n" \
            "vec3 specular = (max(pow(dot(reflectionVector,normalized_viewerVector),u_materialShininess),0.0)) * u_ls; \n" \
            
            // Shadow
            "float shadow = ShadowCalculation(fsm_in.FragPosLightSpace); \n" \

            "phong_ads_light = ambient + (1.0 - shadow) * (diffuse_light_color + specular); \n" \
            
            /********************************************/
            "vec4 landscape = texture(tex_color, fs_in.tc); \n" \
            "if (enable_godRays) \n" \
            "{ \n" \
                "if(u_actualScene == 1) { \n" \
                    "FragColor = landscape * vec4(phong_ads_light, 1.0); \n" \
                    "if (u_fogEnable == 1) \n" \
                    "{ \n" \
                        "FragColor = mix(u_skyFogColor, FragColor, visibility_tes); \n" \
                    "} \n" \
                "} \n" \
                "if(u_depthScene == 1) { \n" \
                    "FragColor = vec4(1.0); \n" \
                "} \n" \
            "}" \
            "else \n" \
            "{\n" \
                "FragColor = vec4(0.0f, 0.0f, 0.0f,1.0); \n" \
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
                LOG("Fragment Shader Terrain Compilation Log: %s\n", log);
                free(log);
                log = NULL;
                uninitializeTerrainShader();
                return(-1);
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
                return(-1);
            }
        }
    }

    terrainShaderUniform.uniform_mv_matrix = glGetUniformLocation(shaderProgramObj_terrain, "mv_matrix");
    terrainShaderUniform.uniform_mvp_matrix = glGetUniformLocation(shaderProgramObj_terrain, "mvp_matrix");
    terrainShaderUniform.uniform_proj_matrix = glGetUniformLocation(shaderProgramObj_terrain, "proj_matrix");
    terrainShaderUniform.uniform_dmap_depth = glGetUniformLocation(shaderProgramObj_terrain, "displacementmap_depth");
    terrainShaderUniform.uniform_enable_fog = glGetUniformLocation(shaderProgramObj_terrain, "enable_fog");
    terrainShaderUniform.uniform_enable_godRays = glGetUniformLocation(shaderProgramObj_terrain, "enable_godRays");
    terrainShaderUniform.textureSamplerUniform1 = glGetUniformLocation(shaderProgramObj_terrain, "tex_displacement");
    terrainShaderUniform.textureSamplerUniform2 = glGetUniformLocation(shaderProgramObj_terrain, "tex_color");

    terrainShaderUniform.gradientUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_gradient");
    terrainShaderUniform.densityUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_density");
    terrainShaderUniform.skyFogColorUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_skyFogColor");
    terrainShaderUniform.fogEnableUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_fogEnable");

    //normal mapping
    terrainShaderUniform.textureSamplerUniform_normal = glGetUniformLocation(shaderProgramObj_terrain, "u_textureSampler_normalMap");
    
    terrainShaderUniform.modelMatrixUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_modelMatrix");
    terrainShaderUniform.viewMatrixUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_viewMatrix");
    terrainShaderUniform.projectionMatrixUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_projectionMatrix");

    terrainShaderUniform.laUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_la");
    terrainShaderUniform.ldUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_ld");
    terrainShaderUniform.lsUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_ls");
    terrainShaderUniform.lightPositionUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_lightPosition");

    terrainShaderUniform.materialShininessUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_materialShininess");

    //shadow
    terrainShaderUniform.shadowMapSamplerUniform = glGetUniformLocation(shaderProgramObj_terrain, "shadowMap");
    terrainShaderUniform.actualSceneUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_actualScene");
    terrainShaderUniform.depthSceneUniform = glGetUniformLocation(shaderProgramObj_terrain, "u_depthScene");
    terrainShaderUniform.lightSpaceMatrixUniform = glGetUniformLocation(shaderProgramObj_terrain, "lightSpaceMatrix");

    //------------------------------------------------------------------------------------

    glUseProgram(shaderProgramObj_terrain);
    glUniform1i(terrainShaderUniform.textureSamplerUniform1, 0);
    glUniform1i(terrainShaderUniform.textureSamplerUniform2, 1);
    glUniform1i(terrainShaderUniform.textureSamplerUniform_normal, 2);
    glUniform1i(terrainShaderUniform.shadowMapSamplerUniform, 3);
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
