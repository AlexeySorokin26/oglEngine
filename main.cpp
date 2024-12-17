#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

// Shader program
GLuint shaderProgram;
GLuint VAO, VBO;

// Rotation angle
float angle = 0.0f;

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec3 position;
    uniform mat4 model;
    void main() {
        gl_Position = model * vec4(position, 1.0);
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red color
    }
)";

// Function to check shader compilation errors
void checkShaderCompileError(GLuint shader) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

// Function to check program linking errors
void checkProgramLinkError(GLuint program) {
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

// Function to compile shaders and link them into a program
void initializeShaders() {
	// Compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkShaderCompileError(vertexShader);

	// Compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	checkShaderCompileError(fragmentShader);

	// Link shaders into a program
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkProgramLinkError(shaderProgram);

	// Clean up shaders (they're linked into the program now)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Function to initialize the OpenGL buffers (VAO, VBO)
void initializeBuffers() {
	// Define the vertices for a triangle
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,  // Bottom left
		 0.5f, -0.5f, 0.0f,  // Bottom right
		 0.0f,  0.5f, 0.0f   // Top
	};

	// Generate a VAO and VBO for the triangle
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Specify the vertex attribute layout (position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
	glBindVertexArray(0); // Unbind the VAO
}

// Function to render the scene
void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	// Create the model matrix for rotation
	GLfloat model[16] = {
		cos(angle),  0.0f, sin(angle), 0.0f,
		0.0f,        1.0f, 0.0f,        0.0f,
		-sin(angle), 0.0f, cos(angle),  0.0f,
		0.0f,        0.0f, 0.0f,        1.0f
	};

	// Set the model matrix uniform in the shader
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_TRUE, model);

	// Draw the triangle
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glutSwapBuffers(); // Swap buffers
}

// Function to update the rotation angle
void update() {
	angle += 0.01f;
	if (angle > 360.0f) angle -= 360.0f;
	glutPostRedisplay(); // Request redraw
}

// Function to initialize OpenGL
void initializeGL() {
	glEnable(GL_DEPTH_TEST); // Enable depth testing
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set the background color to dark gray
}

// Function to initialize GLEW
bool initializeGLEW() {
	GLenum glewInitResult = glewInit();
	if (glewInitResult != GLEW_OK) {
		std::cerr << "Error initializing GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
		return false;
	}
	std::cout << "GLEW Initialized Successfully!" << std::endl;
	return true;
}

// Entry point
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Modern OpenGL with GLEW and FreeGLUT");

	if (!initializeGLEW()) {
		return -1;
	}

	initializeGL(); // Initialize OpenGL settings
	initializeShaders(); // Initialize shaders
	initializeBuffers(); // Initialize VAO and VBO

	glutDisplayFunc(renderScene);
	glutIdleFunc(update);

	glutMainLoop();
	return 0;
}
