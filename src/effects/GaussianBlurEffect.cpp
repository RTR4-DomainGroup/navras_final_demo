#include "../../inc/effects/GaussianBlurEffect.h"


bool initializeGaussianBlur(GaussianBlurEffect* gaussianBlurEffect)
{
    gaussianBlurEffect->horrizontalFBDetails.textureWidth = 1920;
    gaussianBlurEffect->horrizontalFBDetails.textureHeight = 1080;

    gaussianBlurEffect->verticalFBDetails.textureWidth = 1920;
    gaussianBlurEffect->verticalFBDetails.textureHeight = 1080;

    return createFBO(&gaussianBlurEffect->horrizontalFBDetails) 
        && createFBO(&gaussianBlurEffect->verticalFBDetails);
}


