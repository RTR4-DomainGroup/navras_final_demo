#pragma once
#include "../shaders/CloudNoiseShader.h"

GLuint initializeCloud(void);
void updateCloud(bool, float, float);
void updateErode(bool, GLfloat[3], float);
void uninitializeCloud(void);