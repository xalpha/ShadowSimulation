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

#include "Mesh.hpp"

Mesh::Mesh() :
    vertices(0),
    vertexCount(0),
    indices(0),
    indexCount(0),
    normals(0),
    normalCount(0),
    faceNormals(0),
    faceNormalCount(0),
    texCoords(0),
    texCoordCount(0),
    colors(0),
    colorCount(0)
{
}


Mesh::~Mesh()
{
    if(vertexCount != 0) delete [] vertices;
    if(indexCount != 0) delete [] indices;
    if(normalCount != 0) delete [] normals;
    if(faceNormalCount !=0) delete [] faceNormals;
    if(texCoordCount != 0) delete [] texCoords;
    if(colorCount != 0) delete [] colors;
}


Mesh::Mesh(unsigned int meshType) :
    vertices(0),
    vertexCount(0),
    indices(0),
    indexCount(0),
    normals(0),
    normalCount(0),
    faceNormals(0),
    faceNormalCount(0),
    texCoords(0),
    texCoordCount(0),
    colors(0),
    colorCount(0)
{
	switch(meshType)
	{
		case MESH_TYPE_TRIANGLES : this->meshType = meshType;
		                           vertexPerFace = 3;
		                           break;
		                           
		case MESH_TYPE_QUADS     : this->meshType = meshType;
		                           vertexPerFace = 4;
		                           break;

		default : printf("Invalid Mesh Type, must be MESH_TYPE_TRIANGLES, or MESH_TYPE_QUADS!");
	}
}


void Mesh::calculateFaceNormals()
{
	// check if there is any data
	if(vertexCount <= 0)
	{
		printf("Warning: Please load a mesh first!");
		return;
	}

	// delete old face normals
	if(faceNormalCount!=0)
	{
		//printf("deleting old faceNormals\n");
        delete [] faceNormals;
		faceNormalCount=0;
	}
	
	
	
	// check if the mesh uses an index Array or if the faces store the vertices sequentially
	if(indexCount > 0)
	{


		faceNormalCount = indexCount;
		faceNormals = new float[faceNormalCount * 3];

		
		
		float *a = new float[3];
		float *b = new float[3];
	
		int index;
		
		// init boath normal arrays with 0.0
		for(int i=0;i<faceNormalCount*3;i++){ faceNormals[i]=0.0f; }
	
		// calculate face normals
		for(int i=0;i<faceNormalCount;i++)
		{
	        index=i*vertexPerFace;
	        
	        // a= B-A
			a[0]=vertices[ indices[index+1]*3 +0] - vertices[ indices[index+0]*3 +0];
			a[1]=vertices[ indices[index+1]*3 +1] - vertices[ indices[index+0]*3 +1];
			a[2]=vertices[ indices[index+1]*3 +2] - vertices[ indices[index+0]*3 +2];
	        
			// b= C-A
	 		b[0]=vertices[ indices[index+2]*3 +0] - vertices[ indices[index+0]*3 +0];
			b[1]=vertices[ indices[index+2]*3 +1] - vertices[ indices[index+0]*3 +1];
			b[2]=vertices[ indices[index+2]*3 +2] - vertices[ indices[index+0]*3 +2];
			
			// calculate face normal ( a cross b )
			faceNormals[index+0]=a[1]*b[2]-a[2]*b[1];
			faceNormals[index+1]=a[2]*b[0]-a[0]*b[2];
			faceNormals[index+2]=a[0]*b[1]-a[1]*b[0];
		}
	}
	else
	{
		

		faceNormalCount = vertexCount / vertexPerFace;
		faceNormals = new float[faceNormalCount * 3];

		
		
		float *a = new float[3];
		float *b = new float[3];
	
		int index;
		
		// init boath normal arrays with 0.0
		for(int i=0;i<faceNormalCount*3;i++){ faceNormals[i]=0.0f; }
		
		// calculate face normals
		for(int i=0;i<faceNormalCount;i++)
		{
	        index=i*vertexPerFace;
	        
	        // a= B-A
			a[0]=vertices[ (index+1)*3 +0 ] - vertices[ (index+0)*3 +0 ];
	 		a[1]=vertices[ (index+1)*3 +1 ] - vertices[ (index+0)*3 +1 ];
	 		a[2]=vertices[ (index+1)*3 +2 ] - vertices[ (index+0)*3 +2 ];
	        
			// b= C-A
	 		b[0]=vertices[ (index+2)*3 +0 ] - vertices[ (index+0)*3 +0 ];
	 		b[1]=vertices[ (index+2)*3 +1 ] - vertices[ (index+0)*3 +1 ];
	 		b[2]=vertices[ (index+2)*3 +2 ] - vertices[ (index+0)*3 +2 ];
						
			// calculate face normal ( a cross b )
			faceNormals[i*3+0]=a[1]*b[2]-a[2]*b[1];
			faceNormals[i*3+1]=a[2]*b[0]-a[0]*b[2];
			faceNormals[i*3+2]=a[0]*b[1]-a[1]*b[0];
		}
	}
	
	double vLength;
	
	// normlaize face normals
	for(int i=0;i<faceNormalCount;i++)
	{
        
		// calculate length of vertex normal 
		vLength = 0.0;
		vLength += faceNormals[i*3+0]*faceNormals[i*3+0];
		vLength += faceNormals[i*3+1]*faceNormals[i*3+1];
		vLength += faceNormals[i*3+2]*faceNormals[i*3+2];
		vLength = sqrt(vLength);
		//printf( "Normale Vertex %d: %f\t%f\t%f\n", i, normalVer[index+0], normalVer[index+1], normalVer[index+2]);
		
		// normalize vertex normal
		faceNormals[i*3+0] /= (float)vLength;
		faceNormals[i*3+1] /= (float)vLength;
		faceNormals[i*3+2] /= (float)vLength;
	}
}


