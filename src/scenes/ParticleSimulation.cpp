#include "../../inc/effects/ParticelEffect.h"
#include "../../inc/helper/texture_loader.h"
#include "../../inc/helper/camera.h"
#include "../../inc/helper/geometry.h"

extern mat4 perspectiveProjectionMatrix;

ParticleUniform particleUniforms;
ParticleComputeUniform particleComputeUniforms;

#define STRINGIZE(a) #a

GLuint vao_particle;
GLuint vbo_position_particle;
GLuint vbo_velocity_particle;

GLuint vbo_position_buffer;
GLuint vbo_velocity_buffer;

GLuint position_tbo;
GLuint velocity_tbo;

// Attractor UBO
GLuint  attractor_buffer;

// Mass of the attractors
float attractor_masses[MAX_ATTRACTORS];

float aspect_ratio;

float time = 0.0f;
float delta_time = 0.0f;

extern int winWidth;
extern int winHeight;

float angleXYZ = 0;

int initializeParticle(void) {
	
	// Declaration of Vertex Data Arrays
	int i;

	// VAO AND VBO RELATED CODE
	glGenVertexArrays(1, &vao_particle);
	glBindVertexArray(vao_particle);

	// Position VBO
	glGenBuffers(1, &vbo_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(vmath::vec4), NULL, GL_DYNAMIC_COPY);

	vmath::vec4* positions = (vmath::vec4*)glMapBufferRange(GL_ARRAY_BUFFER,
		0,
		PARTICLE_COUNT * sizeof(vmath::vec4),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (i = 0; i < PARTICLE_COUNT; i++)
	{
		positions[i] = vmath::vec4(random_vector(-10.0f, 10.0f), random_float());
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glVertexAttribPointer(DOMAIN_ATTRIBUTE_POSITION, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(DOMAIN_ATTRIBUTE_POSITION);

	// VELOCITY VBO
	glGenBuffers(1, &vbo_velocity_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_velocity_buffer);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_COUNT * sizeof(vmath::vec4), NULL, GL_DYNAMIC_COPY);

	vmath::vec4* velocities = (vmath::vec4*)glMapBufferRange(GL_ARRAY_BUFFER,
		0,
		PARTICLE_COUNT * sizeof(vmath::vec4),
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for (i = 0; i < PARTICLE_COUNT; i++)
	{
		velocities[i] = vmath::vec4(random_vector(-0.1f, 0.1f), 0.0f);
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &position_tbo);
	glBindTexture(GL_TEXTURE_BUFFER, position_tbo);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo_position_buffer);

	glGenTextures(1, &velocity_tbo);
	glBindTexture(GL_TEXTURE_BUFFER, velocity_tbo);
	glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, vbo_velocity_buffer);

	glGenBuffers(1, &attractor_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, attractor_buffer);
	glBufferData(GL_UNIFORM_BUFFER, 64 * sizeof(vmath::vec4), NULL, GL_STATIC_DRAW);

	for (i = 0; i < MAX_ATTRACTORS; i++)
	{
		attractor_masses[i] = 0.5f + random_float() * 0.5f;
	}

   /*for (i = 0; i < 32; i++)
    {
        attractor_masses[i*2] = -50.0f;
        attractor_masses[(i*2)+1] = 50.0f;
        attractor_masses[(i * 2)+2] = -50.0f;
        attractor_masses[(i * 2) + 3] = -50.0f;

        attractor_masses[(i * 2) + 4] = 50.0f;
        attractor_masses[(i * 2) + 5] = -50.0f;
        attractor_masses[(i * 2) + 6] = 50.0f;
        attractor_masses[(i * 2) + 7] = 50.0f;
    }*/

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, attractor_buffer);

	LOG("initializeParticle() Successful !!!\n");

	return 0;
}

