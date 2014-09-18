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
#ifndef FRAME_H_
#define FRAME_H_

# include <stdio.h>
# include <stdlib.h>	
# include <math.h>
# include <string.h>

# include <GL/glew.h>
//# include <GL/glut.h>

#include <QtGui>
//#include <QtOpenGL>
#include <QGLWidget>

//# include "Tools.h"
# include "normalize.h"
# include "texture.h"
# include "vbObject.h"
# include "sunPosition.h"
# include "fbObject.h"
# include "mathutils.h"
# include "shader.h"
# include "meshDXF.h"
# include "meshOBJ.h"

class FrameQT : public QGLWidget
{
	Q_OBJECT

public:
	FrameQT(QWidget *parent=0);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	
	inline Date getStartDate();
	inline void setStartDate(int y, int m, int d, int h, int min, int s);
	
	inline Date getEndDate();
	inline void setEndDate(int y, int m, int d, int h, int min, int s);
	
	inline int *getLongitude();
	inline void setLongitude(int deg, int min, int sec);
	
	inline int *getLatitude();
	inline void setLatitude(int deg, int min, int sec);
	
	inline int getTimeZone();
	inline void setTimeZone(int inputTimeZone);
	
	inline int getSlidesCount();
	inline void setSlidesCount(int slides);

	inline bool getSmoothShading();
	inline void setSmoothShading(bool input);

    inline Mesh *getModel();

	inline void setSceneRenderingMode(float input);
	
	// unprotected methodes
	void initModel(char *fileName);
	void updateModel();
	void initFBOs();
	void updateMeshShadowTexture();

	
public slots:
	// SLOTS...

	
signals:
	// SIGNALS...


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
	inline void renderBackground();
	inline void renderSun();
	inline void renderGround();
	inline void renderEarth();
	inline void renderMesh();
	inline void renderMeshTex();
	inline void renderLights();

	// shadowing period sun positions
	inline void initShadowingPeriod();
	inline void updateCurrentDate();
	inline void updateSunPosition();
	inline void updateLightPosition();

	inline void setupUserViewProj();
	inline void setupCameraTexOrtho();

							
private:
	// Window properties
	int g_iWindowWidth;    // width of viewport.
	int g_iWindowHeight;    // height of viewport.
	float g_fAspect;        // aspect ratio of viewport
	
	// camera properties
	float g_fCameraElevation, g_fCameraAzimuth;
	float g_fCameraZoom;
	//bool g_bFullScreen;
	
	// mouse handling
	int g_iMousePosX,g_iMousePosY;

	// Light Controls
	float *g_pfLightPos;
	//bool g_bManualLightControl;
	float g_fLightElevation, g_fLightAzimuth, g_fLightTransZ;

	// Sun Position
	SunPosition sunPosition;
	Date g_sCurrentDate;	// year,month,day,hour,minute
	double g_dCurrentDate;
	//char *g_cpWeekdays[7] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
	//char *g_cpMonths[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};
	int *g_ipLatitude;
	int *g_ipLongitude;
	int g_fTimeZone;
	
	// Shadows
	//bool g_bToggleShadows;
	int g_iShadowMapSize;
	float *g_pfBiasMatrix;
	float *g_pfLightProjection;
	float *g_pfLightModelView;
	
	// Shadow Frame Buffer Object
	FBObject g_oShadowFBO;
	
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
	VBObject g_oMeshVBO;
	
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
	FBObject g_oMeshTexFBO;
	FBObject g_oGroundTexFBO;
	GLuint *g_ipShadowTextures;
	int g_iShadowTextureCount;
	
	// static textures
	// OBSOLETE
	GLuint g_iSunTex;
	
	// shadowing period
	//bool g_bToggleAnimation;
	//bool g_bToggleLoopAnimation;
	double g_dFPS;
	double g_dLastTime;
	Date g_sStartDate;
	double g_dStartDate;
	Date g_sStopDate;
	double g_dStopDate;
	double g_dShadowingPeriod;
	//static long g_lCurrentDeltaMinutes;
	double g_dTimePerFrame;
	int g_iSteps;
	
	/*
	 * SHADERS
	 */
	Shader shadowMapping;
	Shader depthTexture;
	Shader sceneRendering;
	float g_fSceneRenderingMode;

};

#include "frame.inl"

#endif
