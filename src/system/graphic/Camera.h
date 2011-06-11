#ifndef CAMERA_H_
#define CAMERA_H_

#include "SDL_opengl.h"

class Camera {
private:
	int m_w, m_h, m_halfW, m_halfH;
public:
	Camera();
	~Camera();
	const int getW() const {
		return m_w;
	}
	void setW(int value) {
		m_w = value;
		m_halfW = value / 2;
	}
	const int getH() const {
		return m_h;
	}
	void setH(int value) {
		m_h = value;
		m_halfH = value / 2;
	}
	const int getHalfW() const {
		return m_halfW;
	}
	const int getHalfH() const {
		return m_halfH;
	}
	void applyGLOrtho();
	float X, Y;
};

#endif /* CAMERA_H_ */
