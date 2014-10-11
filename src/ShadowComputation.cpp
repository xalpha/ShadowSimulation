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

#include "MeshDXF.hpp"
#include "MeshOBJ.hpp"

#include "ShadowComputation.hpp"
//#include <GL/glu.h>


ShadowComputation::ShadowComputation() :
    g_fpCameraMatrices(0),
    g_fpCameraClipPlanes(0),
    g_iCameraMatrixCount(0),
    g_iCameraClipPlaneCount(0),
    g_oMesh(0)
{
	// FBO's
    g_iFaceTexSize = 16;
	g_iShadowTextureCount = 0;
	g_fTexelOffset = 0.5f;

    // shadowing period
    g_sStartDate.init(2000,1,1,12,0,0);
    g_sStopDate.init(2000,1,1,13,0,0);
    g_fTimeZone=1;

    // calculate sun position
    g_sCurrentDate.init(2005,9,19,14,15,0);
    g_ipLatitude[0] = 48;
    g_ipLatitude[1] = 9;
    g_ipLatitude[2] = 0;
    g_ipLongitude[0] = 11;
    g_ipLongitude[1] = 43;
    g_ipLongitude[2] = 0;
    g_fTimeZone=1;
	
	// shadowing period
    g_iSteps=1;
	g_dStartDate=0;
	g_dTimePerFrame=0;
}


void ShadowComputation::loadModel( const std::string& filename )
{
    // init OpenGL stuf
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &g_iShadowTexSize );
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &g_iGroundShadowTexSize );

    // find out that type this is
    std::string extension = filename.substr( filename.find_last_of('.'), filename.size()-filename.find_last_of('.') );

    // Load Model
    if( extension.compare(".obj") == 0 )
        g_oMesh = std::shared_ptr<Mesh>(new MeshOBJ);
    else if( extension.compare(".dxf") == 0 )
        g_oMesh = std::shared_ptr<Mesh>(new MeshDXF(Mesh::MESH_TYPE_QUADS) );
    g_oMesh->loadMesh( filename.c_str() );

    // calculate texture coordinates and camera matrices
    initModel();
}


void ShadowComputation::setStartDate(int y, int m, int d, int h, int min, int s)
{
    g_sStartDate.year = y;
    g_sStartDate.month = m;
    g_sStartDate.day = d;
    g_sStartDate.hour = h;
    g_sStartDate.minute = min;
    g_sStartDate.second = s;
}


void ShadowComputation::setEndDate(int y, int m, int d, int h, int min, int s)
{
    g_sStopDate.year = y;
    g_sStopDate.month = m;
    g_sStopDate.day = d;
    g_sStopDate.hour = h;
    g_sStopDate.minute = min;
    g_sStopDate.second = s;
}


void ShadowComputation::setLongitude(int deg, int min, int sec)
{
    g_ipLongitude[0]=deg;
    g_ipLongitude[1]=min;
    g_ipLongitude[2]=sec;
}


void ShadowComputation::setLatitude(int deg, int min, int sec)
{
    g_ipLatitude[0]=deg;
    g_ipLatitude[1]=min;
    g_ipLatitude[2]=sec;
}


void ShadowComputation::setTimeZone(int inputTimeZone)
{
    g_fTimeZone = inputTimeZone;
}


void ShadowComputation::setShadowParams( int faceTexRes )
{
    g_iFaceTexSize = faceTexRes;
}


void ShadowComputation::compute( int steps, bool smoothShading )
{
    // init the rest
    initFBOs();
    initShadowingPeriod();
    initLighting();
    initTextures();
    initSunPosition();
    initShadows();
    initShaders();
}


std::weak_ptr<Mesh> ShadowComputation::mesh()
{
    return std::weak_ptr<Mesh>(g_oMesh);
}


void ShadowComputation::initModel()
{
    // preprocess mesh
    g_oMesh->calculateNormals();
    g_oMesh->resizeMesh(1.0f);
    g_oMesh->centerMesh(0.0f,g_oMesh->getSizeY()/2.0f,0.0f);
    g_fGroundLevel = g_oMesh->getSizeY()/2.0f; // "land" mesh on the ground

    // compute the texture atlas
    g_oMesh->computeTextureAtlas( g_iShadowTexSize, g_iShadowTexRowCount, &g_fpCameraMatrices, &g_fpCameraClipPlanes , g_fTexelOffset );
}


