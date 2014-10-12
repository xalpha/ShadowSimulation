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

#include "ShadowView.hpp"
#include <GL/glu.h>


ShadowView::ShadowView(QWidget *parent) : QGLWidget(parent)
{
//	// window properties
//	g_iWindowWidth=800;
//	g_iWindowHeight=600;
//	g_fAspect=(float)g_iWindowWidth / (float)g_iWindowHeight;

    // camera properties
    g_fCameraAzimuth = 0.0f;
    g_fCameraElevation = 0.0f;
    g_fCameraZoom = -3.5f;

//	// mouse handling
//	g_iMousePosX = 0;
//	g_iMousePosY = 0;

//	// key handling
//	setFocusPolicy(Qt::StrongFocus);

//	// FBO's
//    g_iShadowTexSize=4096;
//	g_iShadowTexRowCount=8;
//    g_iGroundShadowTexSize=4096;
//	g_iShadowTextureCount = 0;
//	g_fTexelOffset = 0.5f;
	
//    // shadowing period
//	g_sStartDate.init(2000,1,1,12,0,0);
//	g_sStopDate.init(2000,1,1,13,0,0);
//	g_iSteps=1;

//	// shadowing period
//	g_dFPS=0;
//	g_dLastTime=0;
//	g_dStartDate=0;
//	g_dTimePerFrame=0;
//	g_iSteps=1;

//	// model
//	g_oMesh = NULL;

//	// shader properties
//	g_bSmoothShading = false;
//	g_fSceneRenderingMode = 0.0f;
}


void ShadowView::setMesh( std::weak_ptr<Mesh> mesh )
{
    m_mesh = mesh;
}


void ShadowView::initializeGL()
{
    // initialize glew
    GLenum error = glewInit();
    if (GLEW_OK != error)
        std::cout << glewGetErrorString(error) << std::endl;
}


