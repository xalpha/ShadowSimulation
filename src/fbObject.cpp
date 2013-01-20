////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is part of ShadowSimulation, a C++ app for schadow simulation    //
//                                                                            //
// Copyright (C) 2006 Alexandru Duliu                                         //
//                                                                            //
// ShadowSimulation is free software; you can redistribute it and/or          //
// modify it under the terms of the GNU  General Public License               //
// as published by the Free Software Foundation; either version 3             //
// of the License, or (at your option) any later version.                     //
//                                                                            //
// ShadowSimulation is distributed in the hope that it will be useful,        //
// but WITHOUT ANY WARRANTY; without even the implied warranty of             //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              //
// GNU General Public License for more details.                               //
//                                                                            //
// You should have received a copy of the GNU General Public License          //
// along with ShadowSimulation. If not, see <http://www.gnu.org/licenses/>.   //
//                                                                            //
///////////////////////////////////////////////////////////////////////////////

#include "fbObject.h"


FBObject::FBObject(){}


FBObject::FBObject(unsigned int size, unsigned int buffers)
{
	init(size, buffers);
}

FBObject::~FBObject()
{
	// delete color buffer
	if(toggleColorBuffer) glDeleteRenderbuffersEXT(1, &colorBuffer);
	if(toggleColorBufferTex) glDeleteTextures(1, &colorBufferTex);

	// delete depth buffer
	if(toggleDepthBuffer) glDeleteRenderbuffersEXT(1, &depthBuffer);
	if(toggleDepthBufferTex) glDeleteTextures( 1, &depthBufferTex);

	// delete FBO
	glDeleteFramebuffersEXT(1,&frameBuffer);

}

void FBObject::init(unsigned int size, unsigned int buffers)
{
	init(size, buffers, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
}


void FBObject::init(unsigned int size, unsigned int buffers, GLint internalFormat, GLenum format, GLenum type )
{
	sizeX = size;
	sizeY = size;
	if( ( (buffers & FBO_COLOR) != 0) && ( (buffers & FBO_COLOR_TEX) !=0) ||
		( (buffers & FBO_DEPTH) != 0) && ( (buffers & FBO_DEPTH_TEX) !=0) )
	{
		printf("Invalid FBO initialisation!\n");
		return;
	}
	toggleColorBuffer = (buffers & FBO_COLOR) !=0;
	toggleColorBufferTex = (buffers & FBO_COLOR_TEX) !=0;
	toggleColorBufferTexExt = (buffers & FBO_COLOR_TEX_EXT) !=0;
	toggleDepthBuffer = (buffers & FBO_DEPTH) !=0;
	toggleDepthBufferTex = (buffers & FBO_DEPTH_TEX) !=0;
	toggleDepthBufferTexExt = (buffers & FBO_DEPTH_TEX_EXT) !=0;

	//printf("cb=%i cbt=%i db=%i dbt=%i",toggleColorBuffer,toggleColorBufferTex,toggleDepthBuffer,toggleDepthBufferTex);

	// gen Framebuffer
	glGenFramebuffersEXT(1,&frameBuffer);
			
	// colorBuffers texture
	
	if(toggleColorBuffer)
	{
		// gen ColorBuffer

		glGenRenderbuffersEXT(1, &colorBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,colorBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,internalFormat,sizeX, sizeY);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colorBuffer);

		glGenRenderbuffersEXT(1, &colorBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,colorBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,internalFormat,sizeX, sizeY);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colorBuffer);
		
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	}
	else if(toggleColorBufferTex)
	{
		glGenTextures(1, &colorBufferTex);
		glBindTexture(GL_TEXTURE_2D, colorBufferTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, sizeX, sizeY, 0, format, type,NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,colorBufferTex,0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	}
	else if(toggleColorBufferTexExt)
	{
		/*glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,colorBufferTex,0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);*/
	}
	else
	{
		// gen blank ColorBuffer
		glGenRenderbuffersEXT(1, &colorBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,colorBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,internalFormat,sizeX, sizeY);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, colorBuffer);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	

	// depthBuffer texture
	if(toggleDepthBuffer)
	{
		// gen DepthBuffer
		glGenRenderbuffersEXT(1, &depthBuffer);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT,depthBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT24,sizeX, sizeY);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuffer);
	}
	else if(toggleDepthBufferTex)
	{
		glGenTextures( 1, &depthBufferTex);
		glBindTexture( GL_TEXTURE_2D, depthBufferTex);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, sizeX, sizeY, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,depthBufferTex,0);
	}
	else if(toggleDepthBufferTexExt)
	{
		/*glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,depthBufferTex,0);*/
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	/*
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindTexture(GL_TEXTURE_2D,0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,frameBuffer);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_2D,colorBuffer,0);
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_RENDERBUFFER_EXT,depthBuffer);
	//glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D,depthBufferTex,0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT,0);
	*/

	// TODO check if the FBO was setup properly
    glBindFramebufferEXT(GL_FRAMEBUFFER,frameBuffer);
    GLenum  fboStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
	switch(fboStatus)
	{
        case GL_FRAMEBUFFER_COMPLETE                        : printf("GL_FRAMEBUFFER_COMPLETE\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT           : printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT   : printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n"); break;
//        case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT : printf("GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT\n"); break;
//        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS           : printf("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS\n"); break;
//        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS              : printf("GL_FRAMEBUFFER_INCOMPLETE_FORMATS\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER          : printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n"); break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER          : printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n"); break;
        case GL_FRAMEBUFFER_UNSUPPORTED                     : printf("GL_FRAMEBUFFER_UNSUPPORTED\n"); break;
//        case GL_FRAMEBUFFER_STATUS_ERROR                    : printf("GL_FRAMEBUFFER_STATUS_ERROR\n");  break;
		default : printf("%i\n",fboStatus);
	}
    glBindFramebufferEXT(GL_FRAMEBUFFER,0);
}
