#include "../../inc/effects/GaussianBlurEffect.h"

struct GaussianBlurEffect gaussianBlurEffect;

bool initializeGaussianBlur(void)
{
    gaussianBlurEffect.horrizontalFBDetails.textureWidth = 800;
    gaussianBlurEffect.horrizontalFBDetails.textureHeight = 600;

    gaussianBlurEffect.verticalFBDetails.textureWidth = 800;
    gaussianBlurEffect.verticalFBDetails.textureHeight = 600;

    return createFBO(&gaussianBlurEffect.horrizontalFBDetails) 
        && createFBO(&gaussianBlurEffect.verticalFBDetails);
}

void displayGaussianBlur(void)
{

}