void ShadowView::paintGL()
{
    // setup the view matrix
    setupUserViewProj();

    // enable Texturing
    glEnable(GL_TEXTURE_2D);

    // Clear Buffer for new frame
    glClearColor(0.0f ,0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render Background
    renderBackground();

//    // render the sun
//    renderSun();

    // Modelview Transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( 0.0f, 0.0f, g_fCameraZoom); // Zoom
    glRotatef(g_fCameraElevation, 1.0f, 0.0f, 0.0f);  // rotate elevation
    glRotatef(g_fCameraAzimuth, 0.0f, 1.0f, 0.0f);    // rotate azimuth
    //glTranslatef( 0.0f, -g_fGroundLevel, 0.0f);   // Lower the whole scene so that the center of the Mesh overlappes with the center of rotation

    std::cout << g_fCameraZoom << " \t" << g_fCameraElevation << " \t" << g_fCameraAzimuth << std::endl;
    std::cout.flush();

    renderGround();

    if( !m_mesh.expired() )
    {
        // enable sceneRendering Shader
        //sceneRendering.enable();

        // setup shader parameters
        // TODO not elegant! try to do with uniform variabiles
        //glColor4f(g_fSceneRenderingMode/4.0f, 0.0f, 0.0f, 0.0f);

        // render the scene
        //m_mesh.lock()->draw();

        //sceneRendering.disable();
    }
}

void ShadowView::resizeGL(int width,int height)
{
    // update viewport dimensions
    g_iWindowWidth = width;
    g_iWindowHeight = height;
    g_fAspect = (float)width/(float)height;
}


//void ShadowView::initModel(char *fileName)
//{
//	printf( "Initializing Model...");
//	fflush(stdout);

//	// delete previously loaded model
//	if(g_oMesh!=NULL)	delete g_oMesh;

//    // find out that type this is
//    std::string filename_str(fileName);
//    std::string extension = filename_str.substr( filename_str.find_last_of('.'), filename_str.size()-filename_str.find_last_of('.') );
//    printf("%s##\n",extension.c_str());
	
//    // Load Model
//    if( extension.compare(".obj") == 0 )
//        g_oMesh = new MeshOBJ;
//    else if( extension.compare(".dxf") == 0 )
//        g_oMesh = new MeshDXF(Mesh::MESH_TYPE_QUADS);
//	g_oMesh->loadMesh(fileName);
//	g_oMesh->calculateFaceNormals();
//	g_oMesh->resizeMesh(1.0f);
//	g_oMesh->centerMesh(0.0f,g_oMesh->getSizeY()/2.0f,0.0f);
//	g_fGroundLevel = g_oMesh->getSizeY()/2.0f;

//	// delete old data
//	if(g_iTexCoordCount!=0)
//	{
//		delete [] g_fpTexCoords;
//		g_iTexCoordCount=0;
//	}

//	if(g_iCameraMatrixCount!=0)
//	{
//		delete [] g_fpCameraMatrices;
//		g_iCameraMatrixCount=0;
//	}

//	if(g_iCameraClipPlaneCount!=0)
//	{
//		delete [] g_fpCameraClipPlanes;
//		g_iCameraClipPlaneCount=0;
//	}

//	// initialize Arrays
//	g_iVertexCount = g_oMesh->getVertexCount();
//	g_fpVertices = g_oMesh->getVertices();
////printf("g_iVertexCount = %i\n",g_iVertexCount);
	
//	g_iNormalCount = g_oMesh->getNormalCount();
//	g_fpNormals = g_oMesh->getNormals();
	
//	g_iFaceNormalCount = g_oMesh->getFaceNormalCount();
//	g_fpFaceNormals = g_oMesh->getFaceNormals();
////printf("g_iFaceNormalCount = %i\n",g_iFaceNormalCount);
	
//	g_iTexCoordCount = g_iVertexCount;
//	g_fpTexCoords = new float[g_iTexCoordCount*2];
////printf("g_iTexCoordCount = %i\n",g_iTexCoordCount);
	
//	g_iCameraMatrixCount = g_iFaceNormalCount;
//	g_fpCameraMatrices = new float[g_iCameraMatrixCount*16];
////printf("g_iCameraMatrixCount = %i\n",g_iCameraMatrixCount);

	
//	g_iCameraClipPlaneCount = g_iFaceNormalCount;
//	g_fpCameraClipPlanes = new float[g_iCameraClipPlaneCount*4];
////printf("g_iCameraClipPlaneCount = %i\n",g_iCameraClipPlaneCount);


//	// calculate texture coordinates and camera matrices
//	for(int i=0; i<g_iVertexCount/4;i++)
//	{
//		// get the vertices
//		float *a = &g_fpVertices[(i*4+0)*3];
//		float *b = &g_fpVertices[(i*4+1)*3];
//		float *c = &g_fpVertices[(i*4+2)*3];
//		float *d = &g_fpVertices[(i*4+3)*3];

//		// calculate A = b-a and B = c-a
//		float A[3] = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };
//		float B[3] = { c[0]-a[0], c[1]-a[1], c[2]-a[2] };

//		// calculate normal to the triangle
//		float N[3] = { A[1]*B[2]-A[2]*B[1], A[2]*B[0]-A[0]*B[2], A[0]*B[1]-A[1]*B[0] };

//		// calculate normal to the plane between the triangle normal and A
//		float Bp[3] = { N[1]*A[2]-N[2]*A[1], N[2]*A[0]-N[0]*A[2], N[0]*A[1]-N[1]*A[0] };

//		// normalize A, N and Bp as the will form a new rotation matrix and thus should be normal
//		float lengthA = sqrt(A[0]*A[0]+A[1]*A[1]+A[2]*A[2]);
//		A[0]/=lengthA;
//		A[1]/=lengthA;
//		A[2]/=lengthA;
//		float lengthN = sqrt(N[0]*N[0]+N[1]*N[1]+N[2]*N[2]);
//		N[0]/=lengthN;
//		N[1]/=lengthN;
//		N[2]/=lengthN;
//		float lengthBp= sqrt(Bp[0]*Bp[0]+Bp[1]*Bp[1]+Bp[2]*Bp[2]);
//		Bp[0]/=lengthBp;
//		Bp[1]/=lengthBp;
//		Bp[2]/=lengthBp;

//		// define and store rotation matrix
//		float M[16]={ A[0],Bp[0],N[0],0.0f,
//		              A[1],Bp[1],N[1],0.0f,
//		              A[2],Bp[2],N[2],0.0f,
//		              0.0f,0.0f, 0.0f,1.0f	};
//		for(int j=0;j<16;j++)
//		{
//			g_fpCameraMatrices[ i*16 + j ] = M[j];
//		}

//		// calculate screen coordinates
//		float Sa[2] = { A[0]*a[0] + A[1]*a[1] + A[2]*a[2],  Bp[0]*a[0] + Bp[1]*a[1] + Bp[2]*a[2]};
//		float Sb[2] = { A[0]*b[0] + A[1]*b[1] + A[2]*b[2],  Bp[0]*b[0] + Bp[1]*b[1] + Bp[2]*b[2]};
//		float Sc[2] = { A[0]*c[0] + A[1]*c[1] + A[2]*c[2],  Bp[0]*c[0] + Bp[1]*c[1] + Bp[2]*c[2]};
//		float Sd[2] = { A[0]*d[0] + A[1]*d[1] + A[2]*d[2],  Bp[0]*d[0] + Bp[1]*d[1] + Bp[2]*d[2]};

//		// calculate clip planes
//        float minX = std::min(Sa[0],std::min(Sb[0],std::min(Sc[0],Sd[0])));
//        float maxX = std::max(Sa[0],std::max(Sb[0],std::max(Sc[0],Sd[0])));
//        float minY = std::min(Sa[1],std::min(Sb[1],std::min(Sc[1],Sd[1])));
//        float maxY = std::max(Sa[1],std::max(Sb[1],std::max(Sc[1],Sd[1])));

//		// store clip planes
//		g_fpCameraClipPlanes[i*4 +0]= minX;
//		g_fpCameraClipPlanes[i*4 +1]= maxX;
//		g_fpCameraClipPlanes[i*4 +2]= minY;
//		g_fpCameraClipPlanes[i*4 +3]= maxY;
		
//		// calculate texture coordinates
//		g_fpTexCoords[ (i*4+0)*2 +0] = (Sa[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+0)*2 +1] = (Sa[1]-minY)/(maxY-minY);

//		g_fpTexCoords[ (i*4+1)*2 +0] = (Sb[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+1)*2 +1] = (Sb[1]-minY)/(maxY-minY);

//		g_fpTexCoords[ (i*4+2)*2 +0] = (Sc[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+2)*2 +1] = (Sc[1]-minY)/(maxY-minY);

//		g_fpTexCoords[ (i*4+3)*2 +0] = (Sd[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+3)*2 +1] = (Sd[1]-minY)/(maxY-minY);
		
//		// move the texture coordinates to the inside (prevent interpolating with the next texel)
//		float textureResolution= (float)g_iShadowTexSize / (float)g_iShadowTexRowCount;
//		float scale = 1.0f - ((g_fTexelOffset*2.0f) / textureResolution);	// scaling down the texCoords with one texel;
	
//		// get the texture coordinates of the quad
//		float *texA = &g_fpTexCoords[ (i*4+0)*2];
//		float *texB = &g_fpTexCoords[ (i*4+1)*2];
//		float *texC = &g_fpTexCoords[ (i*4+2)*2];
//		float *texD = &g_fpTexCoords[ (i*4+3)*2];

//		// calculate old center of the primitive
//		float oldCenter[3] = {(texA[0]+texB[0]+texC[0]+texD[0])/4.0f, (texA[1]+texB[1]+texC[1]+texD[1])/4.0f};

//		for(int j=0; j<8; j++)
//		{
//			texA[j] *= scale;
//		}
	
//		// calculate new center of the primitive
//		float newCenter[3] = {(texA[0]+texB[0]+texC[0]+texD[0])/4.0f, (texA[1]+texB[1]+texC[1]+texD[1])/4.0f};

//		// calculate the diference between the two centers
//		float delta[2] = {oldCenter[0] - newCenter[0], oldCenter[1] - newCenter[1]};

//		// translate the new coordinates back to the old center
//		for(int j=0; j<4; j++)
//		{
//			texA[j*2+0] += delta[0];
//			texA[j*2+1] += delta[1];
//		}
		
//		// scale and translate texture coordinates into the page texture
//		int index = (i)%(g_iShadowTexRowCount*g_iShadowTexRowCount);
//		int tempRow = index / g_iShadowTexRowCount;
//		int tempColumn = index % g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+0)*2 +0] = ( g_fpTexCoords[ (i*4+0)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+0)*2 +1] = ( g_fpTexCoords[ (i*4+0)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+1)*2 +0] = ( g_fpTexCoords[ (i*4+1)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+1)*2 +1] = ( g_fpTexCoords[ (i*4+1)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+2)*2 +0] = ( g_fpTexCoords[ (i*4+2)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+2)*2 +1] = ( g_fpTexCoords[ (i*4+2)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+3)*2 +0] = ( g_fpTexCoords[ (i*4+3)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+3)*2 +1] = ( g_fpTexCoords[ (i*4+3)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;
//	}
	

//	// create the vertex buffer objects
//    g_oMeshVBO.configure( GL_QUADS, GL_DYNAMIC_DRAW_ARB );
//    g_oMeshVBO.initVertices( g_fpVertices, g_iVertexCount );
//    g_oMeshVBO.initNormals( g_fpNormals );
//    g_oMeshVBO.initTexCoords( g_fpTexCoords );

//	g_bMeshLoaded=true;

//	printf(" done.\n");
//}

//void ShadowView::updateModel()
//{
//	// "land" mesh on the ground
//	g_oMesh->resizeMesh(1.0f);
//	g_oMesh->centerMesh();
//	g_oMesh->centerMesh(0.0f,g_oMesh->getSizeY()/2.0f,0.0f);
//	g_fGroundLevel = g_oMesh->getSizeY()/2.0f;

//	// calculate texture coordinates and camera matrices
//	for(int i=0; i<g_iVertexCount/4;i++)
//	{
//		// get the vertices
//		float *a = &g_fpVertices[(i*4+0)*3];
//		float *b = &g_fpVertices[(i*4+1)*3];
//		float *c = &g_fpVertices[(i*4+2)*3];
//		float *d = &g_fpVertices[(i*4+3)*3];

//		// calculate A = b-a and B = c-a
//		float A[3] = { b[0]-a[0], b[1]-a[1], b[2]-a[2] };
//		float B[3] = { c[0]-a[0], c[1]-a[1], c[2]-a[2] };

//		// calculate normal to the triangle
//		float N[3] = { A[1]*B[2]-A[2]*B[1], A[2]*B[0]-A[0]*B[2], A[0]*B[1]-A[1]*B[0] };

//		// calculate normal to the plane between the triangle normal and A
//		float Bp[3] = { N[1]*A[2]-N[2]*A[1], N[2]*A[0]-N[0]*A[2], N[0]*A[1]-N[1]*A[0] };

//		// normalize A, N and Bp as the will form a new rotation matrix and thus should be normal
//		float lengthA = sqrt(A[0]*A[0]+A[1]*A[1]+A[2]*A[2]);
//		A[0]/=lengthA;
//		A[1]/=lengthA;
//		A[2]/=lengthA;
//		float lengthN = sqrt(N[0]*N[0]+N[1]*N[1]+N[2]*N[2]);
//		N[0]/=lengthN;
//		N[1]/=lengthN;
//		N[2]/=lengthN;
//		float lengthBp= sqrt(Bp[0]*Bp[0]+Bp[1]*Bp[1]+Bp[2]*Bp[2]);
//		Bp[0]/=lengthBp;
//		Bp[1]/=lengthBp;
//		Bp[2]/=lengthBp;

//		// define and store rotation matrix
//		float M[16]={ A[0],Bp[0],N[0],0.0f,
//		              A[1],Bp[1],N[1],0.0f,
//		              A[2],Bp[2],N[2],0.0f,
//		              0.0f,0.0f, 0.0f,1.0f	};
//		for(int j=0;j<16;j++)
//		{
//			g_fpCameraMatrices[ i*16 + j ] = M[j];
//		}

//		// calculate screen coordinates
//		float Sa[2] = { A[0]*a[0] + A[1]*a[1] + A[2]*a[2],  Bp[0]*a[0] + Bp[1]*a[1] + Bp[2]*a[2]};
//		float Sb[2] = { A[0]*b[0] + A[1]*b[1] + A[2]*b[2],  Bp[0]*b[0] + Bp[1]*b[1] + Bp[2]*b[2]};
//		float Sc[2] = { A[0]*c[0] + A[1]*c[1] + A[2]*c[2],  Bp[0]*c[0] + Bp[1]*c[1] + Bp[2]*c[2]};
//		float Sd[2] = { A[0]*d[0] + A[1]*d[1] + A[2]*d[2],  Bp[0]*d[0] + Bp[1]*d[1] + Bp[2]*d[2]};

//		// calculate clip planes
//        float minX = std::min(Sa[0],std::min(Sb[0],std::min(Sc[0],Sd[0])));
//        float maxX = std::max(Sa[0],std::max(Sb[0],std::max(Sc[0],Sd[0])));
//        float minY = std::min(Sa[1],std::min(Sb[1],std::min(Sc[1],Sd[1])));
//        float maxY = std::max(Sa[1],std::max(Sb[1],std::max(Sc[1],Sd[1])));

//		// store clip planes
//		g_fpCameraClipPlanes[i*4 +0]= minX;
//		g_fpCameraClipPlanes[i*4 +1]= maxX;
//		g_fpCameraClipPlanes[i*4 +2]= minY;
//		g_fpCameraClipPlanes[i*4 +3]= maxY;
		
//		// calculate texture coordinates
//		g_fpTexCoords[ (i*4+0)*2 +0] = (Sa[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+0)*2 +1] = (Sa[1]-minY)/(maxY-minY);

//		g_fpTexCoords[ (i*4+1)*2 +0] = (Sb[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+1)*2 +1] = (Sb[1]-minY)/(maxY-minY);

//		g_fpTexCoords[ (i*4+2)*2 +0] = (Sc[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+2)*2 +1] = (Sc[1]-minY)/(maxY-minY);

//		g_fpTexCoords[ (i*4+3)*2 +0] = (Sd[0]-minX)/(maxX-minX);
//		g_fpTexCoords[ (i*4+3)*2 +1] = (Sd[1]-minY)/(maxY-minY);
		
//		// move the texture coordinates to the inside (prevent interpolating with the next texel)
//		float textureResolution= (float)g_iShadowTexSize / (float)g_iShadowTexRowCount;
//		float scale = 1.0f - ((g_fTexelOffset*2.0f) / textureResolution);	// scaling down the texCoords with one texel;
	
//		// get the texture coordinates of the quad
//		float *texA = &g_fpTexCoords[ (i*4+0)*2];
//		float *texB = &g_fpTexCoords[ (i*4+1)*2];
//		float *texC = &g_fpTexCoords[ (i*4+2)*2];
//		float *texD = &g_fpTexCoords[ (i*4+3)*2];

//		// calculate old center of the primitive
//		float oldCenter[3] = {(texA[0]+texB[0]+texC[0]+texD[0])/4.0f, (texA[1]+texB[1]+texC[1]+texD[1])/4.0f};

//		for(int j=0; j<8; j++)
//		{
//			texA[j] *= scale;
//		}
	
//		// calculate new center of the primitive
//		float newCenter[3] = {(texA[0]+texB[0]+texC[0]+texD[0])/4.0f, (texA[1]+texB[1]+texC[1]+texD[1])/4.0f};

//		// calculate the diference between the two centers
//		float delta[2] = {oldCenter[0] - newCenter[0], oldCenter[1] - newCenter[1]};

//		// translate the new coordinates back to the old center
//		for(int j=0; j<4; j++)
//		{
//			texA[j*2+0] += delta[0];
//			texA[j*2+1] += delta[1];
//		}
		
//		// scale and translate texture coordinates into the page texture
//		int index = (i)%(g_iShadowTexRowCount*g_iShadowTexRowCount);
//		int tempRow = index / g_iShadowTexRowCount;
//		int tempColumn = index % g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+0)*2 +0] = ( g_fpTexCoords[ (i*4+0)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+0)*2 +1] = ( g_fpTexCoords[ (i*4+0)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+1)*2 +0] = ( g_fpTexCoords[ (i*4+1)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+1)*2 +1] = ( g_fpTexCoords[ (i*4+1)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+2)*2 +0] = ( g_fpTexCoords[ (i*4+2)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+2)*2 +1] = ( g_fpTexCoords[ (i*4+2)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;

//		g_fpTexCoords[ (i*4+3)*2 +0] = ( g_fpTexCoords[ (i*4+3)*2 +0] + (float)tempRow )/(float)g_iShadowTexRowCount;
//		g_fpTexCoords[ (i*4+3)*2 +1] = ( g_fpTexCoords[ (i*4+3)*2 +1] + (float)tempColumn )/(float)g_iShadowTexRowCount;
//	}
	

//	// update the vertex buffer objects
//	g_oMeshVBO.updateVertices(g_fpVertices);
//	g_oMeshVBO.updateNormals(g_fpNormals);
//	g_oMeshVBO.updateTexCoords(g_fpTexCoords);
//}


//void ShadowView::initFBOs()
//{
//	printf( "Initializing Frame Buffer Objects...");

//	// generate color buffer textures
//	g_iShadowTextureCount = g_iCameraMatrixCount / (g_iShadowTexRowCount*g_iShadowTexRowCount);
//	if( (g_iCameraMatrixCount % (g_iShadowTexRowCount*g_iShadowTexRowCount)) != 0) g_iShadowTextureCount++;
//	printf("g_iShadowTextureCount=%i\n",g_iShadowTextureCount);
//    g_ipShadowTextures.clear();
//    g_ipShadowTextures.resize(g_iShadowTextureCount);
//	for(int i=0;i<g_iShadowTextureCount;i++)
//	{
//        g_ipShadowTextures[i].set_format( GL_RGB );
//        g_ipShadowTextures[i].set_data( g_iShadowTexSize, g_iShadowTexSize, 0 );
//	}

//    // ground tex
//    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&g_iGroundShadowTexSize);
//    g_oGroundTexFBO.clear();
//    g_oGroundTex.set_format( GL_RGB );
//    g_oGroundTex.set_data( g_iGroundShadowTexSize, g_iGroundShadowTexSize, 0 );
//    g_oGroundTexFBO.attach_color_texture( g_oGroundTex );
	
//	printf( "done\n");

//}

//// set's the initial geographical so that when loading the model, there are some shadows
//void ShadowView::initSunPosition()
//{
//	// calculate sun position
//	printf( "Initializing Sun Position...");
//	sunPosition.setPosition(g_ipLatitude,g_ipLongitude,g_fTimeZone);
//	sunPosition.setDate(g_sCurrentDate);
//	sunPosition.calculate();
	
//	// update local azimuth and elevation
//	g_fLightAzimuth = (float)sunPosition.azimuthOut;
//	g_fLightElevation = (float)sunPosition.elevationOut;
//	printf( "done\n");
//}

//// OBSOLETE because all the lighting calculations are done by shaders
//// set's up the lighting
//void ShadowView::initLighting()
//{
//	printf( "Initializing Light...");
	
//	g_pfLightPos = new float[4];
//	for(int i=0;i<4;i++)
//	{
//		g_pfLightPos[i]=1.0f;
//	}
//	g_fLightElevation=45.0f;
//	g_fLightAzimuth=45.0f;
//	g_fLightTransZ=5.0f;
//	//float g_pfLightDiffuse[4] = {0.99f,0.99f,1.0f,1.0f};
//	//float g_pfLigthAmbient[4] = {0.0f,0.0f,0.0f,1.0f};
//	//glEnable(GL_COLOR_MATERIAL);
//	//glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//	//glMaterialf( GL_FRONT, GL_SHININESS, 50.0);
//	//glMaterialfv( GL_FRONT, GL_SPECULAR, g_pfLightSpecular);
//	//glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
//	//glLightfv( GL_LIGHT0, GL_POSITION, g_pfLightPos);
//	//glLightfv( GL_LIGHT0, GL_AMBIENT, g_pfLigthAmbient);
//	//glLightfv( GL_LIGHT0, GL_DIFFUSE, g_pfLightDiffuse);
//	//glLightfv( GL_LIGHT0, GL_SPECULAR, g_pfLightSpecular);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
//	//renderLights();
//	updateLightPosition();

//	printf( "done\n");
//}

//// OBSOLETE, shoule be replaced by the lightscattering sky dome
//void ShadowView::initTextures()
//{
//	printf( "Initializing Textures...");
//    g_iSunTex = genExpSprite<float>(32,1.0);
//	printf( "done\n");
//}

//// init the shadow map FBO with the maximum texture size
//void ShadowView::initShadows()
//{
//	printf( "Initializing Shadows...");
	
//	// get the max texture size
//	//glGetIntegerv(GL_MAX_TEXTURE_SIZE,&g_iShadowMapSize);
	
//	// initialize Schadow FBO
//	//g_oShadowFBO.init(g_iShadowMapSize,FBO_DEPTH_TEX | FBO_COLOR_TEX, GL_RGB16F_ARB   ,GL_RGB,GL_FLOAT);
//	//g_oShadowFBO.init(g_iShadowMapSize,FBO_DEPTH_TEX | FBO_COLOR_TEX, GL_RGB ,GL_RGB, GL_FLOAT);
	
//	//Initializing Shadows
//	//updateMeshShadowTexture();

//	// init shadow mapping matrices
//	g_pfBiasMatrix = new float[16];
//	g_pfLightProjection = new float[16];
//	g_pfLightModelView = new float[16];
//	for(int i=0;i<16;i++)
//	{
//		g_pfBiasMatrix[i]=0.0f;
//		g_pfLightProjection[i]=0.0f;
//		g_pfLightModelView[i]=0.0f;
//	}
//	g_pfBiasMatrix[0] = 0.5f;
//	g_pfBiasMatrix[5] = 0.5f;
//	g_pfBiasMatrix[10] = 0.5f;
//	g_pfBiasMatrix[12] = 0.5f;
//	g_pfBiasMatrix[13] = 0.5f;
//	g_pfBiasMatrix[14] = 0.5f;
//	g_pfBiasMatrix[15] = 1.0f;

//	// initialize shadow map
//    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&g_iShadowMapSize);
//    g_oShadowTex.set_format( GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT );
//    g_oShadowTex.set_data( g_iShadowMapSize, g_iShadowMapSize, 0 );
//    g_oShadowFBO.attach_depth_texture( g_oShadowTex );
	
//	printf( "done\n");
//}


//void ShadowView::initShaders()
//{
//	// init Depth Texture
//    depthTexture.load_vertex_shader( "shaders/depthTexture.vert" );
//    depthTexture.load_fragment_shader( "shaders/depthTexture.frag" );

//	// init Shadow Mapping
//    shadowMapping.load_vertex_shader( "shaders/shadowMapping.vert" );
//    shadowMapping.load_fragment_shader( "shaders/shadowMapping.frag" );
	
//	// init Shadow Mapping
//    sceneRendering.load_vertex_shader( "shaders/sceneRendering.vert" );
//    sceneRendering.load_fragment_shader( "shaders/sceneRendering.frag" );
//}


//void ShadowView::initExtensions() {

//	// initialize glew
//	GLenum error = glewInit();
//	if (GLEW_OK != error)
//	{
//	  /* Problem: glewInit failed, something is seriously wrong. */
//	  fprintf(stderr, "ERROR: %s\n", glewGetErrorString(error));
//	}

//    // check mandatory for extensions
//	char missingExtensions[500]="";
//	if (!GLEW_ARB_multitexture)				strcat(missingExtensions,"\nGL_ARB_multitexture");
//	//if (!GLEW_ARB_texture_cube_map)			strcat(missingExtensions,"\nGL_ARB_texture_cube_map");
//	if (!GLEW_ARB_vertex_buffer_object)		strcat(missingExtensions,"\nGL_ARB_vertex_buffer_object");
//	if (!GL_EXT_framebuffer_object)			strcat(missingExtensions,"\nGL_EXT_framebuffer_object");
//	if (!GL_ARB_shader_objects)				strcat(missingExtensions,"\nGL_ARB_shader_objects");
//	if (!GLEW_ARB_fragment_shader)			strcat(missingExtensions,"\nGLEW_ARB_fragment_shader");
//	if (!GLEW_ARB_vertex_shader)			strcat(missingExtensions,"\nGLEW_ARB_vertex_shader");
//	if (!GLEW_VERSION_1_5)					strcat(missingExtensions,"\nOpenGL Version 1.5");
	
//	if (strlen(missingExtensions)>0) {
//		printf("ERROR: Some needed extensions are not present:%s\n",missingExtensions);
//		char dummy;		scanf( "%c",&dummy);		exit(-1);
//	}
//}


//bool ShadowView::fileExists(char* filename) {
//	FILE *stream; stream = fopen(filename, "r");
//	if (stream != NULL) {
//		fclose( stream );
//		return true;
//	} else return false;
//}

//void ShadowView::paintGL()
//{
//	//updateShadowMap();
//    //updateMeshShadowTexture();
//    setupUserViewProj();

//	// enable Texturing
//	glEnable(GL_TEXTURE_2D);

//	// Clear Buffer for new frame
//	glClearColor(0.0f ,0.0f, 0.0f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//	// render Background
//	renderBackground();
	
//	// render the sun
//	renderSun();
	
//	// Modelview Transformation
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glTranslatef( 0.0f, 0.0f, g_fCameraZoom); // Zoom
//	glRotatef(g_fCameraElevation, 1.0f, 0.0f, 0.0f);  // rotate elevation
//	glRotatef(g_fCameraAzimuth, 0.0f, 1.0f, 0.0f);    // rotate azimuth
//	glTranslatef( 0.0f, -g_fGroundLevel, 0.0f);   // Lower the whole scene so that the center of the Mesh overlappes with the center of rotation

//	if(g_bMeshLoaded)
//	{
//		// enable sceneRendering Shader
//		sceneRendering.enable();
		
//		// setup shader parameters
//		// TODO not elegant! try to do with uniform variabiles
//		glColor4f(g_fSceneRenderingMode/4.0f, 0.0f, 0.0f, 0.0f);

//		// render the scene
//		renderMeshTex();

//		sceneRendering.disable();
//	}
//}

//void ShadowView::resizeGL(int width,int height) {
//        // update viewport dimensions
//        g_iWindowWidth     =width;
//        g_iWindowHeight    =height;
//        g_fAspect       =(float)width/(float)height;
//}


//QSize ShadowView::minimumSizeHint() const
//{
//        return QSize(50, 50);
//}


//QSize ShadowView::sizeHint() const
//{
//        return QSize(g_iWindowWidth, g_iWindowHeight);
//}


void ShadowView::mousePressEvent(QMouseEvent *event)
{
    g_iMousePosX = event->x();
    g_iMousePosY = event->y();
}

void ShadowView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        // new Mouse Movement
        float dx = (float) -(g_iMousePosX - event->x()) / 10.0f;
        float dy = (float) -(g_iMousePosY - event->y()) / 10.0f;

        // Update Azimuth & Elevation
        g_fCameraAzimuth   += dx;
        g_fCameraElevation += dy;

        // predevent big values
        g_fCameraAzimuth   = fmod(g_fCameraAzimuth,360.0f);
        g_fCameraElevation = fmod(g_fCameraElevation,360.0f);
    }
    else if(event->buttons() & Qt::RightButton)
    {
        // calculate relative zoom
        float dz = (float)(g_iMousePosY - event->y()) / 50.0f;

        // make zoom logarithmic
        dz *= log(fabs((double)g_fCameraZoom/10.0)+1.0);

        // update camera zoom
        g_fCameraZoom += dz;
    }

    g_iMousePosX = event->x();
    g_iMousePosY = event->y();

    updateGL();
}


