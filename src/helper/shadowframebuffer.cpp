#include "../../inc/helper/common.h"
#include "../../inc/helper/shadowframebuffer.h"

bool shadowCreateFBO(struct ShadowFrameBufferDetails *frameBufferDetails)
{
    // Code
	glGenFramebuffers(1, &frameBufferDetails->frameBuffer);
	// create depth texture

	glGenTextures(1, &frameBufferDetails->frameBufferDepthTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferDetails->frameBufferDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, frameBufferDetails->textureWidth, frameBufferDetails->textureWidth, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferDetails->frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frameBufferDetails->frameBufferDepthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG("Framebuffer is not Shadow Complete \n");
        return false;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;

}
