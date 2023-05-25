#include "../../inc/effects/GaussianBlurEffect.h"


bool initializeGaussianBlur(GaussianBlurEffect* gaussianBlurEffect)
{
    gaussianBlurEffect->horrizontalFBDetails.textureWidth = WIN_WIDTH;
    gaussianBlurEffect->horrizontalFBDetails.textureHeight = WIN_HEIGHT;

    gaussianBlurEffect->verticalFBDetails.textureWidth = WIN_WIDTH;
    gaussianBlurEffect->verticalFBDetails.textureHeight = WIN_HEIGHT;

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

