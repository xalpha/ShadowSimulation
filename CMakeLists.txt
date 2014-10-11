##############################################################################
#                                                                            #
# This file is part of ShadowSimulation, a C++ app for schadow simulation    #
#                                                                            #
# Copyright (C) 2006 Alexandru Duliu                                         #
#                                                                            #
# ShadowSimulation is free software; you can redistribute it and/or          #
# modify it under the terms of the GNU  General Public License               #
# as published by the Free Software Foundation; either version 3             #
# of the License, or (at your option) any later version.                     #
#                                                                            #
# ShadowSimulation is distributed in the hope that it will be useful,        #
# but WITHOUT ANY WARRANTY; without even the implied warranty of             #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
# GNU General Public License for more details.                               #
#                                                                            #
# You should have received a copy of the GNU General Public License          #
# along with ShadowSimulation. If not, see <http://www.gnu.org/licenses/>.   #
#                                                                            #
##############################################################################


cmake_minimum_required(VERSION 2.8)
project( ShadowSimulation CXX )

set( CMAKE_BUILD_TYPE Debug )
if( NOT WIN32 )
    set( BUILD_SHARED_LIBS TRUE )
endif()

# add module paths
list( APPEND CMAKE_MODULE_PATH
    ${CMAKE_INSTALL_PREFIX}/share
    ${CMAKE_CURRENT_LIST_DIR}/cmake
    $ENV{HOME}/.local/share )

# find dependencies
find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( Dime REQUIRED )

# find Qt
if( WIN32 )
    set( QT_QMAKE_EXECUTABLE "$ENV{Qt_DIR}/bin/qmake.exe" )
endif()
find_package( Qt4 COMPONENTS QtCore QtGui QtOpenGL QtXml )


# some more qt stuff
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

# set the target
set( ShadowSimulation_Target shadowSimulation )

# set the header files of QObject derived classes
set( ShadowSimulation_INC
    include/Mesh.hpp
    include/MeshDXF.hpp
    include/MeshOBJ.hpp
    include/SunPosition.hpp
    include/ShadowComputation.hpp
    include/ShadowSimulation.hpp
    include/ShadowView.hpp
    include/util.hpp
    extern/cimg/CImg.h
    extern/nyx/nyx/array_buffer.hpp
    extern/nyx/nyx/buffer.hpp
    extern/nyx/nyx/color_array_buffer.hpp
    extern/nyx/nyx/element_buffer.hpp
    extern/nyx/nyx/exception.hpp
    extern/nyx/nyx/frame_buffer_object.hpp
    extern/nyx/nyx/gl.hpp
    extern/nyx/nyx/normal_array_buffer.hpp
    extern/nyx/nyx/program.hpp
    extern/nyx/nyx/shader.hpp
    extern/nyx/nyx/texcoord_array_buffer.hpp
    extern/nyx/nyx/texture.hpp
    extern/nyx/nyx/util.hpp
    extern/nyx/nyx/vertex_array_buffer.hpp
    extern/nyx/nyx/vertex_buffer_object.hpp )
set( ShadowSimulation_SRC 
    src/main.cpp
    src/Mesh.cpp
    src/MeshDXF.cpp
    src/MeshOBJ.cpp
    src/SunPosition.cpp
    src/ShadowComputation.cpp
    src/ShadowSimulation.cpp
    src/ShadowView.cpp )

set( ShadowSimulation_QT_UI 
    ui/ShadowSimulation.ui )

# set include directories
set( ShadowSimulation_INCLUDE_DIRS
    include
    extern/cimg/
    extern/nyx/
    $ENV{HOME}/.local/include
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_PATH}
    ${Dime_INCLUDE_PATH}
    ${CMAKE_CURRENT_BINARY_DIR} )

# set link libraries
set( ShadowSimulation_LINK_LIBS
    ${QT_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    ${Dime_LIBRARY} )

# set compile definitions
set( ShadowSimulation_COMPILE_DEFINITIONS
    QT_NO_DEPRECATED
    _USE_MATH_DEFINES )

# run the qt moc & uic
#QT4_ADD_RESOURCES( ShadowSimulation_QT_RES_SRC ${ShadowSimulation_QT_RES})
QT4_WRAP_CPP( ShadowSimulation_QT_HPP_MOC
    ${ShadowSimulation_INC} )
QT4_WRAP_UI( ShadowSimulation_QT_UI_UIC ${ShadowSimulation_QT_UI} )

# set include paths
include_directories( ${ShadowSimulation_INCLUDE_DIRS} )

# add the exec
add_executable( ${ShadowSimulation_Target}
    ${ShadowSimulation_SRC}
    ${ShadowSimulation_INC}
    ${ShadowSimulation_QT_HPP_MOC}
    ${ShadowSimulation_QT_UI_UIC} )

# enable C++11 support
if( NOT WIN32 )
    if( CMAKE_COMPILER_IS_GNUCXX )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
    else( CMAKE_COMPILER_IS_GNUCXX )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Qunused-arguments")
    endif()
endif()

# set link libraries
target_link_libraries( ${ShadowSimulation_Target} ${ShadowSimulation_LINK_LIBS} )

# configure target
#set_target_properties(${ShadowSimulation_Target}
#    PROPERTIES
#        PUBLIC_HEADER "${ShadowSimulation_INC}"
#        COMPILE_DEFINITIONS "${ShadowSimulation_COMPILE_DEFINITIONS}" )