//void ShadowView::keyPressEvent( QKeyEvent * event)
//{
//	switch( event->key() )
//	{
//		//case Qt::Key_0 : g_fSceneRenderingMode = 0.0f; break;
//	}
//}

//// update the shadow map
//void ShadowView::updateShadowMap()
//{
//	// setup and copy the modelview matrix
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	glTranslatef( 0.0f , 0.0f , -2.0f );	// Zoom
//	glRotatef(g_fLightElevation, 1.0f, 0.0f, 0.0f );// rotate elevation
//	glRotatef(g_fLightAzimuth - 90.0f, 0.0f, 1.0f, 0.0f);	// rotate azimuth
//	glTranslatef( 0.0f, -g_fGroundLevel, 0.0f); // compensate for groundlevel
//	glGetFloatv(GL_MODELVIEW_MATRIX, g_pfLightModelView);
//	glPopMatrix();

//	// setup and copy the projection matrix
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho( -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 5.0f);
//	glGetFloatv(GL_PROJECTION_MATRIX, g_pfLightProjection);
//	glPopMatrix();

//	// load modelview Matrix
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//	glLoadMatrixf(g_pfLightModelView);

//	// load projetion Matrix
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glLoadMatrixf(g_pfLightProjection);

//	// setup viewPort
//	glViewport(0,0,g_iShadowMapSize,g_iShadowMapSize);

