#include "Camera.h"

violet::Camera::Camera() {
	X = Y = 0;
	setW(1600);
	setH(1200);
	m_aspect = (float)getW() / getH();
}

void violet::Camera::applyGLOrtho() {
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();

	glOrtho(X - m_halfW, X + m_halfW, Y + m_halfH, Y - m_halfH, -1.0f, 1.0f);

	glMatrixMode( GL_MODELVIEW);
}

violet::Camera::~Camera() {
	// nothing
}
