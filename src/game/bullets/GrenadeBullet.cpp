#include "GrenadeBullet.h"

GrenadeBullet::GrenadeBullet(float x, float y, float targetX, float targetY,
		Sprite* sprite) :
	Bullet(x, y, x, y, Bullet::grenade) {
	m_targetX = targetX;
	m_targetY = targetY;
	m_distance = Object::calc_dist(X, Y, m_targetX, m_targetY);
	m_image = new DynamicObject(x, y, sprite);
}

void GrenadeBullet::process(int deltaTime) {
	float d = Object::calc_dist(X, Y, m_targetX, m_targetY);
	float dp = d / m_distance;
	float k = 0.75f - fabs(0.5f - dp);

	float a = Object::calc_angle(X, Y, m_targetX, m_targetY);

	X -= cos((a + 90) * M_PI / 180) * deltaTime * Speed * k;
	Y -= sin((a + 90) * M_PI / 180) * deltaTime * Speed * k;
	m_range += Speed * deltaTime * k;
	m_scale = k / 2.0f;

	m_image->rollFrame(true);

	m_active = !(m_range >= m_distance);
	m_readyToRemove = !m_active;
}

void GrenadeBullet::draw() {
	m_image->draw(X, Y, 0.0f, m_scale, 1.0f, 1.0f, 1.0f, 1.0f);
}

GrenadeBullet::~GrenadeBullet() {
	delete m_image;
}
