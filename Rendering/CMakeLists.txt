set(SRC 
	ShaderProgram.cpp
)

set(HEADERS 
	ShaderProgram.h
)

# Добавляем статическую библиотеку Rendering
add_library(Rendering STATIC ${SRC} ${HEADERS})

# Указываем директории с заголовочными файлами для библиотеки Rendering
target_include_directories(Rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Rendering PUBLIC ${CMAKE_SOURCE_DIR}/General)

# Подключаем GLEW: добавляем пути к заголовочным файлам и библиотекам
target_include_directories(Rendering PRIVATE 
	${CMAKE_SOURCE_DIR}/external/glew/include
	${CMAKE_SOURCE_DIR}/external/glm
)
target_link_directories(Rendering PRIVATE 
	${CMAKE_SOURCE_DIR}/external/glew/lib
	${CMAKE_SOURCE_DIR}/external/glm
)

# Связываем с библиотекой General
target_link_libraries(Rendering PUBLIC General)

# Добавляем зависимость от GLEW
target_link_libraries(Rendering PRIVATE libglew_static)