//	// setup culling
//	glCullFace(GL_BACK);

//	// render the Model
//	g_oShadowFBO.enable();
//	depthTexture.disable();
//	glClearColor(1,1,1,1);
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	g_oMeshVBO.draw();
//	depthTexture.disable();
//	g_oShadowFBO.disable();
//}


//// This methode renders each polygon with shadows into a texture
//void ShadowView::updateMeshShadowTexture()
//{
//	//// DEBUG
//	//printf("\n// window properties\n");
//	//printf("g_iWindowWidth= %i\n",g_iWindowWidth);
//	//printf("g_iWindowHeight= %i\n",g_iWindowHeight);
//	//printf("g_fAspect= %f\n",g_fAspect);
//	//printf("\n");
//	//printf("\n// camera properties\n");
//	//printf("g_fCameraAzimuth= %f\n",g_fCameraAzimuth);
//	//printf("g_fCameraElevation= %f\n",g_fCameraElevation);
//	//printf("g_fCameraZoom= %f\n",g_fCameraZoom);
//	//printf("\n");
//	//printf("\n// window properties\n");
//	//printf("g_iMousePosX= %i\n",g_iMousePosX);
//	//printf("g_iMousePosY= %i\n",g_iMousePosY);
//	//printf("\n");
//	//printf("\n// Light Controls\n");
//	//printf("g_pfLightPos= %f\t%f\t%f\t%f\n",g_pfLightPos[0],g_pfLightPos[1],g_pfLightPos[2],g_pfLightPos[3]);
//	//printf("g_fLightTransZ= %f\n",g_fLightTransZ);
//	//printf("\n");
//	//printf("\n// Light Controls\n");
//	//printf("g_sCurrentDate= %i\t%i\t%i\t%i\t%i\t%i\n",g_sCurrentDate.year,g_sCurrentDate.month,g_sCurrentDate.day,g_sCurrentDate.hour,g_sCurrentDate.minute,g_sCurrentDate.second);
//	//printf("g_ipLatitude= %i\t%i\t%i\n",g_ipLatitude[0],g_ipLatitude[1],g_ipLatitude[2]);
//	//printf("g_ipLongitude= %i\t%i\t%i\n",g_ipLongitude[0],g_ipLongitude[1],g_ipLongitude[2]);
//	//printf("g_fTimeZone= %i\n",g_fTimeZone);
//	//printf("\n");
//	//printf("\n// Light Controls\n");
//	//printf("g_pfBiasMatrix= \n");
//	//for(int i=0;i<16;i++)
//	//{
//	//	printf("\t%f",g_pfBiasMatrix[i]);
//	//	if((i%4)==3)	printf("\n");
//	//}
//	//printf("g_pfLightProjection= \n");
//	//for(int i=0;i<16;i++)
//	//{
//	//	printf("\t%f",g_pfLightProjection[i]);
//	//	if((i%4)==3)	printf("\n");
//	//}
//	//printf("g_pfLightModelView= \n");
//	//for(int i=0;i<16;i++)
//	//{
//	//	printf("\t%f",g_pfLightModelView[i]);
//	//	if((i%4)==3)	printf("\n");
//	//}
//	//printf("\n");
//	//printf("\n// Shadow Texture FBO's\n");
//	//printf("g_iShadowTexSize= %i\n",g_iShadowTexSize);
//	//printf("g_iShadowTexRowCount= %i\n",g_iShadowTexRowCount);
//	//printf("g_iGroundShadowTexSize= %i\n",g_iGroundShadowTexSize);
//	//printf("g_fTexelOffset= %f\n",g_fTexelOffset);
//	//printf("\n");
//	//printf("\n// shadowing period\n");
//	//printf("g_sStartDate= %i\t%i\t%i\t%i\t%i\t%i\n",g_sStartDate.year,g_sStartDate.month,g_sStartDate.day,g_sStartDate.hour,g_sStartDate.minute,g_sStartDate.second);
//	//printf("g_sStopDate= %i\t%i\t%i\t%i\t%i\t%i\n",g_sStopDate.year,g_sStopDate.month,g_sStopDate.day,g_sStopDate.hour,g_sStopDate.minute,g_sStopDate.second);
//	//printf("g_iSteps= %i\n",g_iSteps);
//	//printf("\n");
//	//printf("\n// Model\n");
//	//printf("g_bMeshLoaded= %i\n",g_bMeshLoaded);
//	//printf("\n");


