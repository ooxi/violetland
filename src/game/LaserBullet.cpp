#include "LaserBullet.h"

LaserBullet::LaserBullet(float x, float y) : Bullet(x, y, BulletType::laser) 
{
	Speed = 3;
	Damage = 1;
	MaxRange = 1000;
	Alpha = 0.75f;
}

void LaserBullet::process(int deltaTime) {
	if (startX == X && startY == Y)
	{
		X = startX - cos((Angle + 90) * M_PI / 180) * deltaTime * MaxRange;
		Y = startY - sin((Angle + 90) * M_PI / 180) * deltaTime * MaxRange;
	}

	Alpha -= 0.002f * deltaTime;
	if (Alpha < 0) Alpha = 0;

	m_active = m_active && Alpha > 0.3f;
	m_readyToRemove = !m_active && Alpha == 0;
}

void LaserBullet::draw() {
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor4f(0.3f, 1.0f, 0.3f, Alpha);
	glVertex3f(X, Y, 0);
	glColor4f(0.3f, 1.0f, 0.3f, Alpha);
	glVertex3f(startX, startY, 0);	
	glEnd();
}

bool LaserBullet::checkHit(Object* objRef)
{
	return m_active && objRef->detectCollide(startX, startY, X, Y);
}