#include "LaserBullet.h"

namespace violetland {

LaserBullet::LaserBullet(float x, float y, float dX, float dY) :
	Bullet(x, y, dX, dY, BULLET_LASER) {
	Speed = 3;
	Damage = 1;
	MaxRange = 1000;
	Alpha = 0.75f;
	m_hit = false;
}

void LaserBullet::process(int deltaTime) {
	X = startX - cos((Angle + 90) * M_PI / 180) * MaxRange;
	Y = startY - sin((Angle + 90) * M_PI / 180) * MaxRange;

	m_active = !m_hit && Alpha > 0.15f;

	m_readyToRemove = !m_active && Alpha == 0;

	Alpha -= 0.002f * deltaTime;
	if (Alpha < 0)
		Alpha = 0;
}

void LaserBullet::draw() {
	glDisable(GL_TEXTURE_2D);

	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor4f(0.3f, 1.0f, 0.3f, Alpha / 3.0f);
	glVertex3f(X, Y, 0);
	glColor4f(0.3f, 1.0f, 0.3f, Alpha);
	glVertex3f(dStartX, dStartY, 0);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

bool LaserBullet::checkHit(Object* objRef) {
	bool hit = m_active && objRef->detectCollide(startX, startY, X, Y);
	m_hit = m_hit || hit;
	return hit;
}

}