//	// Setup the mask
//	// clear the FBO's to (0,0,1,0)
//	for(int i=0;i<g_iShadowTextureCount;i++)
//	{
//        g_oMeshTexFBO.attach_color_texture( g_ipShadowTextures[i] );
//        g_oMeshTexFBO.enable();

//		glViewport(0,0,g_iShadowTexSize,g_iShadowTexSize);
//		glClearColor(0.0f,0.0f,1.0f,0.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//		g_oMeshTexFBO.disable();
//	}

//	// draw black geometry
//	for(int i=0;i<g_iVertexCount/4;i++)
//	{
//		int viewPortSize = g_iShadowTexSize / g_iShadowTexRowCount;

//		int indexTex = i / (g_iShadowTexRowCount*g_iShadowTexRowCount);
//		int index = i % (g_iShadowTexRowCount*g_iShadowTexRowCount);

//		int tempRow = index % g_iShadowTexRowCount;
//		int tempColumn = index / g_iShadowTexRowCount;

//		int offsetX = tempColumn * (g_iShadowTexSize / g_iShadowTexRowCount);
//		int offsetY = tempRow * (g_iShadowTexSize / g_iShadowTexRowCount);

//		glViewport(offsetX,offsetY,viewPortSize,viewPortSize);	// and set new dimensions
//		glMatrixMode(GL_PROJECTION);			// reset PROJECTION matrix
//		glLoadIdentity();
//		glOrtho(g_fpCameraClipPlanes[i*4+0],g_fpCameraClipPlanes[i*4+1],g_fpCameraClipPlanes[i*4+2],g_fpCameraClipPlanes[i*4+3],-10,+10);

