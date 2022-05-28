find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(main)
target_include_directories(main PRIVATE ./include)
file(GLOB_RECURSE SOURCES ./src/*.c)
target_sources(main PRIVATE ${SOURCES})
target_link_libraries(main glad glfw OpenGL::GL ${CMAKE_DL_LIBS})
