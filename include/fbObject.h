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

/***************************************************************
 *                                                             *
 * program  : Frame Buffer Object                              *
 * author   : Alexandru Duliu                                  *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/
#ifndef FBOBJECT_H_
#define FBOBJECT_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#define FBO_COLOR 0x0001
#define FBO_COLOR_TEX 0x0002
#define FBO_COLOR_TEX_EXT 0x0010
#define FBO_DEPTH 0x0004
#define FBO_DEPTH_TEX 0x0008
#define FBO_DEPTH_TEX_EXT 0x0020

class FBObject
{
	// size of frameBuffer
	int sizeX;
	int sizeY;
	
	// za FrameBuffer	
	GLuint frameBuffer;


	// colorBuffer
	GLuint colorBuffer;
	bool toggleColorBuffer;
	bool toggleColorBufferTex;
	bool toggleColorBufferTexExt;
	unsigned int colorBufferFormat;

	// depthBuffer
	GLuint depthBuffer;
	bool toggleDepthBuffer;
	bool toggleDepthBufferTex;
	bool toggleDepthBufferTexExt;
	unsigned int depthBufferFormat;

	// attributes
	//GLint internalFormat;
	//GLenum format;
	//GLenum type;

public:

	// textures
	GLuint colorBufferTex;
	GLuint depthBufferTex;
	

	// empty constructor
	FBObject();

	// constructor for square buffers
	FBObject(unsigned int size, unsigned int buffers);
	
	// empty constructor
	~FBObject();

	// initialize FBO
	void init(unsigned int size, unsigned int buffers);
	void init(unsigned int size, unsigned int buffers, GLint internalFormat, GLenum format, GLenum type);

	// enable rendering to this FBO
	inline void enable();
	inline void enable(GLuint colorBufferTex1);

	// disable rendering to this FBO
	inline void disable();

	// get the size
	inline int getSizeX();
	inline int getSizeY();

};

#include "fbObject.inl"

#endif
