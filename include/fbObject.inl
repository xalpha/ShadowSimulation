inline void FBObject::enable()
{
	//glBindTexture(GL_TEXTURE_2D,0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
}

inline void FBObject::enable(GLuint colorBufferTex1)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,colorBufferTex1,0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);

	//glBindTexture(GL_TEXTURE_2D,0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
}

inline void FBObject::disable()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	//glBindTexture(GL_TEXTURE_2D,0);
	//glBindTexture(GL_TEXTURE_2D,colorBuffer);
}
