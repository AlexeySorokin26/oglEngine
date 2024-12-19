#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

GLuint VBO;

void RenderSceneCB() {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, 0, 1);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void CreateVertexBuffer() {

	float v[3] = { 0.5f, 0.1f, 0.0f };

	glGenBuffers(1, &VBO); // allocate buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
}

// Entry point
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Modern OpenGL with GLEW and FreeGLUT");


	GLenum res = glewInit();
	if (res != GLEW_OK) {
		std::cout << "glew failed " << glewGetErrorString(res) << std::endl;
		return 1;
	}

	GLclampf Red = 0.0f, Green = 0.0f, Blue = 0.0f, Alpha = 0.0f;
	glClearColor(Red, Green, Blue, Alpha);

	CreateVertexBuffer();
	glutDisplayFunc(RenderSceneCB);
	glutMainLoop();

	//if (!initializeGLEW()) {
	//	return -1;
	//}

	//initializeGL(); // Initialize OpenGL settings
	//initializeShaders(); // Initialize shaders
	//initializeBuffers(); // Initialize VAO and VBO

	//glutDisplayFunc(renderScene);
	//glutIdleFunc(update);

	//glutMainLoop();
	return 0;
}
