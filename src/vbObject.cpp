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

#include "vbObject.h"

// contructor
VBObject::VBObject()
{
	// set the array counters to zero
	vertexCount = 0;
	indexCount = 0;
	normalCount = 0;
	colorCount = 0;
	texCoordCount = 0;
}

// deconstructor
VBObject::~VBObject()
{
	if(vertexCount != 0)
	{
        //delete [] vertices;
		glDeleteBuffersARB( 1, &vboVertices);
	}

	if(indexCount != 0)
	{
        //delete [] indices;
		glDeleteBuffersARB( 1, &vboIndices);
	}
	
	if(normalCount != 0)
	{
		//delete [] normals;
		glDeleteBuffersARB( 1, &vboNormals);
	}
	
	if(colorCount != 0)
	{
		//delete [] colors;
		glDeleteBuffersARB( 1, &vboColors);

	}
	
	if(texCoordCount != 0)
	{
        //delete [] texCoords;
        glDeleteBuffersARB( 1, &vboTexCoords);
	}
}

void VBObject::initVertices(float *v, int vc, int vs, int vbu, int pt)
{
	// if there was a previously loaded model delete the old VBO
	if(vertexCount != 0) glDeleteBuffersARB( 1, &vboVertices);

	vertices = v;
	vertexCount = vc;
	primitivesType = pt;
	vertexSize = vs;
	vertexBufferUsage = vbu;
	glGenBuffersARB( 1, &vboVertices);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboVertices);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(float)*vertexSize, vertices, vertexBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}

void VBObject::initVertices(float *v, int vc)
{
	initVertices(v, vc, 3, GL_STATIC_DRAW_ARB, GL_TRIANGLES);
}

void VBObject::updateVertices(float *v)
{
	vertices = v;
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboVertices);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, vertexCount*sizeof(float)*vertexSize, vertices, vertexBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}





void VBObject::initIndices(unsigned int *i, int ic, int ibu, int pt)
{
	// if there was a previously loaded model delete the old VBO
	if(indexCount != 0) glDeleteBuffersARB( 1, &vboIndices);

	indices = i;
	indexCount = ic;
	primitivesType = pt;
	switch( primitivesType )
	{
		case GL_POINTS	:	indexSize = 1;	break;
		case GL_LINES	:	indexSize = 2;	break;
		case GL_TRIANGLES	:	indexSize = 3;	break;
		case GL_QUADS	:	indexSize = 4; break;
		//default	:	printf("Invalid index primitive type !\n");
	}
	indexBufferUsage = ibu;
	glGenBuffersARB( 1, &vboIndices);
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, vboIndices);
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, indexCount*sizeof(unsigned int)*indexSize, indices, indexBufferUsage);
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	
}

void VBObject::initNormals(float *n, int nc, int nbu)
{
	// if there was a previously loaded model delete the old VBO
	if(normalCount != 0) glDeleteBuffersARB( 1, &vboNormals);


	normals = n;
	normalCount = nc;
	normalBufferUsage = nbu;
	glGenBuffersARB( 1, &vboNormals);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboNormals);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, normalCount*sizeof(float)*3, normals, normalBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}


void VBObject::updateNormals(float *n)
{
	normals = n;
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboNormals);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, normalCount*sizeof(float)*3, normals, normalBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}


void VBObject::initColors(float *c, int cc, int cs, int cbu)
{
	// if there was a previously loaded model delete the old VBO
	if(colorCount != 0) glDeleteBuffersARB( 1, &vboColors);

	colors = c;
	colorCount = cc;
	colorSize = cs;
	colorBufferUsage = cbu;
	glGenBuffersARB( 1, &vboColors);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboColors);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, colorCount*sizeof(float)*colorSize, colors, colorBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}


void VBObject::updateColors(float *c)
{
	colors = c;
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboColors);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, colorCount*sizeof(float)*colorSize, colors, colorBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}


void VBObject::initTexCoords(float *t1, int tc1)
{
	initTexCoords(t1,tc1,2,GL_STATIC_DRAW_ARB);
}


void VBObject::initTexCoords(float *t, int tc, int ts, int tbu)
{
	// if there was a previously loaded model delete the old VBO
	if(texCoordCount != 0) glDeleteBuffersARB( 1, &vboTexCoords);

	texCoords = t;
	texCoordCount = tc;
	texCoordSize = ts;
	texCoordBufferUsage = tbu;
	glGenBuffersARB( 1, &vboTexCoords);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboTexCoords);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, texCoordCount*sizeof(float)*texCoordSize, texCoords, texCoordBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}


void VBObject::updateTexCoords(float *tc)
{
	texCoords = tc;
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vboTexCoords);
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, texCoordCount*sizeof(float)*2, texCoords, texCoordBufferUsage);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0);
}

// general initialize function which sets up a vertex buffer for static vertices, indices and normals for rendering GL_TRIANGLES
void VBObject::initialize(float *v, unsigned int *i, float *n, int vc, int ic, int nc)
{
	initVertices( v, vc, 3, GL_STATIC_DRAW_ARB, GL_TRIANGLES);
	initIndices( i, ic, GL_STATIC_DRAW_ARB, GL_TRIANGLES);
	initNormals( n, nc, GL_STATIC_DRAW_ARB);
}
