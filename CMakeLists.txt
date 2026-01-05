cmake_minimum_required(VERSION 4.0.0 FATAL_ERROR)

set(PROJECT_NAME OGLEngine)
project(${PROJECT_NAME})

add_subdirectory(external/freeglut)
add_subdirectory(external/glew)
add_subdirectory(external/glm)
add_subdirectory(General)
add_subdirectory(Rendering)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE freeglut_static libglew_static)
target_link_directories(${PROJECT_NAME} PRIVATE external/glew/lib) # help for linker to find custom dirs with libs
target_include_directories(${PROJECT_NAME} PRIVATE 
    external/freeglut/include
    external/glew/include
    external/glm
)
target_link_libraries(${PROJECT_NAME} PUBLIC General)
target_link_libraries(${PROJECT_NAME} PRIVATE Rendering)

# we get here all targets from freeglut
get_property(
    FREEGLUT_TARGETS             # var to store result
    DIRECTORY                    # type of object with which we currently working (directory)
    external/freeglut            # where to work
    PROPERTY BUILDSYSTEM_TARGETS # which property we want to get. BUILDSYSTEM_TARGETS contains a list of all targets in specified dir
)
foreach(target IN LISTS FREEGLUT_TARGETS)
    set_property(TARGET ${target} PROPERTY FOLDER "FreeGLUT")
endforeach()

set_property(TARGET libglew_shared PROPERTY FOLDER "glew")
set_property(TARGET libglew_static PROPERTY FOLDER "glew")
