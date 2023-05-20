#pragma once
#include "../shaders/CloudNoiseShader.h"

GLuint initializeCloud(void);
void updateCloud(bool, float, float);
void uninitializeCloud(void);