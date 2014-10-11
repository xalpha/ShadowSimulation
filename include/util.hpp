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

#pragma once

#include <nyx/texture.hpp>


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
template <typename T>
inline T length(T a[3])
{
    return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
}


// calculates the dot product between two vectors
template <typename T>
inline T dot(T a[3], T b[3])
{
    return (a[0]*b[0]+a[1]*b[1]+a[2]*b[2])/(length(a)*length(b));
}

// multiplies a vector with a matrix
template <typename T>
inline void multMatrixVector3(T M[16], T v[3], T result[3])
{
    T temp[3];

    temp[0] = M[0]*v[0]+M[4]*v[1]+M[8]*v[2];
    temp[1] = M[1]*v[0]+M[5]*v[1]+M[9]*v[2];
    temp[2] = M[2]*v[0]+M[6]*v[1]+M[10]*v[2];

    result[0] = temp[0];	result[1] = temp[1];	result[2] = temp[2];
}

// multiplies two matrices
template <typename T>
inline void multMatrixMatrix(T a[16], T b[16], T result[16])
{
    T tempMatrix[16];

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


template <typename T>
inline void calculateNormals( int vertex_count, int face_count, const T* vertices, const unsigned int* faces, T* normals, T* face_normals )
{
    // initializiere dings
    T a[3];
    T b[3];
    T vLength;
    unsigned int index;
    for(int i=0;i<vertex_count*3;i++)
        normals[i] = 0.0;
    for(int i=0;i<face_count*3;i++)
        face_normals[i]=0.0;

    // berechne die Normalen der Dreiecke
    for(int f=0;f<face_count;f++)
    {
        // index setzte
        index=f*3;
        // b= C-A
        b[0]=vertices[ faces[index+2]*3 +0] - vertices[ faces[index+0]*3 +0];
        b[1]=vertices[ faces[index+2]*3 +1] - vertices[ faces[index+0]*3 +1];
        b[2]=vertices[ faces[index+2]*3 +2] - vertices[ faces[index+0]*3 +2];
        // c= B-A
        a[0]=vertices[ faces[index+1]*3 +0] - vertices[ faces[index+0]*3 +0];
        a[1]=vertices[ faces[index+1]*3 +1] - vertices[ faces[index+0]*3 +1];
        a[2]=vertices[ faces[index+1]*3 +2] - vertices[ faces[index+0]*3 +2];
        // NormalenVektor bestimmen
        face_normals[index+0]=a[1]*b[2]-a[2]*b[1];
        face_normals[index+1]=a[2]*b[0]-a[0]*b[2];
        face_normals[index+2]=a[0]*b[1]-a[1]*b[0];
        // summiere NormalenVektor des Triangles zu allen NormalenVektoren der Vertices des Triangles
        normals[ faces[index+0]*3 +0] += face_normals[index+0];
        normals[ faces[index+0]*3 +1] += face_normals[index+1];
        normals[ faces[index+0]*3 +2] += face_normals[index+2];

        normals[ faces[index+1]*3 +0] += face_normals[index+0];
        normals[ faces[index+1]*3 +1] += face_normals[index+1];
        normals[ faces[index+1]*3 +2] += face_normals[index+2];

        normals[ faces[index+2]*3 +0] += face_normals[index+0];
        normals[ faces[index+2]*3 +1] += face_normals[index+1];
        normals[ faces[index+2]*3 +2] += face_normals[index+2];
    }
    // Normalisiere VertexNormalen
    for(int i=0;i<vertex_count;i++)
    {
        // index setzte
        index=i*3;
        // Laenge des NormalenVektors
        vLength=0.0;
        vLength+=normals[index+0]*normals[index+0];
        vLength+=normals[index+1]*normals[index+1];
        vLength+=normals[index+2]*normals[index+2];
        vLength=sqrt(vLength);
        //printf( "Normale Vertex %d: %f\t%f\t%f\n", i, normalVer[index+0], normalVer[index+1], normalVer[index+2]);

        // NormalenVektor normalisieren
        normals[index+0]/=(T)vLength;
        normals[index+1]/=(T)vLength;
        normals[index+2]/=(T)vLength;
    }
}


template <typename T>
nyx::texture<T> genExpSprite(int size,T factor)
{
    printf("Generating Exponential Sprite...");
    fflush(stdout);
    T distance=0.0f;
    T tempDistance=0.0f;
    T tempX=0.0f;
    T tempY=0.0f;
    T *pixels = new T[size*size];
    for (int x = 0; x<size; x++)
    {
        for (int y = 0; y<size; y++)
        {
            // set indexes
            int i = (x*size+y);
            // calculate length to center of the texture
            tempX = (T)x-(T)(size/2);
            tempY = (T)y-(T)(size/2);
            tempDistance = sqrt((tempX*tempX+tempY*tempY));
            distance=1.0f-(tempDistance/((T)size/2.0f));
            if(distance>0.0f)
            {
                distance=pow(distance,factor);
                pixels[i]=distance*255.0f;
            }
            else
                pixels[i]=0;
        }
    }
    nyx::texture<T> result;
    result.set_format( GL_RGB8, GL_LUMINANCE );
    result.set_data( size, size, pixels );
    delete[] pixels;
    printf(" done.\n");
    return result;
}
