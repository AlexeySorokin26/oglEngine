#include <cmath>

#include "General/mymath.h"
#include "Rendering/ShaderProgram.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

GLuint VBO;
GLuint IBO;
ShaderProgram* sp;

int w = 1920;
int h = 1080;

void RenderSceneCB1() {
	glClear(GL_COLOR_BUFFER_BIT);
	static float angleInDegrees = 45.0f;
	angleInDegrees += 0.05;
	if (angleInDegrees >= 360.0f) {
		angleInDegrees -= 360.0f;
	}
	// glm uses column major 
	// so we need to enter stuff correctly
	glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), glm::vec3(0, 0, 1)); // c
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(angleInDegrees), glm::vec3(1, 0, 0)); // c

	glm::mat4 translation = glm::translate(glm::mat4(1.0f),  // c
		glm::vec3(0, 0, 5));

	glm::vec3 camPos(5.f, 0.f, 0.f);
	glm::vec3 u(1.f, 0.f, 0.f);
	glm::vec3 v(0.f, 1.f, 0.f);
	glm::vec3 n(0.f, 0.f, 1.f);

	glm::mat4 camMat(
		u.x, v.x, n.x, 0.0f,
		u.y, v.y, n.y, 0.0f,
		u.z, v.z, n.z, 0.0f,
		-camPos.x, -camPos.y, -camPos.z, 1.0f
	);

	float fov = 90.f;
	float tanHalfFov = tanf(glm::radians(fov / 2.0f));
	float f = 1.0f / tanHalfFov;
	float ar = (float)w / (float)h;
	float nearZ = 1.f;
	float farZ = 10.f;
	float zRange = nearZ - farZ;
	float a = (-farZ - nearZ) / zRange;
	float b = 2.f * farZ * nearZ / zRange;

	glm::mat4 projection = glm::mat4( // c // it's transposed because of column major which uses glm
		f / ar, 0, 0, 0,
		0, f, 0, 0,
		0, 0, a, 1,
		0, 0, b, 0
	);

	glm::mat4 finalMatrix = projection * camMat * translation * rotationX * rotationZ; // c *

	sp->SetMatrix4("finalMatrix", finalMatrix, false);
	sp->Bind();
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Assosiate our created vbo with specific buffer (array buffer in this case)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glEnableVertexAttribArray(0);       // open gate for our data 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // tell gpu how to interpritate data. Stride is a number of elements to the next element in buffer. Offset here is a number of elements from the begginng to our element.
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);   // finaly tell gpu to render using our VBO. It is massive sets of operation. Drivers tells gpu start reading from the bounded buffer	and pass data through the pipeline 
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void CreateVertexBuffer() {
	Vector3f vertices[8];
	vertices[0] = Vector3f(0.5f, 0.5f, 0.5f);      // 0
	vertices[1] = Vector3f(-0.5f, 0.5f, -0.5f);    // 1  
	vertices[2] = Vector3f(-0.5f, 0.5f, 0.5f);     // 2
	vertices[3] = Vector3f(0.5f, -0.5f, -0.5f);    // 3
	vertices[4] = Vector3f(-0.5f, -0.5f, -0.5f);   // 4
	vertices[5] = Vector3f(0.5f, 0.5f, -0.5f);     // 5
	vertices[6] = Vector3f(0.5f, -0.5f, 0.5f);     // 6
	vertices[7] = Vector3f(-0.5f, -0.5f, 0.5f);    // 7

	// Create a handle for VBO. Which provides access to the gpu memory
	glGenBuffers(1, &VBO);
	// Assosiate our created vbo with specific buffer (array buffer in this case)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Load our vertices in VBO. VBO now associates with this data. Note what we don't use VBO here since we bind it before 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void CreateIndexBuffer() {
	std::vector<unsigned int> indices{
		0, 1, 2,
		1, 3, 4,
		5, 6, 3,
		7, 3, 6,
		2, 4, 7,
		0, 7, 6,
		0, 5, 1,
		1, 5, 3,
		5, 0, 6,
		7, 4, 3,
		2, 1, 4,
		0, 2, 7
	};


	// Create a handle for IBO. Which provides access to the gpu memory
	glGenBuffers(1, &IBO);
	// Assosiate our created IBO with specific buffer (array buffer in this case)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// Load our vertices in IBO. IBO now associates with this data. Note what we don't use IBO here since we bind it before 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(w, h);

	int x = 0;
	int y = 0;
	glutInitWindowPosition(x, y);

	int win = glutCreateWindow("Tutorial 1");

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout << "glew init failed " << glewGetErrorString(res) << std::endl;
		return 1;
	}

	GLclampf red = 0.0f, green = 0.0f, blue = 1.0f, alpha = 0.0f; // floats
	glClearColor(red, green, blue, alpha);

	glEnable(GL_CULL_FACE); // Enable cull facing 
	glFrontFace(GL_CW);  // Front face triangles are Clock wise 
	glCullFace(GL_BACK); // Cull back face triangles

	CreateVertexBuffer();
	CreateIndexBuffer();

	std::string vp = "C:\\Users\\AlexeySorokin\\Desktop\\oglEngine\\shaders\\shader.vs";
	std::string fp = "C:\\Users\\AlexeySorokin\\Desktop\\oglEngine\\shaders\\shader.fs";
	sp = new ShaderProgram(vp.c_str(), fp.c_str());

	glutDisplayFunc(RenderSceneCB1); // call this callback func if we need to redraw the window

	glutMainLoop(); // run infinite loop to handle events 

	return 0;
}
