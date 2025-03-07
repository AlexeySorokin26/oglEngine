cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(PROJECT_NAME OGLEngine)
project(${PROJECT_NAME})

# Add FreeGLUT as a subdirectory
add_subdirectory(external/freeglut)
# Add GLEW as a subdirectory
add_subdirectory(external/glew)
# glm 
add_subdirectory(external/glm)
# General lib
add_subdirectory(General)
# Rendering
add_subdirectory(Rendering)

add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE freeglut_static libglew_static)
target_link_directories(${PROJECT_NAME} PRIVATE external/glew/lib)
target_include_directories(${PROJECT_NAME} PRIVATE 
    external/freeglut/include
    external/glew/include
    external/glm
)
target_link_libraries(${PROJECT_NAME} PUBLIC General)
target_link_libraries(${PROJECT_NAME} PRIVATE Rendering)

# Idea is to move them into one folder 
# Retrieve the list of all targets created in the external/freeglut directory
get_property(FREEGLUT_TARGETS DIRECTORY external/freeglut PROPERTY BUILDSYSTEM_TARGETS)
# Apply the FOLDER property to each target dynamically
foreach(target IN LISTS FREEGLUT_TARGETS)
    set_property(TARGET ${target} PROPERTY FOLDER "FreeGLUT")
endforeach()

set_property(TARGET libglew_shared PROPERTY FOLDER "glew")
set_property(TARGET libglew_static PROPERTY FOLDER "glew")
