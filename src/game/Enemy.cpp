#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Enemy.h"

Enemy::Enemy(float x, float y, Sprite *sprite, Sprite* deathSprite,
		Sprite *bleedSprite, Sound* hitSound) :
	LifeForm(x, y, 128, 128) {
	m_body = new DynamicObject(x, y, sprite);
	m_bleedSprite = bleedSprite;
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
	DynamicObject *newBleed = new DynamicObject(X, Y, m_bleedSprite);
	newBleed->Scale = Scale;
	newBleed->Angle = bullet->Angle;
	m_bleeds.push_back(newBleed);
	if (!m_hitSound->isPlaying()) {
		m_hitSound->play(0, 0);
		m_hitSound->setPos(Object::calculateAngle(pX, pY, X, Y),
				Object::calculateDistance(pX, pY, X, Y));
	}
	setHealth(getHealth() - bullet->Damage);
	Poisoned = bullet->Poisoned || Poisoned;
	Angry = true;
	//	Angle += ((rand() % 80) - 40) * bullet->Damage;
}

bool Enemy::isBleeding() {
	if (!m_bleeds.empty() && m_bleeding > 600) {
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
		float newAngle = Object::calculateAngle(X, Y, TargetX, TargetY);
		float prevAngle = Angle;
		Object::turn(newAngle, MaxSpeed(), deltaTime);
		float deltaAngle = Angle - prevAngle;

		for (unsigned int i = 0; i < m_bleeds.size(); i++) {
			m_bleeds[i]->Angle += deltaAngle;
		}

		move(deltaTime);
	}

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
