#include "Camera.h"

Camera::Camera() {
	X = Y = 0;
	setW(1600);
	setH(1200);
}

void Camera::applyGLOrtho() {
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();

	glOrtho(X - m_halfW, X + m_halfW, Y + m_halfH, Y - m_halfH, -1.0f, 1.0f);

	glMatrixMode( GL_MODELVIEW);
	//glLoadIdentity();
}

Camera::~Camera() {
	// nothing
}
