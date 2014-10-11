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
# include <memory>

# include <GL/glew.h>

#include <nyx/texture.hpp>
#include <nyx/frame_buffer_object.hpp>
#include <nyx/vertex_buffer_object.hpp>
#include <nyx/shader.hpp>
#include <nyx/program.hpp>

# include "SunPosition.hpp"
# include "Mesh.hpp"
# include "util.hpp"


class ShadowComputation
{
public:
    ShadowComputation();

    void loadModel( const std::string& filename );

	inline void setStartDate(int y, int m, int d, int h, int min, int s);
	inline void setEndDate(int y, int m, int d, int h, int min, int s);
	inline void setLongitude(int deg, int min, int sec);
	inline void setLatitude(int deg, int min, int sec);
	inline void setTimeZone(int inputTimeZone);

    void compute( int steps, bool smoothShading );

    std::weak_ptr<Mesh> mesh();

protected:
	// initialize the program
    void initModel();
    void initFBOs();
    void initSunPosition();
    void initLighting();
    void initTextures();
    void initShadows();
    void initShaders();
    void initShadowingPeriod();

	// update shadows
    void updateShadowAtlas();
	void updateShadowMap();
    void updateCurrentDate();
    void updateSunPosition();
    void updateLightPosition();
							
private:
    // model
    std::shared_ptr<Mesh> g_oMesh;

	// Light Controls
	float *g_pfLightPos;
	float g_fLightElevation, g_fLightAzimuth, g_fLightTransZ;

	// Sun Position
	SunPosition sunPosition;
	Date g_sCurrentDate;	// year,month,day,hour,minute
	double g_dCurrentDate;
    int g_ipLatitude[3];
    int g_ipLongitude[3];
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
    //Mesh *g_oMesh;
	
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
