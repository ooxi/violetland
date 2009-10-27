#include "Bullet.h"

Bullet::Bullet(float x, float y, BulletType type) :
	Object(x, y, 1, 1) {
	startX = x;
	startY = y;
	m_active = true;
	m_readyToRemove = false;
	Type = type;
	m_range = 0;
	Poisoned = false;
}

bool Bullet::isActive() {
	return m_active;
}

bool Bullet::isReadyToRemove() {
	return m_readyToRemove;
}

void Bullet::deactivate()
{
	m_active = false;
}