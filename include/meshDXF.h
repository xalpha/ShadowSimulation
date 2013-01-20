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
 * program  : Mesh                                             *
 * author   : Alexandru Duliu                                  *
 * version  : 0.1                                              *
 * e-mail   : duliu@in.tum.de                                  *
 *                                                             *
 ***************************************************************/
#ifndef MESHDXF_H_
#define MESHDXF_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef _WIN32
	#define DIME_DLL
#endif
//#include <dime/Basic.h>
#include <dime/Input.h>
#include <dime/Output.h>
#include <dime/Model.h>
#include <dime/State.h>
#include <dime/sections/BlocksSection.h>
#include <dime/sections/EntitiesSection.h>
#include <dime/entities/Entity.h>
	#include <dime/entities/Block.h>
	#include <dime/entities/FaceEntity.h>
		#include <dime/entities/3DFace.h>
		#include <dime/entities/Solid.h>
		#include <dime/entities/Trace.h>
	#include <dime/entities/ExtrusionEntity.h>
		#include <dime/entities/Arc.h>	
		#include <dime/entities/Circle.h>
		#include <dime/entities/Ellipse.h>
//		#include <dime/entities/Line.h>
//		#include <dime/entities/LWPolyline.h>
//		#include <dime/entities/Point.h>
        #include <dime/entities/Polyline.h>

#include "mesh.h"

class MeshDXF : public Mesh
{

private:
	// index variable for the DXF reader
	int tempIndex;

	void traverseEntitiesCounting(dimeModel &model);
	void traverseEntitiesProcessing(dimeModel &model);

	void traverseBlocksCounting(dimeModel &model);
	void traverseBlocksProcessing(dimeModel &model);

	//static bool countDXFEntity(const dimeState * const state, dimeEntity *entity, void *data);
	//static bool processDXFEntity(const dimeState * const state, dimeEntity *entity, void *data);

	void process3DFaceEntity(dimeEntity *entity);

	void countPolylineEntity(dimeEntity *entity);
	void processPolylineEntity(dimeEntity *entity);

public:
	// empty constructor
	MeshDXF();
	
	// basic contructor
	MeshDXF(unsigned int meshType);

	// initialize Mesh
	void loadMesh(char *path);
};

#endif
