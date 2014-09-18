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

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

// define constants
#define M__PI   3.1415926535898
#define M__PI_2 1.5707963267949
#define M__2_PI 6.2831853071796

// convert radians to degrees
#ifndef radToDeg
#define radToDeg(x) (180.0 * x / 3.1415926535898)
#endif

// convert degrees to radians
#ifndef degToRad
#define degToRad(x) (3.1415926535898 * x / 180.0)
#endif

// calculates the length of a vector
inline float length(float a[3])
{
	return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}

// calculates the dot product between two vectors
inline float dot(float a[3], float b[3])
{
	return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2])/(length(a)*length(b));
}

// multiplies a vector with a matrix
inline void multMatrixVector3(float M[16], float v[3], float result[3]) 
{
	float temp[3];

	temp[0] = M[0]*v[0]+M[4]*v[1]+M[8]*v[2];
	temp[1] = M[1]*v[0]+M[5]*v[1]+M[9]*v[2];
	temp[2] = M[2]*v[0]+M[6]*v[1]+M[10]*v[2];

	result[0] = temp[0];	result[1] = temp[1];	result[2] = temp[2];
}

// multiplies two matrices
inline void multMatrixMatrix(float a[16], float b[16], float result[16]) 
{
	float tempMatrix[16];

	for (int x = 0;x<16;x+=4)
	{
		for (int y = 0;y<4;y++) 
		{
			tempMatrix[x+y] = b[0+y]*a[0+x]+b[4+y]*a[1+x]+b[8+y]*a[2+x]+b[12+y]*a[3+x];
		}
	}

	result[0]= tempMatrix[0];	result[4]= tempMatrix[4];	result[8] = tempMatrix[8];	result[12] = tempMatrix[12];
	result[1]= tempMatrix[1];	result[5]= tempMatrix[5];	result[9] = tempMatrix[9];	result[13] = tempMatrix[13];
	result[2]= tempMatrix[2];	result[6]= tempMatrix[6];	result[10]= tempMatrix[10];	result[14] = tempMatrix[14];
	result[3]= tempMatrix[3];	result[7]= tempMatrix[7];	result[11]= tempMatrix[11];	result[15] = tempMatrix[15]; 
}


#endif /*MATHUTILS_H_*/