void Mesh::calculateNormals()
{
	// check if there is any data
	if(vertexCount <= 0)
	{
		printf("Warning: Unable to calculate normals! Please load a mesh first!");
		return;
	}

	// delete old normals
	if(normalCount!=0)
	{
		delete [] normals;
		normalCount=0;
	}
	
	// check if face normals have allready been calculated
	calculateFaceNormals();
	
	// check if the mesh uses an index Array or if the faces store the vertices sequentially
	if(indexCount > 0)
	{
		normalCount = vertexCount;
		normals = new float[normalCount * 3];
		
		int index;
		
		// init normal array with 0.0
		for(int i=0;i<normalCount*3;i++) { normals[i] = 0.0f; }
	
		// calculate vertex normals
		for(int i=0;i<faceNormalCount;i++)
		{
	        index=i*vertexPerFace;
	        
			// add face normal to the normals of all vertices of this triangle
			for(int j=0; j<vertexPerFace; j++)
			{
				normals[ indices[index+j]*3 +0] += faceNormals[index+0];
				normals[ indices[index+j]*3 +1] += faceNormals[index+1];
				normals[ indices[index+j]*3 +2] += faceNormals[index+2];
			}
		}
	}
	else
	{
		normalCount = vertexCount;
		normals = new float[normalCount * 3];
		
		int index;
		
		// init normal array with 0.0
		for(int i=0;i<normalCount*3;i++) { normals[i] = 0.0f; }
		
		// calculate vertex normals
		for(int i=0;i<faceNormalCount;i++)
		{
	        index=i*vertexPerFace;
	        
			// find all other copies of these vertices and add the face normal to their normals
			// TODO try to find a better way, it's horribly slow, O(n^2)
			for(int j=0; j<vertexPerFace; j++)
			{
				float *currentFaceVertex = &vertices[ (index+j)*3];

				for(int k=0; k<vertexCount; k++)
				{
					float *currentVertex = &vertices[k*3];

					// values are not exactly the same
					float precision = 0.001f;

					bool identicalVertices = ( ( currentFaceVertex[0] - currentVertex[0] < precision) ||
					                           ( currentFaceVertex[0] - currentVertex[0] <-precision) ) &&
					                         ( ( currentFaceVertex[1] - currentVertex[1] < precision) ||
					                           ( currentFaceVertex[1] - currentVertex[1] <-precision) )&&
					                         ( ( currentFaceVertex[2] - currentVertex[2] < precision) ||
					                           ( currentFaceVertex[2] - currentVertex[2] <-precision) );

					//printf("condition= %i\n",(int) identicalVertices );

					// check wether the vertices are have the same value
					if( identicalVertices )
					{
						// if so then add the face normal to their vertex normal
						normals[ k*3 +0 ] += faceNormals[i*3+0];
						normals[ k*3 +1 ] += faceNormals[i*3+1];
						normals[ k*3 +2 ] += faceNormals[i*3+2]; 
					}
				}
			}
		}
	}

	double vLength;
	
	// normlaize vertex normals
	for(int i=0;i<normalCount;i++)
	{
		// calculate length of vertex normal 
		vLength = 0.0;
		vLength += (double)normals[i*3+0]*normals[i*3+0];
		vLength += (double)normals[i*3+1]*normals[i*3+1];
		vLength += (double)normals[i*3+2]*normals[i*3+2];
		vLength = sqrt(vLength);

		// normalize vertex normal
		normals[i*3+0] /= (float)vLength;
		normals[i*3+1] /= (float)vLength;
		normals[i*3+2] /= (float)vLength;

		//printf( "Normal Vertex %i: %f\t%f\t%f\n", i, normals[i*3+0], normals[i*3+1], normals[i*3+2]);
	}
}


