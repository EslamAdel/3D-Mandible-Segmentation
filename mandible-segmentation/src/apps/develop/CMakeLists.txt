## -------------------------- DEVELOP SETTING----------------------------------##

message("Develop Setting...")

################################ rendering3D_Develop
message("rendering3D_Develop ...")
set( VOLUME_RENDERING_EXECUTABLE "rendering3D_Develop" )



### Application source/header/ui files
set( VOLUME_RENDERING_SOURCE_FILES rendering3D.cpp )
set( VOLUME_RENDERING_HEADER_FILES )



### Generate the executable
add_executable( ${VOLUME_RENDERING_EXECUTABLE}
    ${VOLUME_RENDERING_SOURCE_FILES}
    ${VOLUME_RENDERING_HEADER_FILES}
)


### Link the application against the oclMGPUVR library
target_link_libraries( ${VOLUME_RENDERING_EXECUTABLE} ${MANDIBLE_SEGMENTATION_LIBRARY} )

### Make sure to use the required Qt modules
qt5_use_modules( ${VOLUME_RENDERING_EXECUTABLE} Core Widgets OpenGL Gui )

################### viewProjections_develope
message("viewProjections_develope ...")
set( IMAGE_VIEWER_EXECUTABLE "viewProjections_develope" )



### Application source/header/ui files
set( IMAGE_VIEWER_SOURCE_FILES viewProjections.cpp )
set( IMAGE_VIEWER_HEADER_FILES )



### Generate the executable
add_executable( ${IMAGE_VIEWER_EXECUTABLE}
    ${IMAGE_VIEWER_SOURCE_FILES}
    ${IMAGE_VIEWER_HEADER_FILES}
)


### Link the application against the oclMGPUVR library
target_link_libraries( ${IMAGE_VIEWER_EXECUTABLE} ${MANDIBLE_SEGMENTATION_LIBRARY} )

### Make sure to use the required Qt modules
qt5_use_modules( ${IMAGE_VIEWER_EXECUTABLE} Core Widgets OpenGL Gui )

################################# extractSubVol_develope
message("extractSubVol_Develop ...")
set( SUB_VOLUME_EXECUTABLE "extractSubVol_Develop" )



### Application source/header/ui files
set( SUB_VOLUME_SOURCE_FILES extractSubVol.cpp )
set( SUB_VOLUME_HEADER_FILES )



### Generate the executable
add_executable( ${SUB_VOLUME_EXECUTABLE}
    ${SUB_VOLUME_SOURCE_FILES}
    ${SUB_VOLUME_HEADER_FILES}
)


### Link the application against the oclMGPUVR library
target_link_libraries( ${SUB_VOLUME_EXECUTABLE} ${MANDIBLE_SEGMENTATION_LIBRARY} )

### Make sure to use the required Qt modules
qt5_use_modules( ${SUB_VOLUME_EXECUTABLE} Core Widgets OpenGL Gui )

######################################### segmentation_develop
message("segmentation_develop ...")
set( SEGMENTATION_EXECUTABLE "segmentation_develop" )



### Application source/header/ui files
set( SEGMENTATION_SOURCE_FILES segmentation.cpp )
set( SEGMENTATION_HEADER_FILES )



### Generate the executable
add_executable( ${SEGMENTATION_EXECUTABLE}
    ${SEGMENTATION_SOURCE_FILES}
    ${SEGMENTATION_HEADER_FILES}
)


### Link the application against the oclMGPUVR library
target_link_libraries( ${SEGMENTATION_EXECUTABLE} ${MANDIBLE_SEGMENTATION_LIBRARY} )

### Make sure to use the required Qt modules
qt5_use_modules( ${SEGMENTATION_EXECUTABLE} Core Widgets OpenGL Gui )


################################### install
# Install the executable (generate with 'make install' in the bin directory)
install( TARGETS ${MANDIBLE_SEGMENTATION_EXECUTABLE} DESTINATION bin )
