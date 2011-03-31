#include "Camera.h"

Camera::Camera() {
	X = Y = 0;
	setW(1600);
	setH(1200);
}

const int Camera::getW() {
	return m_w;
}

void Camera::setW(int value) {
	m_w = value;
	m_halfW = value / 2;
}

const int Camera::getH() {
	return m_h;
}

void Camera::setH(int value) {
	m_h = value;
	m_halfH = value / 2;
}

const int Camera::getHalfW() {
	return m_halfW;
}

const int Camera::getHalfH() {
	return m_halfH;
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