void Mesh::computeTextureAtlas( int g_iShadowTexSize, int g_iShadowTexRowCount, float** g_fpCameraMatrices, float** g_fpCameraClipPlanes, float g_fTexelOffset )
{
    // re-init stuff
    if( texCoords != 0 ) delete [] texCoords;
    if( *g_fpCameraMatrices != 0 ) delete [] *g_fpCameraMatrices;
    if( *g_fpCameraClipPlanes != 0 ) delete [] *g_fpCameraClipPlanes;
    texCoords = new float[vertexCount*2];
    (*g_fpCameraMatrices) = new float[indexCount*16];
    (*g_fpCameraClipPlanes) = new float[indexCount*4];

    // set some pointers
    float* cameraMatrices = (*g_fpCameraMatrices);
    float* cameraClipPlanes = (*g_fpCameraClipPlanes);

    // calculate texture coordinates and camera matrices
    for(int i=0; i<vertexCount/4;i++)
    {
        // get the vertices
        float *a = &vertices[(i*4+0)*3];
        float *b = &vertices[(i*4+1)*3];
        float *c = &vertices[(i*4+2)*3];
        float *d = &vertices[(i*4+3)*3];

        // calculate A = b-a and B = c-a
        float A[3] = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };
        float B[3] = { c[0]-a[0], c[1]-a[1], c[2]-a[2] };

        // calculate normal to the triangle
        float N[3] = { A[1]*B[2]-A[2]*B[1], A[2]*B[0]-A[0]*B[2], A[0]*B[1]-A[1]*B[0] };

        // calculate normal to the plane between the triangle normal and A
        float Bp[3] = { N[1]*A[2]-N[2]*A[1], N[2]*A[0]-N[0]*A[2], N[0]*A[1]-N[1]*A[0] };

        // normalize A, N and Bp as the will form a new rotation matrix and thus should be normal
        float lengthA = sqrt(A[0]*A[0]+A[1]*A[1]+A[2]*A[2]);
        A[0]/=lengthA;
        A[1]/=lengthA;
        A[2]/=lengthA;
        float lengthN = sqrt(N[0]*N[0]+N[1]*N[1]+N[2]*N[2]);
        N[0]/=lengthN;
        N[1]/=lengthN;
        N[2]/=lengthN;
        float lengthBp= sqrt(Bp[0]*Bp[0]+Bp[1]*Bp[1]+Bp[2]*Bp[2]);
        Bp[0]/=lengthBp;
        Bp[1]/=lengthBp;
        Bp[2]/=lengthBp;

        // define and store rotation matrix
        float M[16]={ A[0],Bp[0],N[0],0.0f,
                      A[1],Bp[1],N[1],0.0f,
                      A[2],Bp[2],N[2],0.0f,
                      0.0f,0.0f, 0.0f,1.0f	};
        for(int j=0;j<16;j++)
            cameraMatrices[ i*16 + j ] = M[j];

        // calculate screen coordinates
        float Sa[2] = { A[0]*a[0] + A[1]*a[1] + A[2]*a[2],  Bp[0]*a[0] + Bp[1]*a[1] + Bp[2]*a[2]};
        float Sb[2] = { A[0]*b[0] + A[1]*b[1] + A[2]*b[2],  Bp[0]*b[0] + Bp[1]*b[1] + Bp[2]*b[2]};
        float Sc[2] = { A[0]*c[0] + A[1]*c[1] + A[2]*c[2],  Bp[0]*c[0] + Bp[1]*c[1] + Bp[2]*c[2]};
        float Sd[2] = { A[0]*d[0] + A[1]*d[1] + A[2]*d[2],  Bp[0]*d[0] + Bp[1]*d[1] + Bp[2]*d[2]};

        // calculate clip planes
        float minX = std::min(Sa[0],std::min(Sb[0],std::min(Sc[0],Sd[0])));
        float maxX = std::max(Sa[0],std::max(Sb[0],std::max(Sc[0],Sd[0])));
        float minY = std::min(Sa[1],std::min(Sb[1],std::min(Sc[1],Sd[1])));
        float maxY = std::max(Sa[1],std::max(Sb[1],std::max(Sc[1],Sd[1])));

        // store clip planes
        cameraClipPlanes[i*4 +0]= minX;
        cameraClipPlanes[i*4 +1]= maxX;
        cameraClipPlanes[i*4 +2]= minY;
        cameraClipPlanes[i*4 +3]= maxY;

        // calculate texture coordinates
        texCoords[ (i*4+0)*2 +0] = (Sa[0]-minX)/(maxX-minX);
        texCoords[ (i*4+0)*2 +1] = (Sa[1]-minY)/(maxY-minY);

        texCoords[ (i*4+1)*2 +0] = (Sb[0]-minX)/(maxX-minX);
        texCoords[ (i*4+1)*2 +1] = (Sb[1]-minY)/(maxY-minY);

        texCoords[ (i*4+2)*2 +0] = (Sc[0]-minX)/(maxX-minX);
        texCoords[ (i*4+2)*2 +1] = (Sc[1]-minY)/(maxY-minY);

        texCoords[ (i*4+3)*2 +0] = (Sd[0]-minX)/(maxX-minX);
        texCoords[ (i*4+3)*2 +1] = (Sd[1]-minY)/(maxY-minY);

        // move the texture coordinates to the inside (prevent interpolating with the next texel)
        float textureResolution= (float)g_iShadowTexSize / (float)g_iShadowTexRowCount;
        float scale = 1.0f - ((g_fTexelOffset*2.0f) / textureResolution);	// scaling down the texCoords with one texel;

        // get the texture coordinates of the quad
        float *texA = &texCoords[ (i*4+0)*2];
        float *texB = &texCoords[ (i*4+1)*2];
        float *texC = &texCoords[ (i*4+2)*2];
        float *texD = &texCoords[ (i*4+3)*2];

        // calculate old center of the primitive
        float oldCenter[3] = {(texA[0]+texB[0]+texC[0]+texD[0])/4.0f, (texA[1]+texB[1]+texC[1]+texD[1])/4.0f};

        for(int j=0; j<8; j++)
            texA[j] *= scale;

        // calculate new center of the primitive
        float newCenter[3] = {(texA[0]+texB[0]+texC[0]+texD[0])/4.0f, (texA[1]+texB[1]+texC[1]+texD[1])/4.0f};

        // calculate the diference between the two centers
        float delta[2] = {oldCenter[0] - newCenter[0], oldCenter[1] - newCenter[1]};

        // translate the new coordinates back to the old center
        for(int j=0; j<4; j++)
        {
            texA[j*2+0] += delta[0];
            texA[j*2+1] += delta[1];
        }

        // scale and translate texture coordinates into the page texture
        int index = (i)%(g_iShadowTexRowCount*g_iShadowTexRowCount);
        int tempRow = index / g_iShadowTexRowCount;
        int tempColumn = index % g_iShadowTexRowCount;

        texCoords[ (i*4+0)*2 +0] = ( texCoords[ (i*4+0)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
        texCoords[ (i*4+0)*2 +1] = ( texCoords[ (i*4+0)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

        texCoords[ (i*4+1)*2 +0] = ( texCoords[ (i*4+1)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
        texCoords[ (i*4+1)*2 +1] = ( texCoords[ (i*4+1)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

        texCoords[ (i*4+2)*2 +0] = ( texCoords[ (i*4+2)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
        texCoords[ (i*4+2)*2 +1] = ( texCoords[ (i*4+2)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

        texCoords[ (i*4+3)*2 +0] = ( texCoords[ (i*4+3)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
        texCoords[ (i*4+3)*2 +1] = ( texCoords[ (i*4+3)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;
    }

    // update the VBO's
    updateVBO();
}


void Mesh::invertFaces()
{
	bool invertVertices = vertexCount > 0;
	bool invertNormals = normalCount > 0;
	bool invertTexCoords = texCoordCount > 0;
	bool invertColors = colorCount > 0;

	if(!invertVertices)
	{
		printf("Warning: Unable to invert Faces! Please load a mesh first!");
		return;
	}

	// check if it is an indexed array
	if(indexCount == 0)
	{
		// vertices and normals

		float *tempVertexArray = new float[vertexPerFace*3];
		float *tempNormalArray;
		float *tempTexCoordArray;
		float *tempColorArray;

		if(invertNormals)	tempNormalArray = new float[vertexPerFace*3];
		if(invertTexCoords)	tempTexCoordArray = new float[vertexPerFace*2];
		if(invertColors)	tempColorArray = new float[vertexPerFace*colorComponentCount];

		for(int i=0; i<vertexCount/vertexPerFace; i++)
		{
			for(int j=0; j<vertexPerFace; j++)
			{
				for(int k=0; k<3; k++)	tempVertexArray[j*3 +k] = vertices[(i*vertexPerFace+j)*3 + k];
				
				if(invertNormals)	for(int k=0; k<3; k++)	tempNormalArray[j*3 +k] = normals[(i*vertexPerFace+j)*3 + k];
				
				if(invertTexCoords)	for(int k=0; k<2; k++)	tempTexCoordArray[j*2 +k] = texCoords[(i*vertexPerFace+j)*2 + k];

				if(invertColors)	for(int k=0; k<colorComponentCount; k++)	tempColorArray[j*colorComponentCount +k] = colors[(i*vertexPerFace+j)*colorComponentCount + k];
			}

			// TODO permutate...

			for(int j=0; j<vertexPerFace; j++)
			{
				for(int k=0; k<3; k++)	vertices[(i*vertexPerFace+j)*3 + k] = tempVertexArray[(((vertexPerFace-1-j)+(vertexPerFace-2))%vertexPerFace)*3 + k];

				if(invertNormals)	for(int k=0; k<3; k++)	normals[(i*vertexPerFace+j)*3 + k] = tempNormalArray[(((vertexPerFace-1-j)+(vertexPerFace-2))%vertexPerFace)*3 + k];

				if(invertTexCoords)	for(int k=0; k<2; k++)	texCoords[(i*vertexPerFace+j)*2 + k] = tempTexCoordArray[(((vertexPerFace-1-j)+(vertexPerFace-2))%vertexPerFace)*2 + k];

				if(invertColors)	for(int k=0; k<colorComponentCount; k++)	colors[(i*vertexPerFace+j)*colorComponentCount + k] = tempColorArray[(((vertexPerFace-1-j)+(vertexPerFace-2))%vertexPerFace)*colorComponentCount + k];

			}
		}

		delete [] tempVertexArray;
		if(invertNormals)	delete [] tempNormalArray;
		if(invertTexCoords)	delete [] tempTexCoordArray;
		if(invertColors)	delete [] tempColorArray;
	}
	else
	{
		// TODO same stuff but with the index
	}
}


void Mesh::invertNormals()
{
	bool invertVertices = vertexCount > 0;
	bool invertNormlas = normalCount > 0;
	bool invertFaceNormals = faceNormalCount > 0;

	if(!invertVertices)
	{
		printf("Warning: Unable to invert Faces! Please load a mesh first!");
		return;
	}

	// invert normals
	if(invertFaceNormals)
	{
		for(int i=0; i<normalCount; i++)
		{
			for(int j=0; j<3; j++)
			{
				normals[i*3+j] *= -1.0f;
			}
		}
	}

	// invert face normals
	if(invertFaceNormals)
	{
		for(int i=0; i<faceNormalCount; i++)
		{
			for(int j=0; j<3; j++)
			{
				faceNormals[i*3+j] *= -1.0f;
			}
		}
	}
}


void Mesh::analyzeMesh()
{
	// check if there is any data
	if(vertexCount <= 0)	return;
	
	float minX=vertices[0];
	float minY=vertices[1];
	float minZ=vertices[2];
	float maxX=vertices[0];
	float maxY=vertices[1];
	float maxZ=vertices[2];

	for(int i=0; i<vertexCount*3; i+=3)
	{
		if(vertices[i] < minX)	minX=vertices[i];
		if(vertices[i+1] < minY)	minY=vertices[i+1];
		if(vertices[i+2] < minZ)	minZ=vertices[i+2];

		if(vertices[i] > maxX)	maxX=vertices[i];
		if(vertices[i+1] > maxY)	maxY=vertices[i+1];
		if(vertices[i+2] > maxZ)	maxZ=vertices[i+2];
	}

	sizeX = sqrt(pow(maxX - minX,2.0f));
	sizeY = sqrt(pow(maxY - minY,2.0f));
	sizeZ = sqrt(pow(maxZ - minZ,2.0f));
	
	centerX = (maxX+minX)/2.0f;
	centerY = (maxY+minY)/2.0f;
	centerZ = (maxZ+minZ)/2.0f;

	//printf("%f %f %f\n",centerX, centerY, centerZ);
}


void Mesh::centerMesh(float x, float y, float z)
{
	// check if there is any data
	if(vertexCount <= 0)	return;

	for(int i=0;i<vertexCount*3;i+=3)
	{
		vertices[i+0]-=	centerX - x;
		vertices[i+1]-=	centerY - y;
		vertices[i+2]-=	centerZ - z;
	}
	
	centerX = x;
	centerY = y;
	centerZ = z;
}


void Mesh::centerMesh()
{
	centerMesh(0.0f,0.0f,0.0f);
}


void Mesh::resizeMesh(float newSizeX, float newSizeY, float newSizeZ)
{
	// check if there is any data
	if(vertexCount <= 0)	return;
	
	// check weather the object is at (0,0,0)
	if(centerX == 0.0f && centerY == 0.0f && centerZ == 0.0f)
	{
		// resize the Mesh
		for(int i=0;i<vertexCount*3;i+=3)
		{
			vertices[i]	*= newSizeX/sizeX;
			vertices[i+1]*=	newSizeX/sizeY;
			vertices[i+2]*=	newSizeZ/sizeZ;
		}	
	}
	else
	{
		// center mesh to (0,0,0) in order to get a nice scale in object space
		float oldX=centerX;
		float oldY=centerY; 
		float oldZ=centerZ;
		centerMesh(0.0f,0.0f,0.0f);
		
		// resize the Mesh
		for(int i=0;i<vertexCount*3;i+=3)
		{
			vertices[i]	*= newSizeX/sizeX;
			vertices[i+1]*=	newSizeY/sizeY;
			vertices[i+2]*=	newSizeZ/sizeZ;
		}
		
		// bring mesh back to its previous position
		centerMesh(oldX, oldY, oldZ);
	}
	
	sizeX = newSizeX;
	sizeY = newSizeY;
	sizeZ = newSizeZ;
}


void Mesh::resizeMesh(float newSize)
{
	// determine which one is bigger
	float maxSize;
	maxSize = sizeX;
	if(sizeY > maxSize)	maxSize = sizeY;
	if(sizeZ > maxSize)	maxSize = sizeZ;
	
	// calculate the actual sizes
	float tempX = sizeX * (newSize / maxSize);
	float tempY = sizeY * (newSize / maxSize);
	float tempZ = sizeZ * (newSize / maxSize);
	
	// resize the mesh
	resizeMesh(tempX, tempY, tempZ);
}


void Mesh::translateMesh(float transX, float transY, float transZ)
{
	// translate all vertices
	for(int i=0; i< vertexCount; i++)
	{
		vertices[i*3+0] += transX;
		vertices[i*3+1] += transY;
		vertices[i*3+2] += transZ;
	}

	// update the new center of the mesh
	centerX += transX;
	centerY += transY;
	centerZ += transZ;
}


void Mesh::rotateMesh(float rotX, float rotY, float rotZ) 
{
	float rotationMatrix[16];
	float tempMatrix[16];

	rotationMatrix[0]= 1.0f;	rotationMatrix[4]= 0.0f;	rotationMatrix[8] = 0.0f;	rotationMatrix[12] = 0.0f;
	rotationMatrix[1]= 0.0f;	rotationMatrix[5]= 1.0f;	rotationMatrix[9] = 0.0f;	rotationMatrix[13] = 0.0f;
	rotationMatrix[2]= 0.0f;	rotationMatrix[6]= 0.0f;	rotationMatrix[10]= 1.0f;	rotationMatrix[14] = 0.0f;
	rotationMatrix[3]= 0.0f;	rotationMatrix[7]= 0.0f;	rotationMatrix[11]= 0.0f;	rotationMatrix[15] = 1.0f;

	// rotate around X-axis
	float rotSin =  (float)sin(-rotX*0.017453292);
	float rotCos =  (float)cos(-rotX*0.017453292);

	tempMatrix[0]= 1.0f;	tempMatrix[4]= 0.0f;		tempMatrix[8] = 0.0f;		tempMatrix[12]= 0.0f;
	tempMatrix[1]= 0.0f;	tempMatrix[5]= rotCos;		tempMatrix[9] = rotSin;		tempMatrix[13]= 0.0f;
	tempMatrix[2]= 0.0f;	tempMatrix[6]= -rotSin;		tempMatrix[10]= rotCos;		tempMatrix[14]= 0.0f;
	tempMatrix[3]= 0.0f;	tempMatrix[7]= 0.0f;		tempMatrix[11]= 0.0f;		tempMatrix[15]= 1.0f;
	multMatrixMatrix(rotationMatrix, tempMatrix, rotationMatrix);

	// rotate around Y-axis
	rotSin = (float)sin(rotY*0.017453292);
	rotCos = (float)cos(rotY*0.017453292);

	tempMatrix[0]= rotCos;	tempMatrix[4]= 0;	tempMatrix[8] = rotSin;	tempMatrix[12]= 0;
	tempMatrix[1]= 0;		tempMatrix[5]= 1;	tempMatrix[9] = 0;		tempMatrix[13]= 0;
	tempMatrix[2]= -rotSin;	tempMatrix[6]= 0;	tempMatrix[10]= rotCos;	tempMatrix[14]= 0;
	tempMatrix[3]= 0;		tempMatrix[7]= 0;	tempMatrix[11]= 0;		tempMatrix[15]= 1;
	multMatrixMatrix(rotationMatrix, tempMatrix, rotationMatrix);

	// rotate around Z-axis
	rotSin = (float)sin(-rotZ*0.017453292);
	rotCos = (float)cos(-rotZ*0.017453292);

	tempMatrix[0]= rotCos;	tempMatrix[4] = rotSin;	tempMatrix[8]  = 0;		tempMatrix[3] = 0;
	tempMatrix[1]= -rotSin;	tempMatrix[5] = rotCos;	tempMatrix[9]  = 0;		tempMatrix[7] = 0;
	tempMatrix[2]= 0;		tempMatrix[6] = 0;		tempMatrix[10] = 1;		tempMatrix[11]= 0;
	tempMatrix[3]= 0;		tempMatrix[7] = 0;		tempMatrix[11] = 0;		tempMatrix[15]= 1;
	multMatrixMatrix(rotationMatrix, tempMatrix, rotationMatrix);

	// update mesh properties
	analyzeMesh();

	// translate to the center (0,0,0)
	translateMesh( -centerX, -centerY, -centerZ);

	// rotate mesh
	for(int i=0; i<vertexCount; i++)	multMatrixVector3(rotationMatrix, &vertices[i*3], &vertices[i*3]);
	for(int i=0; i<normalCount; i++)	multMatrixVector3(rotationMatrix, &normals[i*3], &normals[i*3]);
	for(int i=0; i<faceNormalCount; i++)	multMatrixVector3(rotationMatrix, &faceNormals[i*3], &faceNormals[i*3]);

	// translate back to the old center
	translateMesh(centerX,centerY,centerZ);

	// update data
	analyzeMesh();
}


void Mesh::updateVBO()
{
    // update the vertex buffer objects
    g_oMeshVBO.configure( GL_QUADS, GL_DYNAMIC_DRAW_ARB );
    g_oMeshVBO.initVertices( vertices, vertexCount );
    g_oMeshVBO.initNormals( normals );
    g_oMeshVBO.initTexCoords( texCoords );
}

