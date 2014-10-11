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

MeshDXF::MeshDXF()
{

}

MeshDXF::MeshDXF(unsigned int meshType)
{

}

void MeshDXF::loadMesh(const char *path)
{
	FILE *stream; 
	stream = fopen(path, "r");
	if (stream != NULL)	fclose( stream );
	else
	{
		fprintf(stderr,"ERROR: File not found!\n");
		return;
	}

	dimeInput in;
	if (path == NULL) 
	{
		if (!in.setFileHandle(stdin)) 
		{
			fprintf(stderr,"ERROR: Unexpected error opening file from stdin\n");
			return;
		}
	}
	else 
	{
		if (!in.setFile(path)) 
		{
			fprintf(stderr,"ERROR: Error opening file for reading: %s\n",path );
			return;
		}
	}

	dimeModel model;
	if (!model.read(&in)) 
	{
		fprintf(stderr,"ERROR: DXF read error in line: %d\n", in.getFilePosition());
		return;
	}

	// delete old vertices
	if(vertexCount!=0)
	{
		printf("deleting old vertices\n");
		delete [] vertices;
		vertexCount=0;
	}

	tempIndex = 0;
	traverseEntitiesCounting(model);
	traverseBlocksCounting(model);
	vertexCount = tempIndex;
	vertices = new float[vertexCount*3];
	for(int i=0; i<vertexCount*3; i++)
	{
		vertices[i] = 0.0f;
	}
	printf("tempIndex=%i\n",tempIndex);
	tempIndex = 0;
	traverseEntitiesProcessing(model);
	traverseBlocksProcessing(model);

	analyzeMesh();

	meshType = Mesh::MESH_TYPE_QUADS;
	vertexPerFace = 4;
}


void MeshDXF::traverseEntitiesCounting(dimeModel &model)
{
	dimeEntitiesSection *entities = (dimeEntitiesSection*) model.findSection("ENTITIES");
	if(entities == NULL)	return;

	dimeEntity *tempEntity;
	for(int i=0; i<entities->getNumEntities(); i++)
	{
		tempEntity = entities->getEntity(i);

		if( strcmp(tempEntity->getEntityName(),"3DFACE")==0 ) tempIndex +=4;
		if( strcmp(tempEntity->getEntityName(),"POLYLINE")==0 ) countPolylineEntity(tempEntity);
	}
}


void MeshDXF::traverseEntitiesProcessing(dimeModel &model)
{
	dimeEntitiesSection *entities = (dimeEntitiesSection*) model.findSection("ENTITIES");
	if(entities == NULL)	return;

	dimeEntity *tempEntity;
	for(int i=0; i<entities->getNumEntities(); i++)
	{
		tempEntity = entities->getEntity(i);

		if( strcmp(tempEntity->getEntityName(),"3DFACE")==0 )	process3DFaceEntity(tempEntity);
		if( strcmp(tempEntity->getEntityName(),"POLYLINE")==0 )	processPolylineEntity(tempEntity);
	}
}


void MeshDXF::traverseBlocksCounting(dimeModel &model)
{
	dimeBlocksSection *blocks = (dimeBlocksSection*) model.findSection("BLOCKS");
	if(blocks == NULL)	return;

	dimeBlock *tempBlock;
	dimeEntity *tempEntity;
	for(int i=0; i<blocks->getNumBlocks();i++)
	{
		tempBlock = blocks->getBlock(i);

		for(int j=0; j<tempBlock->getNumEntities(); j++)
		{
			tempEntity = tempBlock->getEntity(j);

			if( strcmp(tempEntity->getEntityName(),"3DFACE")==0 ) tempIndex +=4;
			if( strcmp(tempEntity->getEntityName(),"POLYLINE")==0 ) countPolylineEntity(tempEntity);

			//printf("%s\n",tempEntity->getEntityName());
		}
	}
}


void MeshDXF::traverseBlocksProcessing(dimeModel &model)
{
	dimeBlocksSection *blocks = (dimeBlocksSection*) model.findSection("BLOCKS");
	if(blocks == NULL)	return;

	dimeBlock *tempBlock;
	dimeEntity *tempEntity;
	for(int i=0; i<blocks->getNumBlocks();i++)
	{
		tempBlock = blocks->getBlock(i);

		for(int j=0; j<tempBlock->getNumEntities(); j++)
		{
			tempEntity = tempBlock->getEntity(j);

			if( strcmp(tempEntity->getEntityName(),"3DFACE")==0 )	process3DFaceEntity(tempEntity);
			if( strcmp(tempEntity->getEntityName(),"POLYLINE")==0 )	processPolylineEntity(tempEntity);
		}
	}
}


