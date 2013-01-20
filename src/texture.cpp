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

#include "texture.h"

GLuint loadTexture(char *filepath, GLenum format)
{
	unsigned int tex;
	Bmp texture;
	texture.readBmp( filepath);

	glGenTextures( 1, &tex);
	
	glBindTexture( GL_TEXTURE_2D, tex);

	glTexImage2D( GL_TEXTURE_2D, 0, 3, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.data);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	return tex;	
}

GLuint genCellular(int density, int size, GLfloat color1[4], GLfloat color2[4])
{
	printf("Generating cellular texture... ");
	fflush(stdout);

	GLuint result;

	// variables necesarry for distance calculations
	double totalDistance;
	double *distance = new double[density];
	double near1, near2, far1, far2=0.0;
	double maxDist = 1.41421356*(double)size;
	int tempX, tempY;

	// generate random nodes
	int *nodes = new int [density*2];
	for(int i =0;i<density;i++)
	{
		nodes[i*2]  =rand()%size;
		nodes[i*2+1]=rand()%size;	
	}

	GLubyte *pixels = new GLubyte[size*size*4];
	GLfloat *luminance = new GLfloat[size*size];

	// calculate luminance values
	for (int x = 0; x<size; x++) 
	{
		for (int y = 0; y<size; y++) 
		{
			// set index
			int i = (x*size+y);

			// reset current total Distance
			totalDistance=0.0;
			near1 = maxDist;
			near2 = maxDist;
			far1 = 0.0;
			
			
			// calculate distances to each node
			for(int k=0;k<density;k++)
			{
				tempX = x-nodes[k*2];
				tempY = y-nodes[k*2+1];
				distance[k]=sqrt((double)(tempX*tempX+tempY*tempY));
				totalDistance+=distance[k];
				if(distance[k]<=near1)		{near1 = distance[k];}
				else if(distance[k]<=near2)	{near2 = distance[k];}
				else if(distance[k]>=far1)	{far1 = distance[k];}
			}
			
			// calcuate lumminance
			luminance[i] = pow((double)near1,1.5);
			
			// remember the longest distance (used for normalisation)
			if(luminance[i]>far2)	{far2=luminance[i];}
		}
	}

	// normalize and assign colors
	for (int x = 0; x<size; x++) 
	{
		for (int y = 0; y<size; y++) 
		{
			// calculate indexes
			int i = (x*size+y)*4;
			int j = (x*size+y);
			
			// normalize lumminance
			float tempLuminance = luminance[j]/far2;
			
			// normalize and assign colors
			pixels[i+0] = (GLubyte)((tempLuminance*color1[0] + ((float)1-tempLuminance)*color2[0])*(float)255);
			pixels[i+1] = (GLubyte)((tempLuminance*color1[1] + ((float)1-tempLuminance)*color2[1])*(float)255);
			pixels[i+2] = (GLubyte)((tempLuminance*color1[2] + ((float)1-tempLuminance)*color2[2])*(float)255);
			pixels[i+3] = (GLubyte)((tempLuminance*color1[3] + ((float)1-tempLuminance)*color2[3])*(float)255);
			
		}
	}

	
	glGenTextures(1,&result);
    glBindTexture(GL_TEXTURE_2D,result);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,size,size,0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	// free memory
	delete[] pixels;
	delete[] luminance;

	printf(" done.\n");

	return result;
}

GLuint genCheckerboard(int density, int size, GLfloat color1[4], GLfloat color2[4])
{
	printf("Generating checkerboard texture...");
	fflush(stdout);

	GLuint result;

	GLubyte *pixels = new GLubyte[size*size*4];
	
	int passes = size/density;
	
	for (int x = 0; x<size; x++) 
	{
		for (int y = 0; y<size; y++) 
		{
			// calculate index
			int i = (x*size+y)*4;
			
			// decide which color to use
			if((((x/passes)+(y/passes))%2)==0)
			{
				pixels[i+0] = (GLubyte)(color1[0]*(float)255);
				pixels[i+1] = (GLubyte)(color1[1]*(float)255);
				pixels[i+2] = (GLubyte)(color1[2]*(float)255);
				pixels[i+3] = (GLubyte)(color1[3]*(float)255);
			}
			else
			{
				pixels[i+0] = (GLubyte)(color2[0]*(float)255);
				pixels[i+1] = (GLubyte)(color2[1]*(float)255);
				pixels[i+2] = (GLubyte)(color2[2]*(float)255);
				pixels[i+3] = (GLubyte)(color2[3]*(float)255);
			}
		}
	}


	glGenTextures(1,&result);
    glBindTexture(GL_TEXTURE_2D,result);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,size,size,0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	// free memory
	delete[] pixels;

	

	printf(" done.\n");

	return result;
}