void ShadowComputation::initFBOs()
{
	printf( "Initializing Frame Buffer Objects...");

    // FBOs
    g_oShadowFBO.init();
    g_oMeshTexFBO.init();
    g_oGroundTexFBO.init();

	// generate color buffer textures
    g_iShadowTexRowCount = g_iShadowTexSize / g_iFaceTexSize;
	g_iShadowTextureCount = g_iCameraMatrixCount / (g_iShadowTexRowCount*g_iShadowTexRowCount);
	if( (g_iCameraMatrixCount % (g_iShadowTexRowCount*g_iShadowTexRowCount)) != 0) g_iShadowTextureCount++;
	printf("g_iShadowTextureCount=%i\n",g_iShadowTextureCount);
    g_ipShadowTextures.clear();
    g_ipShadowTextures.resize(g_iShadowTextureCount);
	for(int i=0;i<g_iShadowTextureCount;i++)
	{
        g_ipShadowTextures[i].set_format( GL_RGB );
        g_ipShadowTextures[i].set_data( g_iShadowTexSize, g_iShadowTexSize, 0 );
	}

    // ground tex
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&g_iGroundShadowTexSize);
    g_oGroundTexFBO.clear();
    g_oGroundTex.set_format( GL_RGB );
    g_oGroundTex.set_data( g_iGroundShadowTexSize, g_iGroundShadowTexSize, 0 );
    g_oGroundTexFBO.attach_color_texture( g_oGroundTex );
	printf( "done\n");
}


void ShadowComputation::initSunPosition()
{
	// calculate sun position
	printf( "Initializing Sun Position...");
	sunPosition.setPosition(g_ipLatitude,g_ipLongitude,g_fTimeZone);
	sunPosition.setDate(g_sCurrentDate);
	sunPosition.calculate();
	
	// update local azimuth and elevation
	g_fLightAzimuth = (float)sunPosition.azimuthOut;
	g_fLightElevation = (float)sunPosition.elevationOut;
	printf( "done\n");
}

// OBSOLETE because all the lighting calculations are done by shaders
// set's up the lighting
void ShadowComputation::initLighting()
{
	printf( "Initializing Light...");
	
	g_pfLightPos = new float[4];
	for(int i=0;i<4;i++)
	{
		g_pfLightPos[i]=1.0f;
	}
	g_fLightElevation=45.0f;
	g_fLightAzimuth=45.0f;
	g_fLightTransZ=5.0f;
	//float g_pfLightDiffuse[4] = {0.99f,0.99f,1.0f,1.0f};
	//float g_pfLigthAmbient[4] = {0.0f,0.0f,0.0f,1.0f};
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glMaterialf( GL_FRONT, GL_SHININESS, 50.0);
	//glMaterialfv( GL_FRONT, GL_SPECULAR, g_pfLightSpecular);
	//glLightModelf( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	//glLightfv( GL_LIGHT0, GL_POSITION, g_pfLightPos);
	//glLightfv( GL_LIGHT0, GL_AMBIENT, g_pfLigthAmbient);
	//glLightfv( GL_LIGHT0, GL_DIFFUSE, g_pfLightDiffuse);
	//glLightfv( GL_LIGHT0, GL_SPECULAR, g_pfLightSpecular);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//renderLights();
	updateLightPosition();

	printf( "done\n");
}

// OBSOLETE, shoule be replaced by the lightscattering sky dome
void ShadowComputation::initTextures()
{
	printf( "Initializing Textures...");
    g_iSunTex = genExpSprite<float>(32,1.0);
	printf( "done\n");
}

