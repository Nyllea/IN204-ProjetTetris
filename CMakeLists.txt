cmake_minimum_required(VERSION 3.22.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -rdynamic")

set(CODE_FOLDER "./Code")
set(AFF_GTK_FOLDER "./Code/Affichage-Gtk")
set(GAME_SYS_FOLDER "./Code/GameSystem")

project(Time_Tetris)

# From https://gist.github.com/fracek/3323924#file-cmakelists-txt-L21

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTKMM REQUIRED gtkmm-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTKMM_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTKMM_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTKMM_CFLAGS_OTHER})

add_executable(Time_Tetris ${GAME_SYS_FOLDER}/TetrisElements-Piece.cpp  ${GAME_SYS_FOLDER}/TetrisElements-Terrain.cpp ${AFF_GTK_FOLDER}/TetrisElements_Graphic-Piece.cpp ${CODE_FOLDER}/Random.cpp ${CODE_FOLDER}/TimeManager.cpp ${AFF_GTK_FOLDER}/TetrisElements_Graphic-Terrain.cpp ${CODE_FOLDER}/GameWindow.cpp ${CODE_FOLDER}/main.cpp)
TARGET_LINK_LIBRARIES(Time_Tetris ${GTKMM_LIBRARIES})