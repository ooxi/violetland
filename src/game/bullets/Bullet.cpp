#include "Bullet.h"

namespace violetland {

Bullet::Bullet(float x, float y, float dX, float dY, BulletType type) :
	Object(x, y, 1, 1) {
	startX = x;
	startY = y;
	dStartX = dX;
	dStartY = dY;
	m_active = true;
	m_readyToRemove = false;
	Type = type;
	m_range = 0;
	Poisoned = false;
	BigCalibre = false;
	Penetrating = false;
}

void Bullet::process(int deltaTime) {
	//nothing
}

void Bullet::draw() {
	//nothing
}

bool Bullet::checkHit(Object* objRef) {
	return false;
}

bool Bullet::isActive() {
	return m_active;
}

bool Bullet::isReadyToRemove() {
	return m_readyToRemove;
}

void Bullet::deactivate() {
	m_active = false;
}

Bullet::~Bullet() {
	//nothing
}

}
