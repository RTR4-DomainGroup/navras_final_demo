#pragma once
#include "../video_reader.h"
#include "../helper/common.h"
#include "../helper/geometry.h"
#include "../shaders/FSQuadShader.h"

int initializeVideoEffect(const char*);

void displayVideoEffect(struct FSQuadUniform*);

void uninitializeVideoEffect(void);
