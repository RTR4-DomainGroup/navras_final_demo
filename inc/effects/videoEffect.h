#pragma once
#include "../video_reader.h"
#include "../helper/common.h"
#include "../helper/geometry.h"
#include "../shaders/FSVQuadShader.h"

int initializeVideoEffect(const char*, struct VideoReaderState , uint8_t*, GLuint);

void displayVideoEffect(struct FSVQuadUniform*, uint8_t*, GLuint);

void updateVideoEffect(struct VideoReaderState, uint8_t*);

void uninitializeVideoEffect(struct VideoReaderState, uint8_t*, GLuint);
