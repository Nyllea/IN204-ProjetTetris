cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -rdynamic")

project(Tetris)

# From https://gist.github.com/fracek/3323924#file-cmakelists-txt-L21

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system filesystem thread REQUIRED)
PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-3.0)
PKG_CHECK_MODULES(PORTAUDIO2 portaudio-2.0)
PKG_CHECK_MODULES(SNDFILE REQUIRED sndfile)

#add_subdirectory("./portaudio" EXCLUDE_FROM_ALL)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${PORTAUDIO2_INCLUDE_DIRS})
LINK_DIRECTORIES(${PORTAUDIO2_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${SNDFILE_INCLUDE_DIRS})
LINK_DIRECTORIES(${SNDFILE_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTKMM_CFLAGS_OTHER})
ADD_DEFINITIONS(${PORTAUDIO2_CFLAGS_OTHER})
ADD_DEFINITIONS(${SNDFILE_CFLAGS_OTHER})
ADD_DEFINITIONS(${Boost_CFLAGS_OTHER})

add_executable(Tetris AudioManager.cpp TetrisElements-Piece.cpp TetrisElements-Terrain.cpp TetrisElements_Graphic-Piece.cpp Random.cpp TimeManager.cpp TetrisElements_Graphic-Terrain.cpp GameWindow.cpp main.cpp)
TARGET_LINK_LIBRARIES(Tetris ${GTKMM_LIBRARIES} ${PORTAUDIO2_LIBRARIES} ${SNDFILE_LIBRARIES} ${Boost_LIBRARIES})