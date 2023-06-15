#pragma once
#include "../video_reader.h"
#include "../helper/common.h"
#include "../helper/geometry.h"
#include "../shaders/FSVQuadShader.h"

int initializeVideoEffect(const char*);

void displayVideoEffect();

void updateVideoEffect(void);

void uninitializeVideoEffect(void);
