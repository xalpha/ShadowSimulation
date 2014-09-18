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
#include <fstream>
#include <sstream>
#include <vector>

#include "meshOBJ.h"

MeshOBJ::MeshOBJ()
{

}

MeshOBJ::MeshOBJ(unsigned int meshType)
{

}

void MeshOBJ::loadMesh(char *path)
{
    // try to load the model
    std::ifstream file;
    file.open( path );
    std::vector<float> vs;
    std::vector<unsigned int> is;

    // check if it was loaded
    if( file.is_open() )
    {
        while( !file.eof() )
        {
            // get the next line
            std::string line;
            std::getline( file, line );
            std::stringstream lineSS;
            lineSS<< line;
            lineSS.seekg( 1 );
            float tempV;
            float tempI;

            // interpret line
            switch( line[0] )
            {
                case 'v' : while( !lineSS.eof() ){ lineSS >> tempV; vs.push_back(tempV); } break;
                case 'f' : while( !lineSS.eof() ){ lineSS >> tempI; is.push_back(tempI); } break;
                default : break;
            }
        }

        // copy
        vertexCount = is.size();
        vertices = new float[is.size()*3];
        int idx=0;
        for( int i=0; i<is.size(); i++ )
            for( int j=0; j<3; j++ )
                vertices[idx++] = vs[ (is[i]-1)*3+j ];
    }
    else
        throw std::runtime_error( "MeshOBJ::loadMesh: could not load the model." );

    analyzeMesh();

    meshType = Mesh::MESH_TYPE_QUADS;
    vertexPerFace = 4;
}
