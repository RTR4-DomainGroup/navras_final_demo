#pragma once

#define _USE_MATH_DEFINES 1
#include <math.h> 

// Macro definitions

// small window
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// // maximized
// #define WIN_WIDTH 1280
// #define WIN_HEIGHT 720

// // regular FHD
// #define WIN_WIDTH 1920
// #define WIN_HEIGHT 1080

#define DEG2RADN(x) (x * (M_PI / 180)) 

// #define X_MIN (0.0f)
// #define X_MAX (1.0f)
#define X_MIN (-30.0f)
#define X_MAX (30.0f)

#define Y_MIN (-3.0f)
#define Y_MAX (3.0f)

// #define Z_MIN (0.0f)
// #define Z_MAX (1.0f)
#define Z_MIN (-30.0f)
#define Z_MAX (30.0f)

#define COLR_MIN (0)
#define COLR_MAX (255)

#define NO_OF_INSTANCES 50
#define NO_OF_INSTANCES_DESK 10

#define X_INCREMENT 2.0f
#define Y_INCREMENT 0.8f
#define Z_INCREMENT -0.5f

#define X_SEPARTION_OFFSET 0.5f
#define Y_SEPARTION_OFFSET 0.0f
#define Z_SEPARTION_OFFSET 0.0f

// logging
#define MAX_LOG_LENGTH 255


// sample audios
#define NUM_AUDIO   3
static const char *szAudios[] = {
    "audio1.wav", 
    "audio2.wav", 
    "audio3.wav", 
};

// Feature enable / disable
 
//#define ENABLE_ADSLIGHT		##### ONLY FOR REF.. KEEP COMMENTED #####

// always ON
#define ENABLE_WATER
#define ENABLE_GAUSSIAN_BLUR
#define ENABLE_GODRAYS

// configurable
#define ENABLE_TERRIAN
//#define ENABLE_ATMOSPHERE
#define ENABLE_CLOUD_NOISE
//#define ENABLE_SKYBOX
#define ENABLE_STARFIELD
//#define ENABLE_FOG
#define ENABLE_BILLBOARDING
//#define ENABLE_VIDEO_RENDER
#define ENABLE_SHADOW
#define ENABLE_STATIC_MODELS	
#define ENABLE_DYNAMIC_MODELS

