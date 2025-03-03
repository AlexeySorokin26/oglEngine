#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "General/utility.h"

#include "Rendering/ShaderProgram.h"

// Размеры окна
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

const char* pVSFileName = "C:\\Users\\sorok\\Desktop\\OGLDEV\\src\\shaders\\shader.vs";
const char* pFSFileName = "C:\\Users\\sorok\\Desktop\\OGLDEV\\src\\shaders\\shader.fs";

GLuint VBO, IBO, VAO, gWorldLocation;

ShaderProgram* shaderProgram;

// Вершина с позицией и цветом
struct Vertex {
	glm::vec3 position;
	glm::vec3 color;

	Vertex(float x, float y, float z, float r, float g, float b)
		: position(x, y, z), color(r, g, b) {
	}
};

// Вершины куба
std::vector<Vertex> vertices = {
	Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f), // 0
	Vertex(0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f),  // 1
	Vertex(0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f),  // 2
	Vertex(-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f), // 3
	Vertex(-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f), // 4
	Vertex(0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f),  // 5
	Vertex(0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f),  // 6
	Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.0f)  // 7
};

// Индексы для куба
std::vector<unsigned int> indices = {
	0, 1, 2, 2, 3, 0, // Задняя грань
	4, 5, 6, 6, 7, 4, // Передняя грань
	3, 2, 6, 6, 7, 3, // Верхняя грань
	0, 1, 5, 5, 4, 0, // Нижняя грань
	0, 3, 7, 7, 4, 0, // Левая грань
	1, 2, 6, 6, 5, 1  // Правая грань
};

// Создание VBO и IBO
void CreateBuffers() {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

// Рендеринг сцены
void RenderSceneCB() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static float angle = 0.0f;
	angle += 0.01f;

	glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 gWorld = projection * view * model;

	shaderProgram->SetMatrix4("gWorld", gWorld);

	// tell gpu how to use how to bind actually allocated and filled gpu memory
	// with shader code
	// just activate location = 0
	// like glBindBuffer activate vbo
	// position
	// explain for gpu how we allocated memory
	// 6 * sizeof(float) is a stride 
	// in out case pos + color = 6 -> jump or stride 
	// and here we take ONLY POSs
	// so we provide first position and jump 
	// start pos is the last parameter and it's 0 
	// because pos starts from 0 -> start pos 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}

// Главная функция
int main(int argc, char** argv) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Rotating Cube");

	glewInit();

	glEnable(GL_DEPTH_TEST);

	shaderProgram = new ShaderProgram(pVSFileName, pFSFileName);
	shaderProgram->Bind();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	CreateBuffers();

	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);

	glutMainLoop();

	return 0;
}
