#pragma once

#include <glm/mat4x4.hpp> 
#include <glm/vec3.hpp> 

class ObjectTransform {
public:
	ObjectTransform() {}

	void SetScale(float scale);
	void SetRotation(float x, float y, float z);
	void SetPosition(float x, float y, float z);

	void Rotate(float x, float y, float z);

	glm::mat4 GetMatrix();

private:
	float     scale = 1.0f;
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
};

