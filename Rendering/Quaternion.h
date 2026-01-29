#pragma once

#include <glm/mat4x4.hpp> 
#include <glm/vec3.hpp> 
#include <cmath>

static float ToRadian(float degrees) {
	return degrees * (3.14 / 180.0f);
}

struct Quaternion
{
	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;

	Quaternion() {}

	Quaternion(float Angle, const glm::vec3& V) {
		float HalfAngleInRadians = ToRadian(Angle / 2);

		float SineHalfAngle = sinf(HalfAngleInRadians);
		float CosHalfAngle = cosf(HalfAngleInRadians);

		x = V.x * SineHalfAngle;
		y = V.y * SineHalfAngle;
		z = V.z * SineHalfAngle;
		w = CosHalfAngle;
	}

	Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

	void Normalize() {
		const float len = sqrtf(x * x + y * y + z * z);
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}

	Quaternion Conjugate() const {
		Quaternion ret(-x, -y, -z, w);
		return ret;
	}
};

Quaternion operator*(const Quaternion& l, const Quaternion& r)
{
	Quaternion ret;

	ret.w = (l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z);
	ret.x = (l.w * r.x) + (l.x * r.w) + (l.y * r.z) - (l.z * r.y);
	ret.y = (l.w * r.y) - (l.x * r.z) + (l.y * r.w) + (l.z * r.x);
	ret.z = (l.w * r.z) + (l.x * r.y) - (l.y * r.x) + (l.z * r.w);

	return ret;
}

Quaternion operator*(const Quaternion& q, const glm::vec3& v)
{
	Quaternion ret;

	ret.w = -q.x * v.x - q.y * v.y - q.z * v.z;
	ret.x = q.w * v.x + q.y * v.z - q.z * v.y;
	ret.y = q.w * v.y - q.x * v.z + q.z * v.x;
	ret.z = q.w * v.z + q.x * v.y - q.y * v.x;

	return ret;
}

void RotateQuaternion(glm::vec3& rotatedV, float Angle, const glm::vec3& V)
{
	Quaternion RotationQ(Angle, V);

	Quaternion ConjugateQ = RotationQ.Conjugate();

	Quaternion W = RotationQ * rotatedV * ConjugateQ;

	rotatedV.x = W.x;
	rotatedV.y = W.y;
	rotatedV.z = W.z;
}