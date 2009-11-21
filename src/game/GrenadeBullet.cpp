#include "GrenadeBullet.h"

GrenadeBullet::GrenadeBullet(float x, float y, float targetX, float targetY) :
	Bullet(x, y, x, y, Bullet::grenade) {
	m_targetX = targetX;
	m_targetY = targetY;
	m_distance = Object::calculateDistance(X, Y, m_targetX, m_targetY);
}

void GrenadeBullet::process(int deltaTime) {
	float d = Object::calculateDistance(X, Y, m_targetX, m_targetY);
	float dp = d / m_distance;
	float k = 1.0f - fabs(0.5f - dp);

	float a = Object::calculateAngle(X, Y, m_targetX, m_targetY);

	X -= cos((a + 90) * M_PI / 180) * deltaTime * Speed * k;
	Y -= sin((a + 90) * M_PI / 180) * deltaTime * Speed * k;
	m_range += Speed * deltaTime * k;

	m_active = !(m_range >= m_distance);
	m_readyToRemove = !m_active;
}

void GrenadeBullet::draw() {
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex3f(X - 5.0f, Y - 5.0f, 0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glVertex3f(X + 5.0f, Y + 5.0f, 0);
	glEnd();
}

GrenadeBullet::~GrenadeBullet() {
	// nothing
}
