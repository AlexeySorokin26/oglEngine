#pragma once

#include <glm/mat4x4.hpp>
#include <GL/glew.h>

#include <string>

class ShaderProgram {
public:
	ShaderProgram(const char* vertexFile, const char* fragmentFile);
	~ShaderProgram();
	void Bind() const;
	void Unbind();
	bool IsCompiled() const { return isCompiled; }
	void SetMatrix4(const char* name, const glm::mat4& matrix) const;

	// �������� �� ������ �������
	void CheckShaderCompilation(GLuint shader, const std::string& type);

	GLint GetUniformLocation(const char* name) const;
private:
	bool isCompiled = false;
	unsigned int id = 0;
};