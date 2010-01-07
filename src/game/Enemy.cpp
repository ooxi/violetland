#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Enemy.h"

Enemy::Enemy(Sprite *sprite, Sprite* deathSprite, Sound* hitSound) :
	LifeForm(0, 0, 128, 128) {
	m_body = new DynamicObject(0, 0, sprite);
	m_deathSprite = deathSprite;
	m_hitSound = hitSound;
	m_hitSoundChannel = 0;
	m_bleeding = 0;
	DoNotDisturb = false;
	Angry = false;
	Type = LifeForm::monster;
}

void Enemy::rollFrame(bool forward) {
	m_body->rollFrame(forward);
}

void Enemy::hit(Bullet* bullet, float pX, float pY) {
	if (!m_hitSound->isPlaying()) {
		m_hitSound->play(0, 0);
		m_hitSound->setPos(Object::calculateAngle(pX, pY, X, Y),
				Object::calculateDistance(pX, pY, X, Y));
	}
	setHealth(getHealth() - bullet->Damage);
	Poisoned = bullet->Poisoned || Poisoned;
	Angry = true;
	Angle += ((rand() % 50) - 25) * bullet->Damage;
}

bool Enemy::isBleeding() {
	if (m_bleeding > 600) {
		m_bleeding = 0;
		return true;
	} else {
		return false;
	}
}

bool Enemy::isDeathPhase() {
	return m_body->AnimSprite == m_deathSprite;
}

bool Enemy::isReasyToDisappear() {
	return isDeathPhase() && m_body->Frame
			== m_body->AnimSprite->getFramesCount() - 1;
}

void Enemy::process(int deltaTime) {
	LifeForm::process(deltaTime);

	if (m_dead) {
		if (!isDeathPhase()) {
			m_body = new DynamicObject(X, Y, m_deathSprite);
		} else {
			if (!isReasyToDisappear())
				m_body->rollFrame(true);
		}
	} else {
		if (Frozen == 0) {
			float newAngle = Object::calculateAngle(X, Y, TargetX, TargetY);
			Object::turn(newAngle, MaxSpeed(), deltaTime);

			move(deltaTime);
		}
	}
}

StaticObject* Enemy::getCorpse() {
	StaticObject * corpse = new StaticObject(X, Y, m_width, m_height,
			m_body->getFrame(), false);
	corpse->Scale = Scale;
	corpse->Angle = Object::fixAngle(180 - Angle);
	corpse->RMask = RMask;
	corpse->GMask = GMask;
	corpse->BMask = BMask;
	corpse->AMask = AMask;
	return corpse;
}

void Enemy::draw() {
	m_body->draw(X, Y, Angle, Scale, RMask, GMask, BMask, AMask);
}

Enemy::~Enemy() {
	delete m_body;
}
