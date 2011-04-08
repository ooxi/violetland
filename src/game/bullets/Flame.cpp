#include "Flame.h"

namespace violetland {

Flame::Flame(float x, float y, float targetX, float targetY, Texture* tex) :
	Bullet(x, y, x, y, BULLET_FLAME) {
	m_targetX = targetX;
	m_targetY = targetY;
	m_distance = Object::calc_dist(X, Y, m_targetX, m_targetY);
	m_img = new StaticObject(x, y, 128, 128, tex, false);
	m_img->RMask = 1.0;
	m_img->GMask = (float) (rand() % 50) / 100 + 0.4;
	m_img->BMask = 0.5;
	m_img->Scale = 0.001;
	m_rotation = (float) ((rand() % 10) - 10) / 10;
}

void Flame::process(int deltaTime) {
	X -= cos((Angle + 90) * M_PI / 180) * deltaTime * Speed;
	Y -= sin((Angle + 90) * M_PI / 180) * deltaTime * Speed;

	m_img->X = X;
	m_img->Y = Y;

	m_range += Speed * deltaTime;

	Speed = Speed * 0.96;
	m_img->Scale += Speed / 10;

	m_img->AMask = Speed / 1.5;
	m_img->Angle += m_rotation;

	if (Speed < 0.1)
		m_active = false;

	if (Speed < 0.01)
		m_readyToRemove = true;
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
