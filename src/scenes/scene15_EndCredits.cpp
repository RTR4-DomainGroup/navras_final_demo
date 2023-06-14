
#define _USE_MATH_DEFINES 1
#include <math.h>		// for PI

#include "../../inc/effects/videoEffect.h"
#include "../../inc/shaders/FSVQuadShader.h"
#include "../../inc/helper/texture_loader.h"

#include "../../inc/helper/audioplayer.h"
#include "../../inc/helper/constants.h"

static GLuint texture_frame;
static int frameWidth, frameHeight;
static uint8_t* frame_data = NULL;
static struct VideoReaderState vr_state;

int initializeScene15_EndCredits(void)
{
    return 0;
}