#pragma once

#include "../common.h"
#include "../geometry.h"
#include "../shaders/ADSLightShader.h"
#include "../shaders/CloudNoiseShader.h"

//int initializeInterleaved(void);
//void displayInterleaved(void);
//void updateInterleaved(void);
//void uninitializeInterleaved(void);

int initializeCloudNoise(void);
void displayCloudNoise(void);
void updateCloudNoise(void);
void uninitializeCloudNoise(void);
