################################################################################
# Copyright (C) 2016 Eslam Adel <eslam.adel.mahmoud.ali@gmail.com>
################################################################################

### Add the binary directory to the include and set off the mocs.
include_directories( ${PROJECT_BINARY_DIR} )



### Adding the source files to the CMake source tree
SET( MANDIBLE_SEGMENTATION_SOURCE_FILES
     system/Logger.cpp
     rendering/Render3D.cpp
     rendering/Render2D.cpp
     filters/ExtractVOI.cpp
     filters/Thresholding.cpp
     filters/Segmentation.cpp
    )

### Adding the header files to the CMake source tree
SET( MANDIBLE_SEGMENTATION_HEADER_FILES
     system/Logger.h
     system/Colors.hh
     system/LogLevel.hh
     rendering/Render3D.h
     rendering/Render2D.h
     filters/ExtractVOI.h
     filters/Thresholding.h
     filters/Segmentation.h
   )


### Include the directories to the CMake source tree

INCLUDE_DIRECTORIES( system )
INCLUDE_DIRECTORIES( rendering )
INCLUDE_DIRECTORIES( filters )


### ---------------------- RELEASE LIBRARY SETTING ---------------------------###


# Create a library with the main compoments of the framework and link the apps
# against it later to reduce the compilation time
set( MANDIBLE_SEGMENTATION_LIBRARY "mandibleSegmentation" )

add_library( ${MANDIBLE_SEGMENTATION_LIBRARY} SHARED
    ${MANDIBLE_SEGMENTATION_SOURCE_FILES}
    ${MANDIBLE_SEGMENTATION_HEADER_FILES}
     )


set_property( TARGET ${MANDIBLE_SEGMENTATION_LIBRARY} PROPERTY IMPORTED_LOCATION
    ${CMAKE_BINARY_DIRECTORY}/lib${MANDIBLE_SEGMENTATION_LIBRARY}.so )

install( TARGETS ${MANDIBLE_SEGMENTATION_LIBRARY} DESTINATION lib )
install( FILES ${MANDIBLE_SEGMENTATION_HEADER_FILES} DESTINATION include/${MANDIBLE_SEGMENTATION_LIBRARY} )

# Add the applications
add_subdirectory( apps )

