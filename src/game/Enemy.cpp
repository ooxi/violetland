#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Enemy.h"

Enemy::Enemy(float x, float y, Sprite *sprite, Sprite *bleedSprite,
		Sound* hitSound) :
	LiveObject(x, y, 128, 128) {
	m_body = new DynamicObject(x, y, sprite);
	TargetX = 0.0f;
	TargetY = 0.0f;
	m_bleedSprite = bleedSprite;
	m_hitSound = hitSound;
	m_hitSoundChannel = 0;
	m_bleeding = 0;
	DoNotDisturb = false;
	Angry = false;
	Poisoned = false;
}

void Enemy::rollFrame(bool forward) {
	m_body->rollFrame(forward);
}

void Enemy::hit(Bullet* bullet, float pX, float pY) {
	DynamicObject *newBleed = new DynamicObject(X, Y, m_bleedSprite);
	newBleed->Scale = Scale;
	newBleed->Angle = Object::fixAngle(M_PI - bullet->Angle - 90);
	m_bleeds.push_back(newBleed);
	if (!m_hitSound->isPlaying()) {
		m_hitSound->play(0, 0);
		m_hitSound->setPos(Object::calculateAngle(pX, pY, X, Y),
				Object::calculateDistance(pX, pY, X, Y));
	}
	setHealth(getHealth() - bullet->Damage);
	Poisoned = bullet->Poisoned || Poisoned;
	Angry = true;
	Angle += ((rand() % 100) - 50) * bullet->Damage;
}

bool Enemy::isBleeding() {
	if (!m_bleeds.empty() && m_bleeding > 600) {
		m_bleeding = 0;
		return true;
	} else {
		return false;
	}
}

void Enemy::process(int deltaTime) {
	float newAngle = Object::calculateAngle(X, Y, TargetX, TargetY);
	float prevAngle = Angle;
	Object::turn(newAngle, MaxSpeed(), deltaTime);
	float deltaAngle = Angle - prevAngle;

	for (unsigned int i = 0; i < m_bleeds.size(); i++) {
		m_bleeds[i]->Angle += deltaAngle;
	}

	move(deltaTime);

	setHealth(getHealth() + HealthRegen() * deltaTime);

	if (Poisoned)
		setHealth(getHealth() - 0.0002 * deltaTime);

	if (!m_bleeds.empty()) {
		m_bleeding += deltaTime;

		for (int i = m_bleeds.size() - 1; i >= 0; i--) {
			if (m_bleeds[i]->AMask <= 0) {
				delete m_bleeds[i];
				m_bleeds.erase(m_bleeds.begin() + i);
				continue;
			}

			m_bleeds[i]->AMask -= 0.001;

			m_bleeds[i]->X = X;
			m_bleeds[i]->Y = Y;

			m_bleeds[i]->rollFrame(true);
		}
	}
}

void Enemy::draw() {
	m_body->draw(X, Y, Angle, Scale, RMask, GMask, BMask, AMask);

	for (unsigned int i = 0; i < m_bleeds.size(); i++) {
		m_bleeds[i]->draw();
	}
}

Enemy::~Enemy() {
	delete m_body;
	for (unsigned int i = 0; i < m_bleeds.size(); i++) {
		delete m_bleeds[i];
	}
	m_bleeds.clear();
}
