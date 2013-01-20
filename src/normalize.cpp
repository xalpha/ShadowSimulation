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

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

void calculateNormals(int vertices,int triangles,float vertex[],unsigned int triangle[],float normalVer[],float normalTri[])
{
	float *a = new float[3];
	float *b = new float[3];
	//float *normalTri = new float[triangles*3];
	double vLength;
	int index;
	
	for(int i=0;i<vertices*3;i++) {
		normalVer[i] = 0.0;
	}
	for(int i=0;i<triangles*3;i++)
	{
		normalTri[i]=0.0;
	}



	// berechne die Normalen der Dreiecke
	for(int i=0;i<triangles;i++)
	{
        // index setzte
        index=i*3;
		// b= C-A
 		b[0]=vertex[ triangle[index+2]*3 +0] - vertex[ triangle[index+0]*3 +0];
		b[1]=vertex[ triangle[index+2]*3 +1] - vertex[ triangle[index+0]*3 +1];
		b[2]=vertex[ triangle[index+2]*3 +2] - vertex[ triangle[index+0]*3 +2];
		// c= B-A
		a[0]=vertex[ triangle[index+1]*3 +0] - vertex[ triangle[index+0]*3 +0];
		a[1]=vertex[ triangle[index+1]*3 +1] - vertex[ triangle[index+0]*3 +1];
		a[2]=vertex[ triangle[index+1]*3 +2] - vertex[ triangle[index+0]*3 +2];
		// NormalenVektor bestimmen
		normalTri[index+0]=a[1]*b[2]-a[2]*b[1];
		normalTri[index+1]=a[2]*b[0]-a[0]*b[2];
		normalTri[index+2]=a[0]*b[1]-a[1]*b[0];
		// summiere NormalenVektor des Triangles zu allen NormalenVektoren der Vertices des Triangles
		normalVer[ triangle[index+0]*3 +0] += normalTri[index+0];
		normalVer[ triangle[index+0]*3 +1] += normalTri[index+1];
		normalVer[ triangle[index+0]*3 +2] += normalTri[index+2];
		
		normalVer[ triangle[index+1]*3 +0] += normalTri[index+0];
		normalVer[ triangle[index+1]*3 +1] += normalTri[index+1];
		normalVer[ triangle[index+1]*3 +2] += normalTri[index+2];
		
		normalVer[ triangle[index+2]*3 +0] += normalTri[index+0];
		normalVer[ triangle[index+2]*3 +1] += normalTri[index+1];
		normalVer[ triangle[index+2]*3 +2] += normalTri[index+2];
	}
	// Normalisiere VertexNormalen
	for(int i=0;i<vertices;i++)
	{
        // index setzte
        index=i*3;
		// Laenge des NormalenVektors 
		vLength=0.0;
		vLength+=normalVer[index+0]*normalVer[index+0];
		vLength+=normalVer[index+1]*normalVer[index+1];
		vLength+=normalVer[index+2]*normalVer[index+2];
		vLength=sqrt(vLength);
		//printf( "Normale Vertex %d: %f\t%f\t%f\n", i, normalVer[index+0], normalVer[index+1], normalVer[index+2]);
		
		// NormalenVektor normalisieren
		normalVer[index+0]/=(float)vLength;
		normalVer[index+1]/=(float)vLength;
		normalVer[index+2]/=(float)vLength;
	}
}
