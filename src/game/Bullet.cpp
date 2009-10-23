#include "Bullet.h"

Bullet::Bullet(float x, float y, Texture *texture) :
	StaticObject(x, y, 32, 32, texture, false) {
	m_tex = texture;
	Speed = 3;
	Damage = 1;
	MaxRange = 1000;
	m_range = 0;
	Poisoned = false;
}

Texture *Bullet::getTextureRef() {
	return m_tex;
}

void Bullet::process(int deltaTime) {
	Object::move(deltaTime);
	m_range += Speed * deltaTime;
}

bool Bullet::isFall() {
	return m_range >= MaxRange;
}
