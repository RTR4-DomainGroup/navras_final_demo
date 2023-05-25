#pragma once
#include "../helper/common.h"

struct ParticleComputeUniform
{
    GLint dt_location;
};

struct ParticleUniform
{
    GLint uniform_mvp_matrix;
};

int initializeParticleShader(void);
struct ParticleUniform useParticleShader(void);
struct ParticleComputeUniform useParticleComputeShader(void);
void uninitializeParticleShader(void);
