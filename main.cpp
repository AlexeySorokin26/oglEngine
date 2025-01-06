#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "main.h"

GLuint VBO;
GLuint IBO;
GLint glWorldLocation;

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;

	Vertex() {}

	Vertex(float x, float y) {
		pos = glm::vec3(x, y, 0.0f);

		float r = (float)rand() / (float)RAND_MAX;
		float g = (float)rand() / (float)RAND_MAX;
		float b = (float)rand() / (float)RAND_MAX;
		color = glm::vec3(r, g, b);
	}
};

void World()
{
	static float scale = 0.1f;
	//scale += 0.01f;

	glm::mat4x4 world = glm::mat4x4(
		cosf(scale), -sinf(scale), 0.0f, 0.0f,
		sinf(scale), cosf(scale), 0.0f, 0.0f,
		0.0, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	glUniformMatrix4fv(
		glWorldLocation, 1, GL_TRUE, glm::value_ptr(world));
}

void RenderSceneCB() {
	//glEnable(GL_CULL_FACE);

	glClear(GL_COLOR_BUFFER_BIT);

	World();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// tell gpu how to use how to bind actually allocated and filled gpu memory
	// with shader code
	// just activate location = 0
	// like glBindBuffer activate vbo
	// position
	glEnableVertexAttribArray(0);
	// explain for gpu how we allocated memory
	// 6 * sizeof(float) is a stride 
	// in out case pos + color = 6 -> jump or stride 
	// and here we take ONLY POSs
	// so we provide first position and jump 
	// start pos is the last parameter and it's 0 
	// because pos starts from 0 -> start pos 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	glEnableVertexAttribArray(1);
	// start pos is the last parameter and it's 3 floats elements 
	// because pos starts from 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
		(void*)(3 * sizeof(float)));

	glDrawElements(GL_TRIANGLES, 54, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutPostRedisplay();

	glutSwapBuffers();
}


void CreateVertexBuffer() {

	Vertex vertices[19];

	// Center
	vertices[0] = Vertex(0.0f, 0.0);

	// Top row
	vertices[1] = Vertex(-1.0f, 1.0f);
	vertices[2] = Vertex(-0.75f, 1.0f);
	vertices[3] = Vertex(-0.50f, 1.0f);
	vertices[4] = Vertex(-0.25f, 1.0f);
	vertices[5] = Vertex(-0.0f, 1.0f);
	vertices[6] = Vertex(0.25f, 1.0f);
	vertices[7] = Vertex(0.50f, 1.0f);
	vertices[8] = Vertex(0.75f, 1.0f);
	vertices[9] = Vertex(1.0f, 1.0f);

	// Bottom row
	vertices[10] = Vertex(-1.0f, -1.0f);
	vertices[11] = Vertex(-0.75f, -1.0f);
	vertices[12] = Vertex(-0.50f, -1.0f);
	vertices[13] = Vertex(-0.25f, -1.0f);
	vertices[14] = Vertex(-0.0f, -1.0f);
	vertices[15] = Vertex(0.25f, -1.0f);
	vertices[16] = Vertex(0.50f, -1.0f);
	vertices[17] = Vertex(0.75f, -1.0f);
	vertices[18] = Vertex(1.0f, -1.0f);

	glGenBuffers(1, &VBO);
	// make it current
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// allocate memory on gpu and transfer from cpu
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void CreateIndexBuffer() {
	unsigned int indices[] = { // Top triangles
							   0, 2, 1,
							   0, 3, 2,
							   0, 4, 3,
							   0, 5, 4,
							   0, 6, 5,
							   0, 7, 6,
							   0, 8, 7,
							   0, 9, 8,

							   // Bottom triangles
							   0, 10, 11,
							   0, 11, 12,
							   0, 12, 13,
							   0, 13, 14,
							   0, 14, 15,
							   0, 15, 16,
							   0, 16, 17,
							   0, 17, 18,

							   // Left triangle
							   0, 1, 10,

							   // Right triangle
							   0, 18, 9 };

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

bool ReadFile(const char* pFileName, std::string& outFile) {
	std::ifstream f(pFileName);
	bool ret = false;
	if (f.is_open()) {
		std::string line;
		while (getline(f, line)) {
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		ret = true;
	}
	else {
		throw std::runtime_error("file not opened: " + std::string(pFileName));
	}
	return ret;
}

void AddShader(GLuint shaderProgram, const char* pShaderText, GLenum shaderType) {
	GLuint shaderObj = glCreateShader(shaderType);
	if (shaderObj == 0) {
		std::cout << "error creating shader type" + shaderType << "\n";
		exit(1);
	}

	GLint lenghts[1];
	lenghts[0] = strlen(pShaderText);

	glShaderSource(shaderObj, 1, &pShaderText, lenghts);
	glCompileShader(shaderObj);

	GLint success;
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderObj, 1024, NULL, infoLog);
		std::cout << "error compiling shader type" + shaderType << "\n";
		exit(1);
	}

	glAttachShader(shaderProgram, shaderObj);
}

const char* pVSFileName = "C:\\Users\\sorok\\Desktop\\OGLDEV\\src\\shaders\\shader.vs";
const char* pFSFileName = "C:\\Users\\sorok\\Desktop\\OGLDEV\\src\\shaders\\shader.fs";

void CompileShaders() {
	GLuint shaderProgram = glCreateProgram();
	if (shaderProgram == 0) {
		std::cout << "error creating shader program\n";
		exit(1);
	}

	std::string vs, fs;

	if (!ReadFile(pVSFileName, vs)) {
		std::cout << "error reading vs\n";
		exit(1);
	}

	AddShader(shaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	if (!ReadFile(pFSFileName, fs)) {
		std::cout << "error reading fs\n";
		exit(1);
	}

	AddShader(shaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint success = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (success == 0) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		std::cout << "error linking" + std::string(errorLog) + "\n";
		exit(1);
	}

	glWorldLocation = glGetUniformLocation(shaderProgram, "gWorld");
	if (glWorldLocation == -1) {
		std::cout << "error getting unifrom loc of glWorldLocation\n";
		exit(1);
	}

	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
		std::cout << "Invalid shader program: " + std::string(errorLog) + "\n";
		exit(1);
	}

	glUseProgram(shaderProgram);
}

// Entry point
int main(int argc, char** argv) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Modern OpenGL with GLEW and FreeGLUT");


	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout << "glew failed " << glewGetErrorString(res) << std::endl;
		return 1;
	}

	GLclampf Red = 1.0f, Green = 1.0f, Blue = 0.0f, Alpha = 0.0f;
	glClearColor(Red, Green, Blue, Alpha);

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders();

	glutDisplayFunc(RenderSceneCB);

	glutMainLoop();


	return 0;
}
