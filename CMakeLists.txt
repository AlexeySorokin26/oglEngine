cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

set(PROJECT_NAME OGLEngine)
project(${PROJECT_NAME})

# Add FreeGLUT as a subdirectory
add_subdirectory(external/freeglut)
# Add GLEW as a subdirectory
add_subdirectory(external/glew)

add_executable(${PROJECT_NAME} main.cpp)


target_link_libraries(${PROJECT_NAME} PRIVATE freeglut_static glew)
target_include_directories(${PROJECT_NAME} PRIVATE 
    external/freeglut/include
)

# Idea is to move them into one folder 
# Retrieve the list of all targets created in the external/freeglut directory
get_property(FREEGLUT_TARGETS DIRECTORY external/freeglut PROPERTY BUILDSYSTEM_TARGETS)
# Apply the FOLDER property to each target dynamically
foreach(target IN LISTS FREEGLUT_TARGETS)
    set_property(TARGET ${target} PROPERTY FOLDER "FreeGLUT")
endforeach()
