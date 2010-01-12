#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Enemy.h"

Enemy::Enemy(MonsterTemplate* base, int lvl) :
	LifeForm(0, 0, 128, 128) {
	Base = base;

	float t = Base->Strength + Base->Agility + Base->Vitality;
	float sp = Base->Strength / t;
	float ap = Base->Agility / t;
	float vp = Base->Vitality / t;

	Strength = Base->Strength + 0.1f * lvl * sp;
	Agility = Base->Agility + 0.1f * lvl * ap;
	Vitality = Base->Vitality + 0.1f * lvl * vp;

	t = Strength + Agility + Vitality;

	RMask = Vitality / t;
	GMask = Strength / t;
	BMask = Agility / t * 0.7f;

	setHealth(MaxHealth());
	Speed = MaxSpeed();

	HitR = 0.3;
	m_body = new DynamicObject(0, 0, Base->WalkSprite);
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
	if (!Base->HitSound->isPlaying()) {
		Base->HitSound->play(0, 0);
		Base->HitSound->setPos(Object::calculateAngle(pX, pY, X, Y),
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
	return m_body->AnimSprite == Base->DeathSprite;
}

bool Enemy::isReasyToDisappear() {
	return isDeathPhase() && m_body->Frame
			== m_body->AnimSprite->getFramesCount() - 1;
}

void Enemy::process(int deltaTime) {
	LifeForm::process(deltaTime);

	if (m_dead) {
		if (!isDeathPhase()) {
			m_body = new DynamicObject(X, Y, Base->DeathSprite);
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
