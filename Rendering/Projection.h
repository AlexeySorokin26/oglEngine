#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/trigonometric.hpp>


class PerspectiveProjection {
public:
	PerspectiveProjection
	(
		float fov = 0.0f,
		float w = 0.0f,
		float h = 0.0f,
		float zNear = 0.0f,
		float zFar = 0.0f
	) : fov(fov), w(w), h(h), zNear(zNear), zFar(zFar) {
		Init();
	}
	glm::mat4 GetMatrix() {
		Init();
		return projectionMat;
	}
private:
	void Init() {
		float tanHalfFov = tanf(glm::radians(fov / 2.0f));
		float f = 1.0f / tanHalfFov;
		float ar = (float)w / (float)h;

		float zRange = zNear - zFar;
		float a = (-zFar - zNear) / zRange;
		float b = 2.f * zFar * zNear / zRange;

		projectionMat = glm::mat4( // c // it's transposed because of column major which uses glm
			f / ar, 0, 0, 0,
			0, f, 0, 0,
			0, 0, a, 1,
			0, 0, b, 0
		);
	}
private:
	glm::mat4 projectionMat;

	float fov;
	float w;
	float h;
	float zNear;
	float zFar;
};