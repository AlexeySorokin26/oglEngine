set(SRC 
	ShaderProgram.cpp
)

set(HEADERS 
	ShaderProgram.h
)

# ��������� ����������� ���������� Rendering
add_library(Rendering STATIC ${SRC} ${HEADERS})

# ��������� ���������� � ������������� ������� ��� ���������� Rendering
target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Rendering PUBLIC ${CMAKE_SOURCE_DIR}/General)

# ���������� GLEW: ��������� ���� � ������������ ������ � �����������
target_include_directories(Rendering PRIVATE 
	${CMAKE_SOURCE_DIR}/external/glew/include
	${CMAKE_SOURCE_DIR}/external/glm
)
target_link_directories(Rendering PRIVATE 
	${CMAKE_SOURCE_DIR}/external/glew/lib
	${CMAKE_SOURCE_DIR}/external/glm
)

# ��������� � ����������� General
target_link_libraries(Rendering PUBLIC General)

# ��������� ����������� �� GLEW
target_link_libraries(Rendering PRIVATE libglew_static)

