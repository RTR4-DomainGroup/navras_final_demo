#pragma once
#include "../shaders/StarfieldShader.h"

int initializeStarfield(GLuint*, const char*);
void displayStarfield(GLuint);
float updateStarfield(float);
void uninitializeStarfield(GLuint);

