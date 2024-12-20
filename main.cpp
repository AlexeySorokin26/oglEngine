#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

GLuint VBO;

void RenderSceneCB() {
	glEnable(GL_CULL_FACE);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void CreateVertexBuffer() {

	float v[9] = {
		 1.0f, -1.0f, 0.0f,  // bottom right 
		 0.0f, 1.0f, 0.0f,  // top
		-1.0f, -1.0f, 0.0f // bottom left 
	};

	glGenBuffers(1, &VBO); // allocate buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
	CompileShaders();
	glutDisplayFunc(RenderSceneCB);
	glutMainLoop();


	return 0;
}
