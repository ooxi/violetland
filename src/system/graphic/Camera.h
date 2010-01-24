#include "SDL_opengl.h"

#ifndef CAMERA_H_
#define CAMERA_H_

class Camera {
private:
	float m_w, m_h, m_halfW, m_halfH;
public:
	Camera();
	~Camera();
	const int getW();
	void setW(int value);
	const int getH();
	void setH(int value);
	const int getHalfW();
	const int getHalfH();
	void applyGLOrtho();
	float X, Y;
};

#endif /* CAMERA_H_ */
