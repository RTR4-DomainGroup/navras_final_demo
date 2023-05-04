#pragma once
#include "../shaders/HorrizontalBlur.h"
#include "../shaders/VerticalBlur.h"
#include "../helper/framebuffer.h"

struct GaussianBlurEffect
{
    FrameBufferDetails horrizontalFBDetails;
    FrameBufferDetails verticalFBDetails;
};


bool initializeGaussianBlur(GaussianBlurEffect*);
void displayGaussianBlur(void);
