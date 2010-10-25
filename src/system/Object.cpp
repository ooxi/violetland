#include "Object.h"

Object::Object(float x, float y, int width, int height) {
	X = x;
	Y = y;
	m_width = width;
	m_height = height;
	m_left = -m_width / 2;
	m_right = m_width / 2;
	m_top = -m_height / 2;
	m_bottom = m_height / 2;
	Scale = 1.0f;
	Angle = 0.0f;
	RMask = BMask = GMask = AMask = 1.0f;
	HitR = 1.0f;
	Acceleration = 0.1f;
	Speed = 0.0f;
}

float Object::fixAngle(float angle) {
	if (angle < 0)
		angle += 360;
	if (angle > 360)
		angle -= 360;

	return angle;
}

void Object::turn(float targetAngle, float angleSpeed, int deltaTime) {
	float arch1 = 0.0;
	float arch2 = 0.0;

	if (Angle > targetAngle) {
		arch1 = Angle - targetAngle;
		arch2 = 360 + targetAngle - Angle;
	}

	if (Angle < targetAngle) {
		arch1 = 360 + Angle - targetAngle;
		arch2 = targetAngle - Angle;
	}

	if (arch1 < angleSpeed * deltaTime || arch2 < angleSpeed * deltaTime) {
		Angle = targetAngle;
		return;
	}

	if (arch1 < arch2)
		Angle -= deltaTime * angleSpeed;

	if (arch1 >= arch2)
		Angle += deltaTime * angleSpeed;

	Angle = Object::fixAngle(Angle);
}

float Object::calc_angle(float x1, float y1, float x2, float y2) {
	return 180.0f / M_PI * atan2(x2 - x1, -y2 + y1);
}

const float Object::getWidth() {
	return m_width;
}

const float Object::getHeight() {
	return m_width;
}

float Object::calc_dist(float x1, float y1, float x2, float y2) {
	return hypot(x2 - x1, -y2 + y1);
}

void Object::move(int deltaTime) {
	X -= cos((Angle + 90) * M_PI / 180) * deltaTime * Speed;
	Y -= sin((Angle + 90) * M_PI / 180) * deltaTime * Speed;
}

const float Object::getLeft() {
	return X + m_left * Scale;
}

const float Object::getRight() {
	return X + m_right * Scale;
}

const float Object::getTop() {
	return Y + m_top * Scale;
}

const float Object::getBottom() {
	return Y + m_bottom * Scale;
}

const bool Object::detectCollide(Object *refObj) {
	return calc_dist(X, Y, refObj->X, refObj->Y) < HitR * Scale * m_width
			+ refObj->HitR * refObj->Scale * refObj->getWidth();
}

const bool Object::detectCollide(float x, float y) {
	return calc_dist(X, Y, x, y) < HitR * Scale * m_width;
}

const bool Object::detectCollide(float x1, float y1, float x2, float y2) {
	float tx, ty;
	return detectCollide(x1, y1, x2, y2, &tx, &ty);
}

const bool Object::detectCollide(float x1, float y1, float x2, float y2,
		float* ix, float* iy) {
	float k = (y1 - y2) / (x1 - x2);
	float b = y1 - k * x1;
	float r = HitR * Scale * m_width;

	float d = (pow((2 * k * b - 2 * X - 2 * Y * k), 2) - (4 + 4 * pow(k, 2))
			* (pow(b, 2) - pow(r, 2) + pow(X, 2) + pow(Y, 2) - 2 * Y * b));

	if (d < 0) {
		return false;
	}

	float resX1 = (-(2 * k * b - 2 * X - 2 * Y * k) - sqrt(d)) / (2 + 2 * pow(
			k, 2));
	//	float resX2 = ((-(2* k * b - 2* targetX - 2* targetY * k) + sqrt(d)) / (2
	//			+ 2* k * k));

	float resY1 = k * resX1 + b;

	if (std::abs(std::abs(x1 - resX1) + std::abs(x2 - resX1)
			- std::abs(x2 - x1)) < 1) {
		*ix = resX1;
		*iy = resY1;
		return true;
	}

	//	if (resX1 == resX2) {
	//	one intersection at resX1, y1
	//		return true;
	//	}

	// two intersections: resX1, y1 и resX2, y2
	// float y1 = k * resX1 + b;
	// float y2 = k * resX2 + b;

	return false;
}
void Object::setMask(float _RMask, float _GMask, float _BMask, float _AMask) {
	RMask = _RMask;
	GMask = _GMask;
	BMask = _BMask;
	AMask = _AMask;
}
