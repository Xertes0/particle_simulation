# glad permalink
# download the zip file and extract it in this directory
# https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
	message(FATAL_ERROR "Cannot find glad in its directory. See glad/CMakeLists.txt for details.")
endif()

add_library(glad)
target_sources(glad PRIVATE ./src/glad.c)
target_include_directories(glad PUBLIC ./include)
target_link_libraries(glad)