//		// Set up camera (just over the primitive)
//		glMatrixMode(GL_MODELVIEW);
//		glLoadMatrixf(&(g_fpCameraMatrices[i*16]));
		
//		// render the primitive
//        g_oMeshTexFBO.attach_color_texture( g_ipShadowTextures[indexTex] );
//        g_oMeshTexFBO.enable();

//		glColor4f(0.0f,0.0f,0.0f,0.0f);
	
//        g_oMeshVBO.draw_elements(i,1);
			
//		g_oMeshTexFBO.disable();

//	}

//	// clear the ground texture
//	g_oGroundTexFBO.enable();
//	glViewport(0,0,g_iGroundShadowTexSize,g_iGroundShadowTexSize);
//	glClearColor(0.0f,0.0f,0.0f,0.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	g_oGroundTexFBO.disable();

//	// render model textures
//	for(int j=0; j<g_iSteps; j++)
//	{
//		// update sun position
//		if(j==0)	initShadowingPeriod();
//		else	updateCurrentDate();
//		updateSunPosition();
//		updateLightPosition();
		
//		// update schadow map
//		updateShadowMap();

//		// load shader
//		shadowMapping.enable();

//		// Bind textures
//        glActiveTexture(GL_TEXTURE0);
//        g_oShadowTex.bind();
//		glEnable(GL_TEXTURE_2D);

//		// enable blending
//        glEnable(GL_BLEND);
//		glBlendFunc(GL_ONE,GL_ONE);

//		int viewPortSize = g_iShadowTexSize / g_iShadowTexRowCount;
		
//		for(int i=0;i<g_iVertexCount/4;i++)
//		{
//			int indexTex = i / (g_iShadowTexRowCount*g_iShadowTexRowCount);
//			int index = i % (g_iShadowTexRowCount*g_iShadowTexRowCount);
	
//			int tempRow = index % g_iShadowTexRowCount;
//			int tempColumn = index / g_iShadowTexRowCount;
	
//			int offsetX = tempColumn * (g_iShadowTexSize / g_iShadowTexRowCount);
//			int offsetY = tempRow * (g_iShadowTexSize / g_iShadowTexRowCount);
	
//			// calculate the cosinus of the angle between the light vector and the surface normal
//			// do not render the face if the the angle is greater than 90 (i.e. cosinus of the angle is negative)
//			float cosAngle = dot(g_pfLightPos, &g_fpFaceNormals[i*3]);
//			if( g_bSmoothShading || ((cosAngle >= 0.0f) && (g_fLightElevation > 0.0f)) )
//			{
//				glViewport(offsetX,offsetY,viewPortSize,viewPortSize);	// and set new dimensions
	
