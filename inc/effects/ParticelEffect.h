#pragma once
#include "../shaders/ParticleShader.h"

enum
{
	/*PARTICLE_GROUP_SIZE = 1024,
	PARTICLE_GROUP_COUNT = 8192,*/
	PARTICLE_GROUP_SIZE = 1024,
	PARTICLE_GROUP_COUNT = 17824,
	PARTICLE_COUNT = (PARTICLE_GROUP_SIZE * PARTICLE_GROUP_COUNT),
	MAX_ATTRACTORS = 64
};

static inline float random_float(void);
static vmath::vec3 random_vector(float, float);
unsigned int app_time();

int initializeParticle(void);
void displayParticle(void);
void uninitializeParticle(void);
