#include "../../inc/helper/common.h"
#include "../../inc/helper/waterframebuffer.h"

bool waterCreateFBO(struct WaterFrameBufferDetails *frameBufferDetails)
{
    // Code
    glGenFramebuffers(1, &frameBufferDetails->frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferDetails->frameBuffer);

    glGenTextures(1, &frameBufferDetails->frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, frameBufferDetails->frameBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferDetails->textureWidth, frameBufferDetails->textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glGenTextures(1, &frameBufferDetails->frameBufferDepthTexture);
    glBindTexture(GL_TEXTURE_2D, frameBufferDetails->frameBufferDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, frameBufferDetails->textureWidth, frameBufferDetails->textureHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameBufferDetails->frameBufferTexture, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBufferDetails->frameBufferDepthTexture, 0);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG("Framebuffer is not Water Complete \n");
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;

}