void MeshDXF::process3DFaceEntity(dimeEntity *entity)
{
	//float *vertices = currentMesh->getVertices();
	//int tempIndex = currentMesh->tempIndex;
	dimeVec3f vert0, vert1, vert2, vert3;

	dime3DFace *faceEntity = (dime3DFace*)entity;
	faceEntity->getVertices(vert0, vert1, vert2, vert3);

	if(faceEntity->isQuad())
	{
		vertices[(tempIndex*4 +0)*3 +0] = (float) vert0.x;
		vertices[(tempIndex*4 +0)*3 +1] = (float) vert0.y;
		vertices[(tempIndex*4 +0)*3 +2] = (float) vert0.z;

		vertices[(tempIndex*4 +1)*3 +0] = (float) vert1.x;
		vertices[(tempIndex*4 +1)*3 +1] = (float) vert1.y;
		vertices[(tempIndex*4 +1)*3 +2] = (float) vert1.z;

		vertices[(tempIndex*4 +2)*3 +0] = (float) vert2.x;
		vertices[(tempIndex*4 +2)*3 +1] = (float) vert2.y;
		vertices[(tempIndex*4 +2)*3 +2] = (float) vert2.z;

		vertices[(tempIndex*4 +3)*3 +0] = (float) vert3.x;
		vertices[(tempIndex*4 +3)*3 +1] = (float) vert3.y;
		vertices[(tempIndex*4 +3)*3 +2] = (float) vert3.z;

		//printf("\t\t\t\t\tquad\n");
	}
	else
	{
		/*
		vertices[(tempIndex*3 +0)*3 +0] = (float) vert0.x;
		vertices[(tempIndex*3 +0)*3 +1] = (float) vert0.y;
		vertices[(tempIndex*3 +0)*3 +2] = (float) vert0.z;

		vertices[(tempIndex*3 +1)*3 +0] = (float) vert1.x;
		vertices[(tempIndex*3 +1)*3 +1] = (float) vert1.y;
		vertices[(tempIndex*3 +1)*3 +2] = (float) vert1.z;

		vertices[(tempIndex*3 +2)*3 +0] = (float) vert2.x;
		vertices[(tempIndex*3 +2)*3 +1] = (float) vert2.y;
		vertices[(tempIndex*3 +2)*3 +2] = (float) vert2.z;
		*/

		vertices[(tempIndex*4 +0)*3 +0] = (float) vert0.x;
		vertices[(tempIndex*4 +0)*3 +1] = (float) vert0.y;
		vertices[(tempIndex*4 +0)*3 +2] = (float) vert0.z;

		vertices[(tempIndex*4 +1)*3 +0] = (float) vert1.x;
		vertices[(tempIndex*4 +1)*3 +1] = (float) vert1.y;
		vertices[(tempIndex*4 +1)*3 +2] = (float) vert1.z;

		vertices[(tempIndex*4 +2)*3 +0] = (float) vert2.x;
		vertices[(tempIndex*4 +2)*3 +1] = (float) vert2.y;
		vertices[(tempIndex*4 +2)*3 +2] = (float) vert2.z;

		vertices[(tempIndex*4 +3)*3 +0] = (float) vert2.x;
		vertices[(tempIndex*4 +3)*3 +1] = (float) vert2.y;
		vertices[(tempIndex*4 +3)*3 +2] = (float) vert2.z;
		
		//printf("\t\t\t\t\ttriangle\n");
	}
	
	/*
	printf("%f\t%f\t%f\n", vert0.x, vert0.y, vert0.z);
	printf("%f\t%f\t%f\n", vert1.x, vert1.y, vert1.z);
	printf("%f\t%f\t%f\n", vert2.x, vert2.y, vert2.z);
	printf("%f\t%f\t%f\n", vert3.x, vert3.y, vert3.z);
	*/

	//printf("processing 3DFACE\n");
	tempIndex+=1;
}


void MeshDXF::countPolylineEntity(dimeEntity *entity)
{
    dimePolyline *polylineEntity = (dimePolyline*) entity;

	dimeArray<dimeVec3f> tempVertices;
	dimeArray<int> tempIndices;
	dimeVec3f extrusionVector;
	dxfdouble thickness;

	entity->extractGeometry(tempVertices, tempIndices, extrusionVector, thickness);

    printf("tempVertices.length=%i\n",tempVertices.count());
    printf("tempIndices.length=%i\n",tempIndices.count());
    printf("thickness=%f\n",thickness);

	tempIndex+=tempIndices.count();
	
}


void MeshDXF::processPolylineEntity(dimeEntity *entity)
{
	dimePolyline *polylineEntity = (dimePolyline*) entity;

	dimeArray<dimeVec3f> tempVertices;
	dimeArray<int> tempIndices;
	dimeVec3f extrusionVector;
	dxfdouble thickness;

	entity->extractGeometry(tempVertices, tempIndices, extrusionVector, thickness);

	for(int i=0; i<tempIndices.count(); i++)
	{
		
	}
}