GLuint genColorMap(bool gradient, double level, int size)
{
	printf("Generating colorMap texture...");
	fflush(stdout);


	double pass = 6.0/(double)size;
	int periode = size/6;
	int index = 0;

	double red  = 1.0;
	double green= 1.0-(double)periode*pass;
	double blue = 1.0-(double)periode*pass;
	float alpha = 0.0;

	GLuint result;
	GLfloat *colors = new GLfloat[size*4];
	GLubyte *pixels = new GLubyte[size*size*4];
	

	for(int i=0;i<periode;i++)
	{
		// assign color
		colors[index+0]=red;
		colors[index+1]=green;
		colors[index+2]=blue;
		colors[index+3]=0.0;

		green += pass;

		index += 4;
	}
	for(int i=0;i<periode;i++)
	{
		// assign color
		colors[index+0]=red;
		colors[index+1]=green;
		colors[index+2]=blue;
		colors[index+3]=0.0;
		
		red -= pass;

		index += 4;
	}
	for(int i=0;i<periode;i++)
	{
		// assign color
		colors[index+0]=red;
		colors[index+1]=green;
		colors[index+2]=blue;
		colors[index+3]=0.0;

		blue += pass;

		index += 4;
	}
	for(int i=0;i<periode;i++)
	{
		// assign color
		colors[index+0]=red;
		colors[index+1]=green;
		colors[index+2]=blue;
		colors[index+3]=0.0;

		green -= pass;

		index += 4;
	}
	for(int i=0;i<periode;i++)
	{
		

		// assign color
		colors[index+0]=red;
		colors[index+1]=green;
		colors[index+2]=blue;
		colors[index+3]=0.0;

		red += pass;

		index += 4;
	}
	for(int i=0;i<periode;i++)
	{
		// assign color
		colors[index+0]=red;
		colors[index+1]=green;
		colors[index+2]=blue;
		colors[index+3]=0.0;

		blue -= pass;

		index += 4;
	}
		
		
		


	// fill the rest of the array with red
	for(int i=0;i<(size-(6*periode));i++)
	{
		// assign color
		colors[index+0]=1.0;
		colors[index+1]=1.0-(double)periode*pass;
		colors[index+2]=1.0-(double)periode*pass;
		colors[index+3]=0.0;

		index += 4;
	}

	for (int x = 0; x<size; x++) 
	{
		if(gradient)
		{
			alpha= pow((float)(abs((size/2)-x))/(float)(size/2),(float)level);
		}
		for (int y = 0; y<size; y++) 
		{
			// set indexes
			int i = (x*size+y)*4;
			int j = y*4;

			
			pixels[i+0]=(GLubyte)(  ( colors[j+0]*((float)1-alpha) + ((float)x/(float)size)*alpha )  *(float)255);
			pixels[i+1]=(GLubyte)(  ( colors[j+1]*((float)1-alpha) + ((float)x/(float)size)*alpha )  *(float)255);
			pixels[i+2]=(GLubyte)(  ( colors[j+2]*((float)1-alpha) + ((float)x/(float)size)*alpha )  *(float)255);
			pixels[i+3]=0;
		}
	}


	glGenTextures(1,&result);
    glBindTexture(GL_TEXTURE_2D,result);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,size,size,0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	delete[] pixels;
	delete[] colors;

	

	printf(" done.\n");

	return result;
}



GLuint genExpSprite(int size,float factor)
{
	printf("Generating Exponential Sprite...");
	fflush(stdout);


	float distance=0.0f;
	float tempDistance=0.0f;
	float tempX=0.0f;
	float tempY=0.0f;

	GLuint result;
	GLubyte *pixels = new GLubyte[size*size];
	

	for (int x = 0; x<size; x++) 
	{
		for (int y = 0; y<size; y++) 
		{
			// set indexes
			int i = (x*size+y);

			// calculate length to center of the texture
			tempX = (float)x-(float)(size/2);
			tempY = (float)y-(float)(size/2);
			tempDistance = sqrt((tempX*tempX+tempY*tempY));
			distance=1.0f-(tempDistance/((float)size/2.0f));
			
			if(distance>0.0f)
			{
				distance=pow(distance,factor);
				pixels[i]=(GLuint)(distance*255.0f);
			}
			else
			{
				pixels[i]=0;
			}

			//printf("dist=%f\n",distance);
		}
	}


	glGenTextures(1,&result);
    	glBindTexture(GL_TEXTURE_2D,result);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,size,size,0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
	
	delete[] pixels;
	

	printf(" done.\n");

	return result;
}



GLuint genWhiteCircle(int size)
{
	printf("Generating White Circulat Sprite...");
	fflush(stdout);


	float distance=0.0f;
	float tempX=0.0f;
	float tempY=0.0f;

	GLuint result;
	GLubyte *pixels = new GLubyte[size*size*4];
	

	for (int x = 0; x<size; x++) 
	{
		for (int y = 0; y<size; y++) 
		{
			// set indexes
			int i = (x*size+y)*4;

			// calculate length to center of the texture
			tempX = (float)x-(float)(size/2);
			tempY = (float)y-(float)(size/2);
			distance = sqrt((tempX*tempX+tempY*tempY));
			
			
			if(distance<=((float)size/2.0f))
			{
				pixels[i+0]=(GLuint)((1.0f-(distance/(float)size/2.0f))*255.0f);
				pixels[i+1]=(GLuint)((1.0f-(distance/(float)size/2.0f))*255.0f);
				pixels[i+2]=(GLuint)((1.0f-(distance/(float)size/2.0f))*255.0f);
				pixels[i+3]=0;
			}
			else
			{
				pixels[i+0]=0;
				pixels[i+1]=0;
				pixels[i+2]=0;
				pixels[i+3]=0;
			}
		}
	}


	glGenTextures(1,&result);
    	glBindTexture(GL_TEXTURE_2D,result);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8,size,size,0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	delete[] pixels;
	

	printf(" done.\n");

	return result;
}
