#include "ObjectTransform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

void ObjectTransform::SetScale(float scale)
{
	scale = scale;
}


void ObjectTransform::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}


void ObjectTransform::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}


void ObjectTransform::Rotate(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}


glm::mat4 ObjectTransform::GetMatrix()
{
	glm::mat4 scaleMat = glm::scale(glm::mat4(1.f), glm::vec3(scale));

	glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0, 0, 1));
	glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0, 1, 0));
	glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1, 0, 0));

	glm::mat4 rotationMat = rotationZ * rotationY * rotationX;

	glm::mat4 translationMat = glm::translate(glm::mat4(1.0f),
		glm::vec3(pos.x, pos.y, pos.z));

	glm::mat4 worldTransformation = translationMat * rotationMat * scaleMat;

	return worldTransformation;
}