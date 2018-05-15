#include "ShadowMapSimple.h"
#include <concrt.h>


ShadowMapSimple::ShadowMapSimple()
{
}

bool ShadowMapSimple::init(unsigned int window_width, unsigned int window_height)
{
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	//Depth texture
	glGenTextures(1, &_shadowMap);
	glBindTexture(GL_TEXTURE_2D, _shadowMap);
	glTexImage2D(_shadowMap, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glReadBuffer(GL_NONE);
	glDrawBuffer(GL_NONE);

	GLenum status = glCheckFramebufferStatus(_fbo);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", status);
		return false;
	}
	return true;
}

void ShadowMapSimple::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo);
}

void ShadowMapSimple::bindForReading()
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, _shadowMap);
}

ShadowMapSimple::~ShadowMapSimple()
{
	glDeleteFramebuffers(1,&_fbo);
	glDeleteTextures(1, &_shadowMap);
}