// init the shadow map FBO with the maximum texture size
void ShadowComputation::initShadows()
{
	printf( "Initializing Shadows...");
	
	// get the max texture size
	//glGetIntegerv(GL_MAX_TEXTURE_SIZE,&g_iShadowMapSize);
	
	// initialize Schadow FBO
	//g_oShadowFBO.init(g_iShadowMapSize,FBO_DEPTH_TEX | FBO_COLOR_TEX, GL_RGB16F_ARB   ,GL_RGB,GL_FLOAT);
	//g_oShadowFBO.init(g_iShadowMapSize,FBO_DEPTH_TEX | FBO_COLOR_TEX, GL_RGB ,GL_RGB, GL_FLOAT);
	
	//Initializing Shadows
	//updateMeshShadowTexture();

	// init shadow mapping matrices
	g_pfBiasMatrix = new float[16];
	g_pfLightProjection = new float[16];
	g_pfLightModelView = new float[16];
	for(int i=0;i<16;i++)
	{
		g_pfBiasMatrix[i]=0.0f;
		g_pfLightProjection[i]=0.0f;
		g_pfLightModelView[i]=0.0f;
	}
	g_pfBiasMatrix[0] = 0.5f;
	g_pfBiasMatrix[5] = 0.5f;
	g_pfBiasMatrix[10] = 0.5f;
	g_pfBiasMatrix[12] = 0.5f;
	g_pfBiasMatrix[13] = 0.5f;
	g_pfBiasMatrix[14] = 0.5f;
	g_pfBiasMatrix[15] = 1.0f;

	// initialize shadow map
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&g_iShadowMapSize);
    g_oShadowTex.set_format( GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT );
    g_oShadowTex.set_data( g_iShadowMapSize, g_iShadowMapSize, 0 );
    g_oShadowFBO.attach_depth_texture( g_oShadowTex );
	
	printf( "done\n");
}


void ShadowComputation::initShaders()
{
	// init Depth Texture
    depthTexture.load_vertex_shader( "shaders/depthTexture.vert" );
    depthTexture.load_fragment_shader( "shaders/depthTexture.frag" );

	// init Shadow Mapping
    shadowMapping.load_vertex_shader( "shaders/shadowMapping.vert" );
    shadowMapping.load_fragment_shader( "shaders/shadowMapping.frag" );
	
	// init Shadow Mapping
    sceneRendering.load_vertex_shader( "shaders/sceneRendering.vert" );
    sceneRendering.load_fragment_shader( "shaders/sceneRendering.frag" );
}


void ShadowComputation::initShadowingPeriod()
{

    g_sCurrentDate.year   = g_sStartDate.year;
    g_sCurrentDate.month  = g_sStartDate.month;
    g_sCurrentDate.day    = g_sStartDate.day;
    g_sCurrentDate.hour   = g_sStartDate.hour;
    g_sCurrentDate.minute = g_sStartDate.minute;
    g_sCurrentDate.second = g_sStartDate.second;

    g_dStartDate = sunPosition.dateToJulian(g_sCurrentDate);
    g_dCurrentDate = g_dStartDate;
    g_dStopDate = sunPosition.dateToJulian(g_sStopDate);

    g_dShadowingPeriod = g_dStopDate - g_dStartDate;

    g_dTimePerFrame = g_dShadowingPeriod / (double)g_iSteps;
}



// update the shadow map
void ShadowComputation::updateShadowMap()
{
	// setup and copy the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( 0.0f , 0.0f , -2.0f );	// Zoom
	glRotatef(g_fLightElevation, 1.0f, 0.0f, 0.0f );// rotate elevation
	glRotatef(g_fLightAzimuth - 90.0f, 0.0f, 1.0f, 0.0f);	// rotate azimuth
	glTranslatef( 0.0f, -g_fGroundLevel, 0.0f); // compensate for groundlevel
	glGetFloatv(GL_MODELVIEW_MATRIX, g_pfLightModelView);
	glPopMatrix();

	// setup and copy the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 5.0f);
	glGetFloatv(GL_PROJECTION_MATRIX, g_pfLightProjection);
	glPopMatrix();

	// load modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(g_pfLightModelView);

	// load projetion Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(g_pfLightProjection);

	// setup viewPort
	glViewport(0,0,g_iShadowMapSize,g_iShadowMapSize);

	// setup culling
	glCullFace(GL_BACK);

	// render the Model
	g_oShadowFBO.enable();
	depthTexture.disable();
	glClearColor(1,1,1,1);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    g_oMesh->VBO().draw();
	depthTexture.disable();
	g_oShadowFBO.disable();
}


