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

/***************************************************************
 *                                                             *
 * program  : Shadow Simulation                                *
 * author   : Alexandru Duliu                                  *
 * version  : 0.6                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/

#pragma once

# include <stdio.h>
# include <stdlib.h>	
# include <math.h>
# include <string.h>

# include <QGLWidget>
# include <GL/glew.h>

#include <nyx/texture.hpp>
#include <nyx/frame_buffer_object.hpp>
#include <nyx/vertex_buffer_object.hpp>
#include <nyx/shader.hpp>
#include <nyx/program.hpp>

# include "SunPosition.hpp"
# include "MeshDXF.hpp"
# include "MeshOBJ.hpp"
# include "util.hpp"


class ShadowView : public QGLWidget
{
public:
    ShadowView( QObject* parent=0 );

    inline Mesh *getModel();

	inline void setSceneRenderingMode(float input);
	
	// unprotected methodes
	void initModel(char *fileName);
	void updateModel();
	void initFBOs();
	void updateMeshShadowTexture();


protected:
	// initialize the program
	void initLighting();
	void initTextures();
	void initSunPosition();
	void initShadows();
	void initShaders();
	void initExtensions(); // TODO gets the wrong flags
	bool fileExists(char* filename);

	// standard Qt OpenGL functions
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent( QKeyEvent * event);
	// TODO keyboard function
	
	// update shadows
	void updateShadowMap();
	
	// render functions
    void renderBackground();
    void renderSun();
    void renderGround();
    void renderEarth();
    void renderMesh();
    void renderMeshTex();
    void renderLights();

	// shadowing period sun positions
    void initShadowingPeriod();
    void updateCurrentDate();
    void updateSunPosition();
    void updateLightPosition();

    void setupUserViewProj();
    void setupCameraTexOrtho();

							
private:
	// Window properties
	int g_iWindowWidth;    // width of viewport.
	int g_iWindowHeight;    // height of viewport.
	float g_fAspect;        // aspect ratio of viewport
	
	// camera properties
	float g_fCameraElevation, g_fCameraAzimuth;
	float g_fCameraZoom;
	
	// mouse handling
	int g_iMousePosX,g_iMousePosY;

	// Light Controls
	float *g_pfLightPos;
	float g_fLightElevation, g_fLightAzimuth, g_fLightTransZ;

	// Sun Position
	SunPosition sunPosition;
	Date g_sCurrentDate;	// year,month,day,hour,minute
	double g_dCurrentDate;
	int *g_ipLatitude;
	int *g_ipLongitude;
	int g_fTimeZone;
	
	// Shadows
	int g_iShadowMapSize;
	float *g_pfBiasMatrix;
	float *g_pfLightProjection;
	float *g_pfLightModelView;
	
	// Shadow Frame Buffer Object
    nyx::texture<float> g_oShadowTex;
    nyx::frame_buffer_objects<float> g_oShadowFBO;

	//Model
	bool g_bMeshLoaded;
	bool g_bSmoothShading;
    Mesh *g_oMesh;
	
	// Mesh - geometry data
	float *g_fpVertices;
	int g_iVertexCount;
	float *g_fpNormals;
	int g_iNormalCount;
	float *g_fpFaceNormals;
	int g_iFaceNormalCount;
	float *g_fpTexCoords;
	int g_iTexCoordCount;
    nyx::vertex_buffer_object<float, unsigned int> g_oMeshVBO;
	
	// Mesh - secondary attributes
	float g_fGroundLevel;
	float *g_fpCameraMatrices;
	int g_iCameraMatrixCount;
	float *g_fpCameraClipPlanes;
	int g_iCameraClipPlaneCount;
	
	// Shadow Texture FBO's
	int g_iShadowTexSize;
	int g_iShadowTexRowCount;
	int g_iGroundShadowTexSize;
	float g_fTexelOffset;
    nyx::frame_buffer_objects<float> g_oMeshTexFBO;
    nyx::frame_buffer_objects<float> g_oGroundTexFBO;
    std::vector<nyx::texture<float> > g_ipShadowTextures;
    nyx::texture<float> g_oGroundTex;
	int g_iShadowTextureCount;
	
	// static textures
    nyx::texture<float> g_iSunTex;
	
	// shadowing period
	double g_dFPS;
	double g_dLastTime;
	Date g_sStartDate;
	double g_dStartDate;
	Date g_sStopDate;
	double g_dStopDate;
	double g_dShadowingPeriod;
	double g_dTimePerFrame;
	int g_iSteps;
	
	/*
	 * SHADERS
	 */
    nyx::shader_program shadowMapping;
    nyx::shader_program depthTexture;
    nyx::shader_program sceneRendering;
	float g_fSceneRenderingMode;

};
