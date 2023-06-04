#include "../../inc/helper/common.h"
#include "../../inc/helper/ssaoframebuffer.h"

bool ssaoCreateFBO(struct SSAOFrameBufferStruct *frameBufferDetails)
{
	glGenFramebuffers(1, &frameBufferDetails->render_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferDetails->render_fbo);
	glGenTextures(3, frameBufferDetails->fbo_textures);

	glBindTexture(GL_TEXTURE_2D, frameBufferDetails->fbo_textures[0]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB16F, 2048, 2048);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, frameBufferDetails->fbo_textures[1]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, 2048, 2048);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, frameBufferDetails->fbo_textures[2]);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 2048, 2048);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frameBufferDetails->fbo_textures[0], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, frameBufferDetails->fbo_textures[1], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, frameBufferDetails->fbo_textures[2], 0);

	
	static const GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, draw_buffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG("Framebuffer is not Shadow Complete \n");
		return false;
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
