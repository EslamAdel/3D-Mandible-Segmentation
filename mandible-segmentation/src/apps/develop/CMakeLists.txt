## -------------------------- DEVELOP SETTING----------------------------------##

message("Develop Setting...")
### Application name
set( MANDIBLE_SEGMENTATION_EXECUTABLE "mandibleSegmentation_Develop" )



### Application source/header/ui files
set( MANDIBLE_SEGMENTATION_HEADER_FILES rendering3D.cpp )
set( MANDIBLE_SEGMENTATION_SOURCE_FILES  )



### Generate the executable
add_executable( ${MANDIBLE_SEGMENTATION_EXECUTABLE}
    ${MANDIBLE_SEGMENTATION_SOURCE_FILES}
    ${MANDIBLE_SEGMENTATION_HEADER_FILES}
)


### Link the application against the oclMGPUVR library
target_link_libraries( ${MANDIBLE_SEGMENTATION_EXECUTABLE} ${MANDIBLE_SEGMENTATION_LIBRARY} )

### Make sure to use the required Qt modules
qt5_use_modules( ${MANDIBLE_SEGMENTATION_EXECUTABLE} Core Widgets OpenGL Gui )

# Install the executable (generate with 'make install' in the bin directory)
install( TARGETS ${MANDIBLE_SEGMENTATION_EXECUTABLE} DESTINATION bin )