// This methode renders each polygon with shadows into a texture
void ShadowComputation::updateShadowAtlas()
{
	// Setup the mask
	// clear the FBO's to (0,0,1,0) 
	for(int i=0;i<g_iShadowTextureCount;i++)
	{
        g_oMeshTexFBO.attach_color_texture( g_ipShadowTextures[i] );
        g_oMeshTexFBO.enable();

		glViewport(0,0,g_iShadowTexSize,g_iShadowTexSize);
		glClearColor(0.0f,0.0f,1.0f,0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g_oMeshTexFBO.disable();
	}

	// draw black geometry
    for(int i=0;i<g_oMesh->getVertexCount();i++)
	{
		int viewPortSize = g_iShadowTexSize / g_iShadowTexRowCount;

		int indexTex = i / (g_iShadowTexRowCount*g_iShadowTexRowCount);
		int index = i % (g_iShadowTexRowCount*g_iShadowTexRowCount);

		int tempRow = index % g_iShadowTexRowCount;
		int tempColumn = index / g_iShadowTexRowCount;

		int offsetX = tempColumn * (g_iShadowTexSize / g_iShadowTexRowCount);
		int offsetY = tempRow * (g_iShadowTexSize / g_iShadowTexRowCount);

		glViewport(offsetX,offsetY,viewPortSize,viewPortSize);	// and set new dimensions
		glMatrixMode(GL_PROJECTION);			// reset PROJECTION matrix
		glLoadIdentity();
		glOrtho(g_fpCameraClipPlanes[i*4+0],g_fpCameraClipPlanes[i*4+1],g_fpCameraClipPlanes[i*4+2],g_fpCameraClipPlanes[i*4+3],-10,+10);

		// Set up camera (just over the primitive)
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&(g_fpCameraMatrices[i*16]));
		
		// render the primitive
        g_oMeshTexFBO.attach_color_texture( g_ipShadowTextures[indexTex] );
        g_oMeshTexFBO.enable();

		glColor4f(0.0f,0.0f,0.0f,0.0f);
	
        g_oMesh->VBO().draw_elements(i,1);
			
		g_oMeshTexFBO.disable();

	}

	// clear the ground texture
	g_oGroundTexFBO.enable();
	glViewport(0,0,g_iGroundShadowTexSize,g_iGroundShadowTexSize);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	g_oGroundTexFBO.disable();

	// render model textures
	for(int j=0; j<g_iSteps; j++)
	{
		// update sun position
		if(j==0)	initShadowingPeriod();
		else	updateCurrentDate();
		updateSunPosition();
		updateLightPosition();
		
		// update schadow map
		updateShadowMap();

		// load shader
		shadowMapping.enable();

		// Bind textures
        glActiveTexture(GL_TEXTURE0);
        g_oShadowTex.bind();
		glEnable(GL_TEXTURE_2D);

		// enable blending
        glEnable(GL_BLEND);
		glBlendFunc(GL_ONE,GL_ONE);

		int viewPortSize = g_iShadowTexSize / g_iShadowTexRowCount;
		
        float* faceNormals = g_oMesh->getFaceNormals();
        for(int i=0;i<g_oMesh->getVertexCount();i++)
		{
			int indexTex = i / (g_iShadowTexRowCount*g_iShadowTexRowCount);
			int index = i % (g_iShadowTexRowCount*g_iShadowTexRowCount);
	
			int tempRow = index % g_iShadowTexRowCount;
			int tempColumn = index / g_iShadowTexRowCount;
	
			int offsetX = tempColumn * (g_iShadowTexSize / g_iShadowTexRowCount);
			int offsetY = tempRow * (g_iShadowTexSize / g_iShadowTexRowCount);
	
			// calculate the cosinus of the angle between the light vector and the surface normal
			// do not render the face if the the angle is greater than 90 (i.e. cosinus of the angle is negative)
            float cosAngle = dot(g_pfLightPos, &faceNormals[i*3]);
			if( g_bSmoothShading || ((cosAngle >= 0.0f) && (g_fLightElevation > 0.0f)) )
			{
				glViewport(offsetX,offsetY,viewPortSize,viewPortSize);	// and set new dimensions
	
				// Set up camera (just over the primitive)
				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixf(&(g_fpCameraMatrices[i*16]));
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(g_fpCameraClipPlanes[i*4+0],g_fpCameraClipPlanes[i*4+1],g_fpCameraClipPlanes[i*4+2],g_fpCameraClipPlanes[i*4+3],-10,+10);
	
				// setup TextureMatrix
				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				glMultMatrixf(g_pfBiasMatrix);
				glMultMatrixf(g_pfLightProjection);
				glMultMatrixf(g_pfLightModelView);
				glMatrixMode(GL_MODELVIEW);
	
				// render the primitive
                g_oMeshTexFBO.attach_color_texture( g_ipShadowTextures[indexTex] );
                g_oMeshTexFBO.enable();
                glColor4f(cosAngle, (float)((int)g_bSmoothShading), 0.0f, 1.0f/(float)g_iSteps);
                g_oMesh->VBO().draw_vertices(i,4);
				g_oMeshTexFBO.disable();
			}
		}
		
		// render the ground shadow
		float grpundNormal[3] = {0.0f, 1.0f, 0.0f};
		float cosAngle = dot(g_pfLightPos, grpundNormal);
		if( g_bSmoothShading || ((cosAngle >= 0.0f) && (g_fLightElevation > 0.0f)) )
		{
			glViewport(0,0,g_iGroundShadowTexSize,g_iGroundShadowTexSize);

			// Set up camera (just over the primitive)
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glRotatef(90.0f,1.0f,0.0f,0.0f);
			glRotatef(180.0f,0.0f,1.0f,0.0f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-2.0f,2.0f,-2.0f,2.0f,-2.0f,2.0f);

			// setup TextureMatrix
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMultMatrixf(g_pfBiasMatrix);
			glMultMatrixf(g_pfLightProjection);
			glMultMatrixf(g_pfLightModelView);
			glMatrixMode(GL_MODELVIEW);
	
			// render the ground
			g_oGroundTexFBO.enable();
            glColor4f(cosAngle, (float)((int)g_bSmoothShading), 0.0f, 1.0f/(float)g_iSteps);
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
			g_oGroundTexFBO.disable();
		}

		// disable stuff
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D,0);
		glDisable(GL_TEXTURE_2D);
		shadowMapping.disable();
	}
}



