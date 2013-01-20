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

#include "shader.h"


Shader::Shader(){}


Shader::Shader(char *vertexPath, char *fragmentPath)
{
	init(vertexPath, fragmentPath);
}

Shader::~Shader()
{
	
}


void Shader::init(char *vertexPath, char *fragmentPath)
{
	// read vertex shader source
	char *vertexSource = NULL;
	
	FILE *filePointer;

	int count=0;

	if (vertexPath != NULL) 
	{
		filePointer = fopen(vertexPath,"rt");

		if (filePointer != NULL) 
		{
			fseek(filePointer, 0, SEEK_END);
			count = ftell(filePointer);
			rewind(filePointer);

			if (count > 0) {
				vertexSource = (char *)malloc(sizeof(char) * (count+1));
				count = fread(vertexSource,sizeof(char),count,filePointer);
				vertexSource[count] = '\0';
			}
			fclose(filePointer);
		}
	}

	// read vertex shader source
	char *fragmentSource = NULL;

	if (fragmentPath != NULL) 
	{
		filePointer = fopen(fragmentPath,"rt");

		if (filePointer != NULL) 
		{
			fseek(filePointer, 0, SEEK_END);
			count = ftell(filePointer);
			rewind(filePointer);

			if (count > 0) {
				fragmentSource = (char *)malloc(sizeof(char) * (count+1));
				count = fread(fragmentSource,sizeof(char),count,filePointer);
				fragmentSource[count] = '\0';
			}
			fclose(filePointer);
		}
	}

	const char *constVertexSource = vertexSource;
	const char *constFragmentSource = fragmentSource;
	
	vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
   	fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	glShaderSourceARB(vertexShader, 1, &constVertexSource,NULL);
	glShaderSourceARB(fragmentShader, 1, &constFragmentSource,NULL);

	delete [] constVertexSource;
	delete [] constFragmentSource;
   	
   	glCompileShaderARB(vertexShader);
   	glCompileShaderARB(fragmentShader);
   	
  	program = glCreateProgramObjectARB();
  	
	glAttachObjectARB(program,vertexShader);
	glAttachObjectARB(program,fragmentShader);
	
	glLinkProgramARB(program);

	glUseProgramObjectARB(program);

	// unifrom variable stuff

	glUseProgramObjectARB(0);

	printf("\n\n%s\n\n",vertexPath);

	printInfoLog();
}


void Shader::printInfoLog()
{
	int logLength = 0;
    glGetObjectParameterivARB(program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength);
	if (logLength > 1)
	{
		char *logText = (char*)malloc(logLength);
		int writtenLength = 0;

		glGetInfoLogARB(program, logLength, &writtenLength, logText);
		
		printf("\n\n%s\n\n",logText);
		fflush(stdout);
		
		delete [] logText;
	}
}
