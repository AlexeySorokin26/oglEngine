#pragma once

#include <glm/mat4x4.hpp> 
#include <glm/vec3.hpp> 

class Camera {
public:
	Camera();

	void SetPosition(float x, float y, float z);

	void OnKeyboard(int key);
	void OnKeyboard(unsigned char key);

	glm::mat4 GetMatrix();

private:
	glm::mat4 InitCameraMatrix(const glm::vec3& target, const glm::vec3& up);

private:
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
	float speed = 1.0f;
};
