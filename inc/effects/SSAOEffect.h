#pragma once
#include "../shaders/SSAOShader.h"
#include "../../inc/helper/ssaoframebuffer.h"

struct SAMPLE_POINTS
{
	vmath::vec4     point[256];
	vmath::vec4     random_vectors[256];
};

static unsigned int seed = 0x13371337;



int initializeSSAO(void);
void displaySSAO(struct SSAOFrameBufferStruct* frameBufferDetails);
void uninitializeSSAO(void);
