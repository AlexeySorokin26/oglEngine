#include "Camera.h"
#include <GL/freeglut.h>


Camera::Camera() :
	pos(0.0f, 0.0f, 0.0f),
	target(0.0f, 0.0f, 1.0f),
	up(0.0f, 1.0f, 0.0f) {
}


void Camera::SetPosition(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}


void Camera::OnKeyboard(unsigned char Key) {
	switch (Key) {

	case GLUT_KEY_UP:
		pos += (target * speed);
		break;

	case GLUT_KEY_DOWN:
		pos -= (target * speed);
		break;

	case GLUT_KEY_LEFT:
	{
		glm::vec3 Left = glm::normalize(glm::cross(target, up));
		Left *= speed;
		pos += Left;
	}
	break;

	case GLUT_KEY_RIGHT:
	{
		glm::vec3 Right = glm::normalize(glm::cross(up, target));
		Right *= speed;
		pos += Right;
	}
	break;

	case GLUT_KEY_PAGE_UP:
		pos.y += speed;
		break;

	case GLUT_KEY_PAGE_DOWN:
		pos.y -= speed;
		break;

	case '+':
		speed += 0.1f;
		break;

	case '-':
		speed -= 0.1f;
		if (speed < 0.1f) {
			speed = 0.1f;
		}
		break;
	}
}

glm::mat4 Camera::InitCameraMatrix(const glm::vec3& target, const glm::vec3& up) {
	glm::vec3 n = glm::normalize(target);
	glm::vec3 upNorm = glm::normalize(up);
	glm::vec3 u = glm::normalize(glm::cross(upNorm, n));
	glm::vec3 v = glm::normalize(glm::cross(n, u));

	glm::mat4 camMat(
		u.x, v.x, n.x, 0.0f,
		u.y, v.y, n.y, 0.0f,
		u.z, v.z, n.z, 0.0f,
		-pos.x, -pos.y, -pos.z, 1.0f
	);

	return camMat;
}

glm::mat4 Camera::GetMatrix() {
	glm::mat4 CameraTransformation = InitCameraMatrix(target, up);
	return CameraTransformation;
}