// update the current time according to the elapsed time
void ShadowComputation::updateCurrentDate()
{
    if(g_dShadowingPeriod <= 0  )
    {
        return;
        //if(!g_bToggleLoopAnimation)	return;
        //
        //// reinit g_dDeltaMinutes
        //g_dDeltaMinutes = g_dStopDate - g_dStartDate;
    }
    else
    {
        g_dShadowingPeriod -= g_dTimePerFrame;
        g_dCurrentDate += g_dTimePerFrame;
        g_sCurrentDate = sunPosition.julianToDate(g_dCurrentDate);
    }

    //printf("%i %i %i %i %i %i %s %f\n",g_ipCurrentDate[0],g_ipCurrentDate[1],g_ipCurrentDate[2],g_ipCurrentDate[3],g_ipCurrentDate[4],g_ipCurrentDate[5],g_cpWeekdays[(int)fmod(g_dCurrentDate,7.0)],g_dCurrentDate);
}

// calculate new sun position based on the current date
void ShadowComputation::updateSunPosition()
{
    // update sunPosition
    sunPosition.setDate(g_sCurrentDate);
    sunPosition.calculate();

    // update local azimuth and elevation
    g_fLightAzimuth = (float)sunPosition.azimuthOut;
    g_fLightElevation = (float)sunPosition.elevationOut;

    updateLightPosition();
}

// set the Light position Array, by projecting the azimuth and elevation angles
void ShadowComputation::updateLightPosition()
{
    // x component
    g_pfLightPos[0]	= cos(degToRad(g_fLightAzimuth)) * cos(degToRad(g_fLightElevation));

    // y component
    g_pfLightPos[1] = sin(degToRad(g_fLightElevation));

    // z component
    g_pfLightPos[2] = sin(degToRad(g_fLightAzimuth)) * cos(degToRad(g_fLightElevation));

    // normalize Light vector
    float vLength = sqrt(g_pfLightPos[0]*g_pfLightPos[0] + g_pfLightPos[1]*g_pfLightPos[1] + g_pfLightPos[2]*g_pfLightPos[2]);
    g_pfLightPos[0] /= vLength;
    g_pfLightPos[1] /= vLength;
    g_pfLightPos[2] /= vLength;

    // set the light position in the openGL state
    float *lightVector = new float[4];
    lightVector[0] = g_pfLightPos[0];
    lightVector[1] = g_pfLightPos[2];
    lightVector[2] = g_pfLightPos[1];
    lightVector[3] = 1.0f;
    glLightfv( GL_LIGHT0, GL_POSITION, lightVector);
}
