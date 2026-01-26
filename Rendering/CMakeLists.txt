set(SRC 
	ShaderProgram.cpp
	Camera.cpp
	ObjectTransform.cpp
)

set(HEADERS 
	ShaderProgram.h
	Camera.h
	ObjectTransform.h
)

add_library(Rendering STATIC ${SRC} ${HEADERS})

target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Rendering PUBLIC ${CMAKE_SOURCE_DIR}/General)

target_include_directories(Rendering PRIVATE 
	${CMAKE_SOURCE_DIR}/external/glew/include
    ${CMAKE_SOURCE_DIR}/external/freeglut/include
	${CMAKE_SOURCE_DIR}/external/glm
)
target_link_directories(Rendering PRIVATE 
	${CMAKE_SOURCE_DIR}/external/glew/lib
	${CMAKE_SOURCE_DIR}/external/glm
)

target_link_libraries(Rendering PUBLIC General)
target_link_libraries(Rendering PRIVATE libglew_static)
target_link_libraries(Rendering PRIVATE freeglut_static)

