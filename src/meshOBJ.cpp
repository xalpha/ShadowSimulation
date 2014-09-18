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

#include <stdexcept>
#include "meshOBJ.h"
#include <model_obj.hpp>

MeshOBJ::MeshOBJ()
{

}

MeshOBJ::MeshOBJ(unsigned int meshType)
{

}

void MeshOBJ::loadMesh(char *path)
{
    // try to load the model
    ModelOBJ reader;
    bool loaded = reader.import( path );

    // check if it was loaded
    if( loaded )
    {
        // let's see what we've got
        // NOTE: ModelOBJ automatically triangulates everything
        bool hasVertices = reader.hasPositions();

        // check if there are any vertices
        if( hasVertices )
        {
            // init stuff
            vertexCount = reader.getNumberOfVertices();
            indexCount = reader.getNumberOfTriangles();

            // reserve stuff
            vertices = new float[ vertexCount*3 ];
            indices = new unsigned int[ indexCount * 3 ];

            // copy vertex the data
            for( size_t i=0; i<vertexCount; i++ )
            {
                const ModelOBJ::Vertex &v = reader.getVertex( i );

                // copy position
                vertices[i*3+0] = v.position[0] ;
                vertices[i*3+1] = v.position[1] ;
                vertices[i*3+2] = v.position[2] ;
            }

            // copy triangle conectivity
            const int *meshIndices = reader.getIndexBuffer();
            for( size_t i=0; i<indexCount; i++ )
            {
                indices[i*3+0] = static_cast<unsigned int>( meshIndices[i*3 + 0] );
                indices[i*3+1] = static_cast<unsigned int>( meshIndices[i*3 + 1] );
                indices[i*3+2] = static_cast<unsigned int>( meshIndices[i*3 + 2] );
            }
        }
        else
            throw std::runtime_error( "MeshOBJ::loadMesh: no vertices." );
    }
    else
        throw std::runtime_error( "MeshOBJ::loadMesh: could not load the model." );

    analyzeMesh();

    meshType = Mesh::MESH_TYPE_TRIANGLES;
    vertexPerFace = 3;
}