//				// Set up camera (just over the primitive)
//				glMatrixMode(GL_MODELVIEW);
//				glLoadMatrixf(&(g_fpCameraMatrices[i*16]));
//				glMatrixMode(GL_PROJECTION);
//				glLoadIdentity();
//				glOrtho(g_fpCameraClipPlanes[i*4+0],g_fpCameraClipPlanes[i*4+1],g_fpCameraClipPlanes[i*4+2],g_fpCameraClipPlanes[i*4+3],-10,+10);
	
//				// setup TextureMatrix
//				glMatrixMode(GL_TEXTURE);
//				glLoadIdentity();
//				glMultMatrixf(g_pfBiasMatrix);
//				glMultMatrixf(g_pfLightProjection);
//				glMultMatrixf(g_pfLightModelView);
//				glMatrixMode(GL_MODELVIEW);
	
//				// render the primitive
//                g_oMeshTexFBO.attach_color_texture( g_ipShadowTextures[indexTex] );
//                g_oMeshTexFBO.enable();
//                glColor4f(cosAngle, (float)((int)g_bSmoothShading), 0.0f, 1.0f/(float)g_iSteps);
//                g_oMeshVBO.draw_vertices(i,4);
//				g_oMeshTexFBO.disable();
//			}
//		}
		
//		// render the ground shadow
//		float grpundNormal[3] = {0.0f, 1.0f, 0.0f};
//		float cosAngle = dot(g_pfLightPos, grpundNormal);
//		if( g_bSmoothShading || ((cosAngle >= 0.0f) && (g_fLightElevation > 0.0f)) )
//		{
//			glViewport(0,0,g_iGroundShadowTexSize,g_iGroundShadowTexSize);

//			// Set up camera (just over the primitive)
//			glMatrixMode(GL_MODELVIEW);
//			glLoadIdentity();
//			glRotatef(90.0f,1.0f,0.0f,0.0f);
//			glRotatef(180.0f,0.0f,1.0f,0.0f);
//			glMatrixMode(GL_PROJECTION);
//			glLoadIdentity();
//			glOrtho(-2.0f,2.0f,-2.0f,2.0f,-2.0f,2.0f);

//			// setup TextureMatrix
//			glMatrixMode(GL_TEXTURE);
//			glLoadIdentity();
//			glMultMatrixf(g_pfBiasMatrix);
//			glMultMatrixf(g_pfLightProjection);
//			glMultMatrixf(g_pfLightModelView);
//			glMatrixMode(GL_MODELVIEW);
	
//			// render the ground
//			g_oGroundTexFBO.enable();
//				glColor4f(cosAngle, (float)((int)g_bSmoothShading), 0.0f, 1.0f/(float)g_iSteps);
//				renderGround();
//			g_oGroundTexFBO.disable();
//		}

//		// disable stuff
//		glDisable(GL_BLEND);
//		glBindTexture(GL_TEXTURE_2D,0);
//		glDisable(GL_TEXTURE_2D);
//		shadowMapping.disable();
//	}
//}


