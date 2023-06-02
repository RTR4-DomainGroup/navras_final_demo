#pragma once
//#include "../Noise.h"
#include "../shaders/ErosionNoiseShader.h"

GLuint initializeErosion(void);
void updateErosion(bool, float[3], float);
void uninitializeErosion(void);