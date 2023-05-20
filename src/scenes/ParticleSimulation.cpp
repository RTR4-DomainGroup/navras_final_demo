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
	glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(vmath::vec4), NULL, GL_STATIC_DRAW);

	for (i = 0; i < MAX_ATTRACTORS; i++)
	{
		attractor_masses[i] = 0.5f + random_float() * 0.5f;
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, attractor_buffer);

	LOG("initializeParticle() Successful !!!\n");

	return 0;
}

void displayParticle(void) {

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
        32 * sizeof(vmath::vec4),
        GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    int i;

    for (i = 0; i < 32; i++)
    {
        attractors[i] = vmath::vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,
            cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f,
            sinf(time * (float)(i + 3) * 5.3f * 20.0f) * cosf(time * (float)(i + 5) * 9.1f) * 100.0f,
            attractor_masses[i]);
    }

    /*for (i = 0; i < 32; i++)
    {
        attractors[0] = vmath::vec4(5.0f,
            5.0f,
            0.0f,
            attractor_masses[0]);

        attractors[1] = vmath::vec4(-5.0f,
            5.0f,
            0.0f,
            attractor_masses[1]);

        attractors[2] = vmath::vec4(-5.0f,
            -5.0f,
            0.0f,
            attractor_masses[2]);

        attractors[3] = vmath::vec4(5.0f,
            -5.0f,
            0.0f,
            attractor_masses[3]);

        if (i > 3) {

            attractors[i] = vmath::vec4(sinf(time * (float)(i + 4) * 7.5f * 20.0f) * 50.0f,
                cosf(time * (float)(i + 7) * 3.9f * 20.0f) * 50.0f,
                sinf(time * (float)(i + 3) * 5.3f * 20.0f) * cosf(time * (float)(i + 5) * 9.1f) * 100.0f,
                attractor_masses[i]);
        }


    }*/

    glUnmapBuffer(GL_UNIFORM_BUFFER);

    // If dt is too large, the system could explode, so cap it to
    // some maximum allowed value
    if (delta_time >= 2.0f)
    {
        delta_time = 2.0f;
    }

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
        vmath::translate(0.0f, 0.0f, -80.0f) *
        vmath::rotate(time * 1000.0f, vmath::vec3(0.0f, 1.0f, 0.0f));

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
