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

#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>


Bmp::Bmp(){}

Bmp::Bmp(char *fileName)
{
	readBmp(fileName);
}

/*
Bmp::Bmp(int widthX, int heightX, char format, char *dataX)
{
	width = widthX;
	height= heightX;
	data  = dataX;
}
*/

/*
unsigned int Bmp::getWidth()	{	return width;	}
void Bmp::setWidth(int widthX)	{	width = widthX;	}

unsigned int Bmp::getHeight()	{	return height;	}
void Bmp::setHeight(int heightX)	{	height = heightX;	}

unsigned int Bmp::getColorCount()	{	return colorCount;	}
void Bmp::setColorCount(int colorCountX)	{	colorCount = colorCountX;	}

char* Bmp::getData()	{	return data;	}
void Bmp::setData(char *dataX)	{	data = dataX;	}
*/



bool Bmp::readBmp(char *fileName)
{
	// The BITMAPFILEHEADER
	short type;   // must always be set to 'BM' to declare that this is a .bmp-file.
	long fileSize;   // specifies the size of the file in bytes.
	short bfReserved1;   // must always be set to zero.
	short bfReserved2;   // must always be set to zero.
	long dataOffset;   // specifies the offset from the beginning of the file to the bitmap data.

	// The BITMAPINFOHEADER
	long infoHeaderSize;   // specifies the size of the BITMAPINFOHEADER structure, in bytes.
	long imgWidth;   // specifies the width of the image, in pixels.
	long imgHeight;   // specifies the height of the image, in pixels.
	short planes;   // specifies the number of planes of the target device, must be set to zero.
	short bitCount;   // specifies the number of bits per pixel.
	long compression;   // specifies the type of compression, usually set to zero (no compression).
	long imgSize;   // specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero.
	long xPixMeter;   // specifies the the horizontal pixels per meter on the designated targer device, usually set to zero.
	long yPixMeter;   // specifies the the vertical pixels per meter on the designated targer device, usually set to zero.
	long colorCountX;   // specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member.
	long colorImportant;   // specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.


	// Open the File
	FILE *file = fopen( fileName, "r");
	
	// read the BITMAPFILEHEADER 
	fread( &type, sizeof(short), 1, file);
	fread( &fileSize, sizeof(long), 1, file);
	fread( &bfReserved1, sizeof(short), 1, file);
	fread( &bfReserved2, sizeof(short), 1, file);
	fread( &dataOffset, sizeof(long), 1, file);
	/*
	printf("type=%i\n", type);
	printf("fileSize=%i\n", fileSize);
	printf("bfReserved1=%i\n", bfReserved1);
	printf("bfReserved1=%i\n", bfReserved2);
	printf("dataOffset=%i\n", dataOffset);
	*/

	// read the BITMAPINFOHEADER
	fread( &infoHeaderSize, sizeof(long), 1, file);
	fread( &imgWidth, sizeof(long), 1, file);
	fread( &imgHeight, sizeof(long), 1, file);
	fread( &planes, sizeof(short), 1, file);
	fread( &bitCount, sizeof(short), 1, file);
	fread( &compression, sizeof(long), 1, file);
	fread( &imgSize, sizeof(long), 1, file);
	fread( &xPixMeter, sizeof(long), 1, file);
	fread( &yPixMeter, sizeof(long), 1, file);
	fread( &colorCountX, sizeof(long), 1, file);
	fread( &colorImportant, sizeof(long), 1, file);
	/*
	printf("infoHeaderSize=%i\n",infoHeaderSize);
	printf("imgWidth=%i\n",imgWidth);
	printf("imgHeight=%i\n",imgHeight);
	printf("planes=%i\n",planes);
	printf("bitCount=%i\n",bitCount);
	printf("compression=%i\n",compression);
	printf("imgSize=%i\n",imgSize);
	printf("xPixMeter=%i\n",xPixMeter);
	printf("yPixMeter=%i\n",yPixMeter);
	printf("colorCountX=%i\n",colorCountX);
	printf("colorImportant=%i\n",colorImportant);
	*/

	// read the RGBQUAD array
	colorCount = bitCount/8;
	width = imgWidth;
	height= imgHeight;
	data = new unsigned char[width*height*colorCount];
	fseek(file, dataOffset, SEEK_SET);
	fread(data, sizeof(unsigned char)*width*height*colorCount, 1, file);
	fclose( file );

	return false;
}