void ShadowView::renderBackground()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);

    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, 1.0f);
        glColor3f( 0.75, 0.75, 1);
        glVertex3f(-2.0f,-1.5f,-2.0f);
        glVertex3f(+2.0f,-1.5f,-2.0f);
        glColor3f( 0, 0, 0);
        glVertex3f(+2.0f,+1.5f,-2.0f);
        glVertex3f(-2.0f,+1.5f,-2.0f);
    glEnd();

    glDepthMask(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

//void ShadowView::renderSun()
//{
//    // rotate the Light Source
//    //glMatrixMode(GL_MODELVIEW);
//    //glPushMatrix();
//    //glRotatef(-g_fLightAzimuth,0,1,0);	// rotate azimuth
//    //glRotatef(g_fLightElevation,-1,0,0);	// rotate elevation
//    //glTranslatef(0,0,g_fLightTransZ*1.1);

//    // Modelview Transformation
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glLoadIdentity();
//    //glTranslatef( 0, 0, g_fCameraZoom); // Zoom
//    glRotatef(g_fCameraElevation,1,0,0);  // rotate elevation
//    glRotatef(g_fCameraAzimuth,0,1,0);    // rotate azimuth
//    //glTranslatef( 0, -g_fGroundLevel, 0);

//    //Calculate Current Light Position (for Shadows)

//    // render a sprite for the Light
//    glPointSize(67.0f);
//    glColor3f( 1.0, 1.0, 1.0);
//    g_iSunTex.bind();

//    float quadratic[3] = {1.0f,0.1f,0.01f};
//    glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB,quadratic);
//    glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );

//    glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
//    glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
//    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, 67.0f );

//    glDisable(GL_LIGHTING);
//    glEnable(GL_BLEND);
//    glEnable( GL_POINT_SPRITE_ARB );
//    glDepthMask(false);

//    initShadowingPeriod();

//    for(int i=0;i<g_iSteps;i++)
//    {
//        if(i!=0)	updateCurrentDate();
//        updateSunPosition();
//        updateLightPosition();

//        // draw the sprite
//        glBegin(GL_POINTS);
//            glVertex3f(g_pfLightPos[0]*g_fLightTransZ,g_pfLightPos[1]*g_fLightTransZ,g_pfLightPos[2]*g_fLightTransZ);
//        glEnd();
//    }


//    // restore stuff
//    glMatrixMode(GL_MODELVIEW);
//    glPopMatrix();
//    glBindTexture(GL_TEXTURE_2D,0);
//    glDisable( GL_POINT_SPRITE_ARB );
//    glDisable(GL_BLEND);
//    glDepthMask(true);
//}

void ShadowView::renderGround() {

    //glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glNormal3f( 0.0f, 1.0f, 0.0f); // needs only to be set if it changes.
        glTexCoord2f( 1.0f, 0.0f);
        glVertex3f(-2.0f,0.0f,-2.0f);
        glTexCoord2f( 1.0f, 1.0f);
        glVertex3f(-2.0f,0.0f,+2.0f);
        glTexCoord2f( 0.0f, 1.0f);
        glVertex3f(+2.0f,0.0f,+2.0f);
        glTexCoord2f( 0.0f, 0.0f);
        glVertex3f(+2.0f,0.0f,-2.0f);
    glEnd();
}

//void ShadowView::renderMesh() {

//    //glColor3f(1.0f,1.0f,1.0f);
//    g_oMeshVBO.draw();

//    // visualize the cameraPositions
////	glLineWidth(15.0);
////	glDisable(GL_LIGHTING);
////	glColor3f(0,1,0);
////	cameras.draw();

//}

//void ShadowView::renderMeshTex()
//{
//    // render the model
//    int rest = g_iVertexCount / 4;
//    int length=0;

//    for(int i=0;i<g_iShadowTextureCount;i++)
//    {
//        // determine how many vertices to render
//        if(rest < (g_iShadowTexRowCount*g_iShadowTexRowCount) )	length = rest;
//        else length = g_iShadowTexRowCount*g_iShadowTexRowCount;

//        // render the mesh
//        g_ipShadowTextures[i].bind();
//        g_oMeshVBO.draw_vertices(i*4*g_iShadowTexRowCount*g_iShadowTexRowCount,4*length) ;
//        rest -= g_iShadowTexRowCount*g_iShadowTexRowCount;
//    }

//    // render Ground
//    g_oGroundTexFBO.enable();
//    renderGround();
//    g_oGroundTexFBO.disable();
//}



//void ShadowView::renderLights()
//{
//    glLightfv( GL_LIGHT0, GL_POSITION, g_pfLightPos);
//}


//void ShadowView::initShadowingPeriod()
//{

//    g_sCurrentDate.year   = g_sStartDate.year;
//    g_sCurrentDate.month  = g_sStartDate.month;
//    g_sCurrentDate.day    = g_sStartDate.day;
//    g_sCurrentDate.hour   = g_sStartDate.hour;
//    g_sCurrentDate.minute = g_sStartDate.minute;
//    g_sCurrentDate.second = g_sStartDate.second;

//    g_dStartDate = sunPosition.dateToJulian(g_sCurrentDate);
//    g_dCurrentDate = g_dStartDate;
//    g_dStopDate = sunPosition.dateToJulian(g_sStopDate);

//    g_dShadowingPeriod = g_dStopDate - g_dStartDate;

//    g_dTimePerFrame = g_dShadowingPeriod / (double)g_iSteps;
//}

//// update the current time according to the elapsed time
//void ShadowView::updateCurrentDate()
//{
//    if(g_dShadowingPeriod <= 0  )
//    {
//        return;
//        //if(!g_bToggleLoopAnimation)	return;
//        //
//        //// reinit g_dDeltaMinutes
//        //g_dDeltaMinutes = g_dStopDate - g_dStartDate;
//    }
//    else
//    {
//        g_dShadowingPeriod -= g_dTimePerFrame;
//        g_dCurrentDate += g_dTimePerFrame;
//        g_sCurrentDate = sunPosition.julianToDate(g_dCurrentDate);
//    }

//    //printf("%i %i %i %i %i %i %s %f\n",g_ipCurrentDate[0],g_ipCurrentDate[1],g_ipCurrentDate[2],g_ipCurrentDate[3],g_ipCurrentDate[4],g_ipCurrentDate[5],g_cpWeekdays[(int)fmod(g_dCurrentDate,7.0)],g_dCurrentDate);
//}

//// calculate new sun position based on the current date
//void ShadowView::updateSunPosition()
//{
//    // update sunPosition
//    sunPosition.setDate(g_sCurrentDate);
//    sunPosition.calculate();

//    // update local azimuth and elevation
//    g_fLightAzimuth = (float)sunPosition.azimuthOut;
//    g_fLightElevation = (float)sunPosition.elevationOut;

//    updateLightPosition();
//}

//// set the Light position Array, by projecting the azimuth and elevation angles
//void ShadowView::updateLightPosition()
//{
//    // x component
//    g_pfLightPos[0]	= cos(degToRad(g_fLightAzimuth)) * cos(degToRad(g_fLightElevation));

//    // y component
//    g_pfLightPos[1] = sin(degToRad(g_fLightElevation));

//    // z component
//    g_pfLightPos[2] = sin(degToRad(g_fLightAzimuth)) * cos(degToRad(g_fLightElevation));

//    // normalize Light vector
//    float vLength = sqrt(g_pfLightPos[0]*g_pfLightPos[0] + g_pfLightPos[1]*g_pfLightPos[1] + g_pfLightPos[2]*g_pfLightPos[2]);
//    g_pfLightPos[0] /= vLength;
//    g_pfLightPos[1] /= vLength;
//    g_pfLightPos[2] /= vLength;

//    // set the light position in the openGL state
//    float *lightVector = new float[4];
//    lightVector[0] = g_pfLightPos[0];
//    lightVector[1] = g_pfLightPos[2];
//    lightVector[2] = g_pfLightPos[1];
//    lightVector[3] = 1.0f;
//    glLightfv( GL_LIGHT0, GL_POSITION, lightVector);
//}


void ShadowView::setupUserViewProj() {
    glViewport(0,0,g_iWindowWidth,g_iWindowHeight);	// and set new dimensions
    glMatrixMode(GL_PROJECTION);			// reset PROJECTION matrix
    glLoadIdentity();
    gluPerspective(60.0,g_fAspect,0.002,20.0);
    glMatrixMode(GL_MODELVIEW);
}


//void ShadowView::setupCameraTexOrtho() {
//    glViewport(0,0,g_iShadowTexSize,g_iShadowTexSize);	// and set new dimensions
//    glMatrixMode(GL_PROJECTION);			// reset PROJECTION matrix
//    glLoadIdentity();
//}

//Date ShadowView::getStartDate()
//{
//        return g_sStartDate;
//}
//void ShadowView::setStartDate(int y, int m, int d, int h, int min, int s)
//{
//        g_sStartDate.year = y;
//        g_sStartDate.month = m;
//        g_sStartDate.day = d;
//        g_sStartDate.hour = h;
//        g_sStartDate.minute = min;
//        g_sStartDate.second = s;
//}

//Date ShadowView::getEndDate()
//{
//        return g_sStopDate;
//}
//void ShadowView::setEndDate(int y, int m, int d, int h, int min, int s)
//{
//        g_sStopDate.year = y;
//        g_sStopDate.month = m;
//        g_sStopDate.day = d;
//        g_sStopDate.hour = h;
//        g_sStopDate.minute = min;
//        g_sStopDate.second = s;
//}

//int *ShadowView::getLongitude()
//{
//        return g_ipLongitude;
//}
//void ShadowView::setLongitude(int deg, int min, int sec)
//{
//        g_ipLongitude[0]=deg;
//        g_ipLongitude[1]=min;
//        g_ipLongitude[2]=sec;
//}

//int *ShadowView::getLatitude()
//{
//        return g_ipLatitude;
//}
//void ShadowView::setLatitude(int deg, int min, int sec)
//{
//        g_ipLatitude[0]=deg;
//        g_ipLatitude[1]=min;
//        g_ipLatitude[2]=sec;
//}

//int ShadowView::getTimeZone()
//{
//        return g_fTimeZone;
//}
//void ShadowView::setTimeZone(int inputTimeZone)
//{
//        g_fTimeZone = inputTimeZone;
//}

//int ShadowView::getSlidesCount()
//{
//        return g_iSteps;
//}
//void ShadowView::setSlidesCount(int slides)
//{
//        g_iSteps = slides;
//}

//bool ShadowView::getSmoothShading()
//{
//    return g_bSmoothShading;
//}


//void ShadowView::setSmoothShading(bool input)
//{
//    g_bSmoothShading = input;
//}


//Mesh *ShadowView::getModel()
//{
//    return g_oMesh;
//}


//void ShadowView::setSceneRenderingMode(float input)
//{
//    g_fSceneRenderingMode = input;
//}


