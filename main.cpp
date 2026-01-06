#include <cmath>

#include "General/mymath.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>


GLuint VBO;

void RenderSceneCB1() {
	glClear(GL_COLOR_BUFFER_BIT);  // actually clear it color buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // activate our vbo (tells gpu we are going to use it)
	glEnableVertexAttribArray(0);       // open gate for our data 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // tell gpu how to interpritate data. Stride is a number of elements to the next element in buffer. Offset here is a number of elements from the begginng to our element.
	glDrawArrays(GL_TRIANGLES, 0, 3); // finaly tell gpu to render using our VBO. It is massive sets of operation. Drivers tells gpu start reading from the bounded buffer	and pass data through the pipeline 
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void CreateVertexBuffer() {
	Vector3f vertices[3];
	vertices[0] = Vector3f(-1.0f, -1.0f, 0.0f);
	vertices[1] = Vector3f(0.0f, 1.0f, 0.0f);
	vertices[2] = Vector3f(1.0f, -1.0f, 0.0f);

	// Create a handle for VBO. Which provides access to the gpu memory
	glGenBuffers(1, &VBO);
	// Assosiate our created vbo with specific buffer 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Load our vertices in VBO. Note what we don't use VBO here since we bind it before 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	int w = 1920;
	int h = 1080;
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

	CreateVertexBuffer();

	glutDisplayFunc(RenderSceneCB1); // call this callback func if we need to redraw the window

	glutMainLoop(); // run infinite loop to handle events 

	return 0;
}
