#ifndef OBJECT_H_
#define OBJECT_H_

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include <cmath>

class Object {
protected:
	int m_width, m_height;
	float m_left, m_right, m_top, m_bottom;
public:
	void move(int deltaTime);
	static float fixAngle(float angle);
	static float calc_angle(float x1, float y1, float x2, float y2);
	static float calc_dist(float x1, float y1, float x2, float y2);
	Object(float x, float y, int width, int height);
	void turn(float targetAngle, float angleSpeed, int deltaTime);
	const float getWidth();
	const float getHeight();
	const float getLeft();
	const float getRight();
	const float getTop();
	const float getBottom();
	const bool detectCollide(Object *refObj);
	const bool detectCollide(float x, float y);
	const bool detectCollide(float x1, float y1, float x2, float y2);
	const bool detectCollide(float x1, float y1, float x2, float y2, float* ix,
			float* iy);
	void setMask(float _RMask, float _GMask, float _BMask, float _AMask);
	float X, Y;
	float Scale;
	float Angle;
	float RMask, GMask, BMask, AMask;
	float HitR;
	float Acceleration, Speed;
};

#endif /* OBJECT_H_ */
