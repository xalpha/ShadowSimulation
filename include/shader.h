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
 * program  : Shader                                           *
 * author   : Alexandru Duliu                                  *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/
#ifndef SHADER_H_
#define SHADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

class Shader
{
	GLhandleARB program;
	GLhandleARB vertexShader;
	GLhandleARB fragmentShader;

public:

	// constructor
	Shader();

	// constructor with the path to the vertex and fragment shader
	Shader(char *vertexPath, char *fragmentPath);

	// deconstructor
	~Shader();

	// initialize Shader
	void init(char *vertexPath, char *fragmentPath);

	// print InfoLog
	void printInfoLog();

	// switch from ffp to this shader
	inline void enable();

	// return to ffp
	inline void disable();
};

#include "shader.inl"

#endif
