#include "Flame.h"

namespace violetland {

Flame::Flame(float x, float y, Texture* tex) :
	Bullet(x, y, x, y, BULLET_FLAME) {
	m_img = new StaticObject(x, y, 128, 128, tex, false);
	m_img->RMask = 1.0;
	m_img->GMask = (float) (rand() % 50) / 100 + 0.4;
	m_img->BMask = 0.3;
	m_img->Scale = 0.001;
}

void Flame::process(int deltaTime) {
	const float dist = m_range / MaxRange;

	if (m_active && dist >= 0.7)
		m_active = false;

	if (dist >= 1.0)
		m_readyToRemove = true;

	const float relSpeed = (1.0 - dist) * Speed;

	X -= cos((Angle + 90) * M_PI / 180) * deltaTime * relSpeed;
	Y -= sin((Angle + 90) * M_PI / 180) * deltaTime * relSpeed;

	m_img->X = X;
	m_img->Y = Y;

	m_range += relSpeed * deltaTime;

	m_img->Scale = 2.5 * dist;

	if (dist < 0.5)
		m_img->AMask = dist;
	else
		m_img->AMask = 1.0 - dist;
}

void Flame::draw() {
	m_img->draw(false, false);
}

bool Flame::checkHit(Object* objRef) {
	bool hit = m_active && objRef->detectCollide(m_img);
	return hit;
}

Flame::~Flame() {
	delete m_img;
}

}