void displayParticle(void) {

    //perspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)winWidth / winHeight, 0.1f, 1000.0f);

    // Code
    static const GLuint start_ticks = app_time() - 100000;
    GLuint current_ticks = app_time();
    static GLuint last_ticks = current_ticks;
    time = ((start_ticks - current_ticks) & 0xFFFFF) / float(0xFFFFF);
    delta_time = (float)(current_ticks - last_ticks) * 0.075f;

    // Compute

    if (delta_time < 0.01f)
    {
        return;
    }

    vmath::vec4* attractors = (vmath::vec4*)glMapBufferRange(GL_UNIFORM_BUFFER,
        0,
        36 * sizeof(vmath::vec4),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    int i;

    /*for (i = 0; i < 32; i++)
    {
        attractors[i] = vmath::vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,
            cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f,
            sinf(time * (float)(i + 3) * 5.3f * 20.0f) * cosf(time * (float)(i + 5) * 9.1f) * 100.0f,
            attractor_masses[i]);
    }*/

   /* float shapeParticle[] = {
    
        71,87,
        78,76,
        86,68,
        98,61,
        109,56,
        122,56,
        134,56,
        143,60,
        152,64,
        158,71,
        164,80,
        165,92,
        163,104,
        159,112,
        152,119,
        144,124,
        135,128,
        129,130,
        143,132,
        151,137,
        158,142,
        163,148,
        163,156,
        163,165,
        156,175,
        146,185,
        136,186,
        120,184,
        107,180,
        96,175,
        83,167,
        67,164,
        52,166,
        45,172,
        37,184,
        39,198,
        172,108,
        184,106,
        196,108,
        205,117,
        211,126,
        214,138,
        212,151,
        208,162,
        200,173,
        195,181,
        194,191,
        198,200,
        211,205,
        222,206,
        183,67,
        190,73,
        198,76,
        207,74,
        213,66,
        199,63

    
    };*/


    float shapeParticle[] = {

        70,86,
        82,72,
        99,60,
        118,56,
        140,60,
        157,70,
        165,88,
        161,108,
        149,122,
        129,129,
        145,134,
        159,144,
        163,162,
        153,180,
        134,186,
        113,182,
        95,175,
        79,166,
        63,164,
        44,170,
        37,182,
        39,198,
        181,106,
        199,111,
        210,124,
        213,134,
        211,154,
        204,168,
        193,184,
        197,202,
        212,206,
        227,204,
        183,65,
        192,74,
        207,73,
        198,60

    };

    float imageWidth = (260.0f / 2.0f), imageHeight = (280.0f / 2.0f);

    for(i = 0; i < 36; i++){
        
        shapeParticle[i * 2] = imageWidth - shapeParticle[i * 2];
        shapeParticle[(i * 2) + 1] = imageHeight - shapeParticle[(i * 2) + 1];

    }

    for (i = 0; i < 36; i++)
    {
        attractors[i] = vmath::vec4(shapeParticle[i * 2],
            shapeParticle[(i * 2) + 1],
            0.0f,
            attractor_masses[i]);

    }

    //for (i = 0; i < 32; i+=4)
    //{
    //    attractors[i] = vmath::vec4(500.0f,
    //        500.0f,
    //        0.0f,
    //        attractor_masses[i]);

    //    attractors[i+1] = vmath::vec4(-500.0f,
    //       500.0f,
    //        0.0f,
    //        attractor_masses[i+1]);

    //    attractors[i+2] = vmath::vec4(-500.0f,
    //        -500.0f,
    //        0.0f,
    //        attractor_masses[i+2]);

    //    attractors[i+3] = vmath::vec4(500.0f,
    //        -500.0f,
    //        0.0f,
    //        attractor_masses[i+3]);

    //   /* if (i > 3) {

    //        attractors[i] = vmath::vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,
    //            cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f,
    //            sinf(time * (float)(i + 3) * 5.3f * 20.0f) * cosf(time * (float)(i + 5) * 9.1f) * 100.0f,
    //            attractor_masses[i]);
    //    }*/


    //}

    glUnmapBuffer(GL_UNIFORM_BUFFER);

    // If dt is too large, the system could explode, so cap it to
    // some maximum allowed value
    /*if (delta_time >= 2.0f)
    {
        delta_time = 2.0f;
    }*/

    particleComputeUniforms = useParticleComputeShader();

    glBindImageTexture(0, velocity_tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glBindImageTexture(1, position_tbo, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    // Set delta time
    glUniform1f(particleComputeUniforms.dt_location, delta_time);
    // Dispatch
    glDispatchCompute(PARTICLE_GROUP_COUNT, 1, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);




    // Regular
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use The SHaderProgramObject
    particleUniforms = useParticleShader();

    // Here The Game STarts
    // Transformations
    vmath::mat4 mvp = perspectiveProjectionMatrix *
        vmath::translate(0.0f, -25.0f, -200.0f) *
        vmath::rotate(180.0f ,vmath::vec3(0.0f, 1.0f, 0.0f));
    /*angleXYZ += 1.0f;
    if(angleXYZ >= 360.0f)
        angleXYZ -= 360.0f;*/

    /*vmath::mat4 mvp = vmath::perspective(45.0f, aspect_ratio, 0.1f, 1000.0f) *
        vmath::translate(0.0f, 0.0f, -150.0f);*/

    glUniformMatrix4fv(particleUniforms.uniform_mvp_matrix, 1, GL_FALSE, mvp);

    glBindVertexArray(vao_particle);

    //glPointSize(2.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glDrawArrays(GL_POINTS, 0, PARTICLE_COUNT);

    glBindVertexArray(0);

    // Un-use ShaderProgramObject
    glUseProgram(0);

    last_ticks = current_ticks;

}

unsigned int app_time()
{
#ifdef _WIN32
	ULONGLONG currentTime = ::GetTickCount64();

	return (unsigned int)(currentTime);
#else
	return 0;
#endif
}

static inline float random_float()
{
	float res;
	unsigned int tmp;
	static unsigned int seed = 0xFFFF0C59;

	seed *= 16807;

	tmp = seed ^ (seed >> 4) ^ (seed << 15);

	*((unsigned int*)&res) = (tmp >> 9) | 0x3F800000;

	return (res - 1.0f);
}

static vmath::vec3 random_vector(float minmag = 0.0f, float maxmag = 1.0f)
{
	vmath::vec3 randomvec(random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f, random_float() * 2.0f - 1.0f);
	randomvec = normalize(randomvec);
	randomvec *= (random_float() * (maxmag - minmag) + minmag);

	return randomvec;
}

void uninitializeParticle(void) {

	if (vao_particle) {

		glDeleteVertexArrays(1, &vao_particle);
		vao_particle = 0;

	}
}
