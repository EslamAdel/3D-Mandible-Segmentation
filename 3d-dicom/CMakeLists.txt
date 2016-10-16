cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

PROJECT (Medical)

find_package(VTK REQUIRED) 

include(${VTK_USE_FILE})

add_executable(Medical1 MACOSX_BUNDLE Medical1.cxx)

target_link_libraries(Medical1 ${VTK_LIBRARIES})
