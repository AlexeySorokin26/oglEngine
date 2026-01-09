#include "ShaderProgram.h"

#include "utility.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode, fragmentCode;
	if (!ReadFile(vertexFile, vertexCode)) {
		throw std::runtime_error("Failed to read vertex shader file");
	}
	if (!ReadFile(fragmentFile, fragmentCode)) {
		throw std::runtime_error("Failed to read fragment shader file");
	}

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	CheckShaderCompilation(vertexShader, "Vertex Shader");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	CheckShaderCompilation(fragmentShader, "Fragment Shader");

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	GLint success;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, nullptr);
		throw std::runtime_error("Shader Program linking failed!");
	}

	glDetachShader(id, vertexShader);
	glDetachShader(id, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void ShaderProgram::Bind() const
{
	glUseProgram(id); // make shader current
}

void ShaderProgram::Unbind()
{
	glUseProgram(0); // means 0 shader 
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(id);
}

void ShaderProgram::CheckShaderCompilation(GLuint shader, const std::string& type) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		throw std::runtime_error(type + " compilation failed: " + infoLog);
	}
}

void ShaderProgram::SetMatrix4(const char* name, const glm::mat4& matrix, const bool rowMajor) const {
	// get location by name, amount of args, transpose or not, pointer to data specially for glm such way
	// glm::mat4 is column major 
	// 1 2 3 4
	// 5 6 7 8 
	// so it goes 1 -> 5 -> 2 -> 6
	// false here means column major
	glUniformMatrix4fv(GetUniformLocation(name), 1, rowMajor, glm::value_ptr(matrix));
}

void ShaderProgram::SetFloat(const char* name, float v) const {
	glUniform1f(GetUniformLocation(name), v);
}

GLint ShaderProgram::GetUniformLocation(const char* name) const {
	GLint location = glGetUniformLocation(id, name);
	if (location == -1) {
		throw("Error getting uniform location for " + std::string(name) + "\n");
	}
	return location;
}