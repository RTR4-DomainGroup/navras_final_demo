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

void uninitializeGaussianBlur(GaussianBlurEffect* gaussianBlurEffect)
{
    if (gaussianBlurEffect)
    {
        if(gaussianBlurEffect->horrizontalFBDetails.frameBufferTexture)
        {
            glDeleteTextures(1, &gaussianBlurEffect->horrizontalFBDetails.frameBufferTexture);
		    gaussianBlurEffect->horrizontalFBDetails.frameBufferTexture = 0;
        }

        if(gaussianBlurEffect->verticalFBDetails.frameBufferTexture)
        {
            glDeleteTextures(1, &gaussianBlurEffect->verticalFBDetails.frameBufferTexture);
		    gaussianBlurEffect->verticalFBDetails.frameBufferTexture = 0;
        }

        if (gaussianBlurEffect->horrizontalFBDetails.frameBuffer)
        {
            glDeleteFramebuffers(1, &gaussianBlurEffect->horrizontalFBDetails.frameBuffer);
            gaussianBlurEffect->horrizontalFBDetails.frameBuffer = 0;
        }

        if (gaussianBlurEffect->horrizontalFBDetails.renderBuffer)
        {
            glDeleteRenderbuffers(1, &gaussianBlurEffect->horrizontalFBDetails.renderBuffer);
            gaussianBlurEffect->horrizontalFBDetails.renderBuffer = 0;
        }

        if (gaussianBlurEffect->verticalFBDetails.frameBuffer)
        {
            glDeleteFramebuffers(1, &gaussianBlurEffect->verticalFBDetails.frameBuffer);
            gaussianBlurEffect->verticalFBDetails.frameBuffer = 0;
        }
        
        if (gaussianBlurEffect->verticalFBDetails.renderBuffer)
        {
            glDeleteRenderbuffers(1, &gaussianBlurEffect->verticalFBDetails.renderBuffer);
            gaussianBlurEffect->verticalFBDetails.renderBuffer = 0;
        }
        
        free(gaussianBlurEffect);
        gaussianBlurEffect = NULL;
    }
    
}

