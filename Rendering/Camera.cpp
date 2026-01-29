#include "Camera.h"
#include "Quaternion.h"

#include <GL/freeglut.h>
#include <glm/trigonometric.hpp>

static int MARGIN = 10;
static float EDGE_STEP = 1.0f;

Camera::Camera(int w, int h)
	: windowW(w), windowH(h)
{
	Init();
}

Camera::Camera(int w, int h, const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up)
	: windowW(w), windowH(h),
	pos(pos),
	target(glm::normalize(target)),
	up(glm::normalize(up)),
	speed(1.0f),
	onUpperEdge(false),
	onLowerEdge(false),
	onLeftEdge(false),
	onRightEdge(false) {
	Init();
}

void Camera::SetPosition(float x, float y, float z) {
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void Camera::OnKeyboard(unsigned char key) {
	switch (key) {
	case 'w':
	case 'W':
		pos += (up * speed);
		break;
	case 's':
	case 'S':
		pos -= (up * speed);
		break;
		break;
	}
}

void Camera::OnKeyboard(int Key) {
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

void Camera::OnMouse(int x, int y) {
	int DeltaX = x - mousePos.x;
	int DeltaY = y - mousePos.y;

	mousePos.x = x;
	mousePos.y = y;

	angleH += (float)DeltaX / 20.0f;
	angleV += (float)DeltaY / 50.0f;

	if (DeltaX == 0) {
		if (x <= MARGIN) {
			onLeftEdge = true;
		}
		else if (x >= (windowW - MARGIN)) {
			onRightEdge = true;
		}
	}
	else {
		onLeftEdge = false;
		onRightEdge = false;
	}

	if (DeltaY == 0) {
		if (y <= MARGIN) {
			onUpperEdge = true;
		}
		else if (y >= (windowH - MARGIN)) {
			onLowerEdge = true;
		}
	}
	else {
		onUpperEdge = false;
		onLowerEdge = false;
	}

	Update();
}

void Camera::OnRender()
{
	bool ShouldUpdate = false;

	if (onLeftEdge) {
		angleH -= EDGE_STEP;
		ShouldUpdate = true;
	}
	else if (onRightEdge) {
		angleH += EDGE_STEP;
		ShouldUpdate = true;
	}

	if (onUpperEdge) {
		if (angleV > -90.0f) {
			angleV -= EDGE_STEP;
			ShouldUpdate = true;
		}
	}
	else if (onLowerEdge) {
		if (angleV < 90.0f) {
			angleV += EDGE_STEP;
			ShouldUpdate = true;
		}
	}

	if (ShouldUpdate) {
		Update();
	}
}

void Camera::Update() {
	// Goal is to rotate the VIEW vector around 2 axises using 2 angles 
	glm::vec3 Yaxis(0.0f, 1.0f, 0.0f);

	// Rotate the view vector by the horizontal angle around the vertical axis
	glm::vec3 viewV(1.0f, 0.0f, 0.0f);
	RotateQuaternion(viewV, angleH, Yaxis);
	glm::normalize(viewV);

	// Rotate the view vector by the vertical angle around the horizontal axis
	glm::vec3 U = glm::normalize(glm::cross(Yaxis, viewV));
	RotateQuaternion(viewV, angleV, U);

	target = viewV;

	up = glm::normalize(glm::cross(target, U));
}

void Camera::Init()
{
	glm::vec3 hTarget(target.x, 0.0, target.z);
	hTarget = glm::normalize(hTarget);

	float angleRad = glm::asin(glm::abs(hTarget.z));
	float angleDeg = glm::degrees(angleRad);

	if (hTarget.z >= 0.0f) {
		if (hTarget.x >= 0.0f) {
			angleH = 360.0f - angleDeg;
		}
		else {
			angleH = 180.0f + angleDeg;
		}
	}
	else {
		if (hTarget.x >= 0.0f) {
			angleH = angleDeg;
		}
		else {
			angleH = 180.0f - angleDeg;
		}
	}

	float angleVRad = glm::asin(target.y);
	angleV = glm::degrees(angleVRad);

	mousePos.x = windowW / 2;
	mousePos.y = windowH / 2;
}

glm::mat4 Camera::ComputeCamMatrix() {
	glm::vec3 n = glm::normalize(target);
	glm::vec3 upNorm = glm::normalize(up);
	glm::vec3 u = glm::normalize(glm::cross(upNorm, n));
	glm::vec3 v = glm::normalize(glm::cross(n, u));

	glm::mat4 camMat(
		u.x, v.x, n.x, 0.0f,
		u.y, v.y, n.y, 0.0f,
		u.z, v.z, n.z, 0.0f,
		-glm::dot(u, pos), -glm::dot(v, pos), -glm::dot(n, pos), 1.0f
	);

	return camMat;
}

glm::mat4 Camera::GetMatrix() {
	glm::mat4 CameraTransformation = ComputeCamMatrix();
	return CameraTransformation;
}