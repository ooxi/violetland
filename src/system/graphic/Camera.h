#ifndef CAMERA_H_
#define CAMERA_H_

#include "SDL_opengl.h"

class Camera {
private:
	int m_w, m_h, m_halfW, m_halfH;
	float m_aspect;
public:
	// Point of view
	float X, Y;

	// Default size: 1600x1200, aspect ratio 1,(3)
	Camera();
	~Camera();

	const int getW() const { return m_w; }

	// Explicit set width and recalculate height based on aspect ratio
	void setW(int value) {
		m_w = value;
		m_halfW = value / 2;
		setH((int) (getW() / m_aspect));
	}

	const int getH() const { return m_h; }

	// Explicit set height and recalculate aspect ratio
	void setH(int value) {
		m_h = value;
		m_halfH = value / 2;
		m_aspect = (float) getW() / getH();
	}

	const float getAspect() const { return m_aspect; }

	// Explicit set aspect ratio and recalculate height based on aspect ratio
	void setAspect(float aspect)
	{
		m_aspect = aspect;
		setH((int) (getW() / m_aspect));
	}

	const int getHalfW() const { return m_halfW; }

	const int getHalfH() const { return m_halfH; }

	void applyGLOrtho();
};

#endif /* CAMERA_H_ */
