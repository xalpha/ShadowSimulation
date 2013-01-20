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

#ifndef NORMALIZE_H_
#define NORMALIZE_H_

/** 
 * Berectnet die Triangle- und die VertexNormalen
 * vertices	= Anzahl der Vertices
 * triangles 	= Anzahl der Triangles
 * vertex	= Array von Vertices
 * triangle	= Array von Triangles
 * normalVer	= Array der VertexNormalen
**/
void calculateNormals(int vertices,int triangles,float vertex[],unsigned int triangle[],float normalVer[], float normalTri[]);


#endif
