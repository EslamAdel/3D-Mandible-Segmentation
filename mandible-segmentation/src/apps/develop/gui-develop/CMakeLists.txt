## -------------------------- GUI DEVELOP SETTING----------------------------------##

message("GUI Develop Setting...")

################################ Segmenter
message("Segmenter ...")
set( SEGMENTER_EXECUTABLE "segmenter_Develop" )



### Application source/header/ui files
set( SEGMENTER_SOURCE_FILES main.cpp SimpleView.cpp)
set( SEGMENTER_HEADER_FILES SimpleView.h)
set( SEGMENTER_APP_FORMS  SimpleView.ui)
set( SEGMENTER_RESOURCES Icons/icons.qrc )

qt5_wrap_ui( SEGMENTER_APP_FORMS_MOC ${SEGMENTER_APP_FORMS})
qt5_add_resources(SEGMENTER_APP_RESOURCES ${SEGMENTER_RESOURCES} )

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
### Generate the executable
add_executable( ${SEGMENTER_EXECUTABLE}
    ${SEGMENTER_SOURCE_FILES}
    ${SEGMENTER_HEADER_FILES}
    ${SEGMENTER_APP_FORMS}
    ${SEGMENTER_APP_FORMS_MOC}
    ${SEGMENTER_APP_RESOURCES}
)


### Link the application against the oclMGPUVR library
target_link_libraries( ${SEGMENTER_EXECUTABLE} ${MANDIBLE_SEGMENTATION_LIBRARY} )

### Make sure to use the required Qt modules
qt5_use_modules( ${SEGMENTER_EXECUTABLE} Core Widgets OpenGL Gui )


################################### install
# Install the executable (generate with 'make install' in the bin directory)
install( TARGETS ${MANDIBLE_SEGMENTATION_EXECUTABLE} DESTINATION bin )


