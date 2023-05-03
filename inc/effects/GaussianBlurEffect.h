#pragma once
#include "../shaders/HorrizontalBlur.h"
#include "../shaders/HorrizontalBlur.h"
#include "../helper/framebuffer.h"

struct GaussianBlurEffect
{
    FrameBufferDetails horrizontalFBDetails;
    FrameBufferDetails verticalFBDetails;
};


bool initializeGaussianBlur(void);
void displayGaussianBlur(void);
