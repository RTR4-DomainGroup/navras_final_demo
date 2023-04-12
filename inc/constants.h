#pragma once

#define _USE_MATH_DEFINES 1
#include <math.h> 

// #include "types.h"

// Macro definition

// // maximized
// #define WIN_WIDTH 1280
// #define WIN_HEIGHT 720


#define DOMAIN_PI 3.14159265358979323846

// regular
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// #define PIE 3.14f
// #define DEG2RADN(x) (x * (PIE / 180)) // (deg * 2 * PIE) / 360
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

#define NO_OF_INSTANCES 500

#define X_INCREMENT 2.0f
#define Y_INCREMENT 0.8f
#define Z_INCREMENT -0.5f

#define X_SEPARTION_OFFSET 0.5f
#define Y_SEPARTION_OFFSET 0.0f
#define Z_SEPARTION_OFFSET 0.0f

