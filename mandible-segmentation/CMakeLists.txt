################################################################################
# Copyright (C) 2015 Eslam Adel <eslam.adel.mahmoud.ali@gmail.com>
# Cluster Rendering Project
################################################################################

CMAKE_MINIMUM_REQUIRED( VERSION 3.2 FATAL_ERROR)
SET( CMAKE_CXX_STANDARD 11 )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
PROJECT( mandible-segmentation )

# Add the CMAKE directory into the source tree
SET( CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )

# Adding VTK libraries
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
link_libraries(${VTK_LIBRARIES})

include( Qt )
include( OpenMP )

# Add the src directory to the CMake tree and set the source/header/ui files
add_subdirectory( src )

message("Project Executables will be built to <project_dir>/bin")
