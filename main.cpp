#include <cmath>

#include "General/mymath.h"
#include "Rendering/ShaderProgram.h"
#include "Rendering/Camera.h"
#include "Rendering/ObjectTransform.h"
#include "Rendering/Projection.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp>

GLuint VBO;
GLuint IBO;
ShaderProgram* sp;

Camera camera;

ObjectTransform objTransform;

float fov = 90.f;
float zNear = 1.f;
float zFar = 10.f;
int w = 1920;
int h = 1080;
PerspectiveProjection perspectiveProjection;

void RenderSceneCB() {
	glClear(GL_COLOR_BUFFER_BIT);
	static float angleInDegrees = .04f;
	objTransform.SetPosition(0, 0, 4);
	objTransform.Rotate(0, angleInDegrees, 0);

	glm::mat4 finalMatrix = perspectiveProjection.GetMatrix() * camera.GetMatrix() * objTransform.GetMatrix(); 

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

static void SpecialKeyboardCB(int Key, int x, int y)
{
	camera.OnKeyboard(Key);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
	glutSpecialFunc(SpecialKeyboardCB);
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

	InitializeGlutCallbacks(); // call this callback func if we need to redraw the window

	perspectiveProjection = PerspectiveProjection(fov, w, h, zNear, zFar);

	glutMainLoop(); // run infinite loop to handle events 

	return 0;
}
