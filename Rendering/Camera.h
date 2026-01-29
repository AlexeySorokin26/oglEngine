#pragma once

#include <glm/mat4x4.hpp> 
#include <glm/vec3.hpp> 

class Camera {
public:
	Camera(int w, int h);
	Camera(int w, int h, const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

	void SetPosition(float x, float y, float z);

	void OnKeyboard(int key);
	void OnKeyboard(unsigned char key);
	void OnMouse(int x, int y);
	void OnRender();

	glm::mat4 GetMatrix();

private:
	void Init();
	glm::mat4 ComputeCamMatrix();
	void Update();

private:
	glm::vec3 pos;
	glm::vec3 target;
	glm::vec3 up;
	float speed = 1.0f;

	int windowW;
	int windowH;

	float angleH;
	float angleV;

	bool onUpperEdge;
	bool onLowerEdge;
	bool onLeftEdge;
	bool onRightEdge;

	glm::vec2 mousePos;
};
