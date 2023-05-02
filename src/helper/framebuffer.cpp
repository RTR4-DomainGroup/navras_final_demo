#include "../../inc/helper/common.h"
#include "../../inc/helper/framebuffer.h"

bool createFBO(struct FrameBufferDetails* frameBufferDetails)
{
    // Code

    //1. Check available render buffer Size
    int maxRenderbufferSize;

    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);
    if (maxRenderbufferSize < frameBufferDetails->textureWidth || maxRenderbufferSize < frameBufferDetails->textureHeight)
    {
        LOG("Insufficient Render buffer size");
        return false;
    }
    
    //2. Create frame buffer object
    glGenFramebuffers(1, &frameBufferDetails->frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferDetails->frameBuffer);

    // 3. Create Render Buffer object
    glGenRenderbuffers(1, &frameBufferDetails->renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, frameBufferDetails->renderBuffer);

    // 4. Storage and Format of the Render Buffer
    //This has nothing to with depth. 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, frameBufferDetails->textureWidth, frameBufferDetails->textureHeight);

    //5. Create Empty texture for upcoming target scene
    glGenTextures(1, &frameBufferDetails->frameBufferTexture);
    glBindTexture(GL_TEXTURE_2D, frameBufferDetails->frameBufferTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameBufferDetails->textureWidth, frameBufferDetails->textureHeight, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferDetails->frameBufferTexture, 0);
  
    // 6. Give RBO to FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, frameBufferDetails->renderBuffer);

    // 7. Check whether FB created successfully or not
    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG("Framebuffer is not complete \n");
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}
