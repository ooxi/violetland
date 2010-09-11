#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Monster.h"

violetland::Enemy::Enemy(MonsterTemplate* base, int lvl) :
	LifeForm(0, 0, 128, 128) {
	Id = "10-" + Id;

	Base = base;
	Level = lvl;

	float t = Base->Strength + Base->Agility + Base->Vitality;
	float sp = Base->Strength / t;
	float ap = Base->Agility / t;
	float vp = Base->Vitality / t;

	Strength = Base->Strength + 0.03f * Level * sp;
	Agility = Base->Agility + 0.03f * Level * ap;
	Vitality = Base->Vitality + 0.03f * Level * vp;

	if ((rand() % 100) > 95) {
		Strength *= 2.0f;
		setMask(0.2f, 0.7f, 0.2f, 1.0f);
		Name = "Strong " + Base->Name;
	} else if ((rand() % 100) > 95) {
		Agility *= 1.8f;
		setMask(0.3f, 0.4f, 0.7f, 1.0f);
		Name = "Fast " + Base->Name;
	} else if ((rand() % 100) > 95) {
		Vitality *= 2.0f;
		setMask(1.0f, 0.2f, 0.2f, 1.0f);
		Name = "Healthy " + Base->Name;
	} else {
		setMask(0.8f, 0.7f, 0.4f, 1.0f);
		Name = "Regular " + Base->Name;
	}

	setHealth( MaxHealth());

	HitR = 0.3;
	Acceleration = 0.0004f;
	m_body = new DynamicObject(0, 0, Base->WalkSprite);
	m_hitSoundChannel = 0;
	m_bleedDelay = 0;
	m_bleedCount = 0;
	DoNotDisturb = false;
	Angry = false;
	Type = LIFEFORM_MONSTER;
}

void violetland::Enemy::rollFrame(bool forward) {
	m_body->rollFrame(forward);
}

void violetland::Enemy::hit(float damage, bool poison, float pX, float pY) {
	LifeForm::hit(damage, poison, pX, pY);
	if (!Base->HitSounds.empty()) {
		int s = rand() % (int) Base->HitSounds.size();
		Base->HitSounds[s]->play(7, 0, 0);
		Base->HitSounds[s]->setPos(Object::calculateAngle(pX, pY, X, Y),
				Object::calculateDistance(pX, pY, X, Y));
	}
	m_bleedCount += damage * 5;
	Poisoned = poison || Poisoned;
	Angry = true;
	Angle += ((rand() % 50) - 25) * damage;
}

bool violetland::Enemy::isBleeding() {
	if (m_bleedCount > 0 && m_bleedDelay > 600) {
		m_bleedDelay = 0;
		m_bleedCount--;
		return true;
	} else {
		return false;
	}
}

void violetland::Enemy::process(int deltaTime) {
	LifeForm::process(deltaTime);

	if (m_bleedCount > 0)
		m_bleedDelay += deltaTime;

	if (State == LIFEFORM_STATE_DYING) {
		if (m_body->Frame == m_body->AnimSprite->getFramesCount() - 1) {
			State = LIFEFORM_STATE_DIED;
		} else {
			m_body->rollFrame(true);
		}
	}

	if (State == LIFEFORM_STATE_SMITTEN) {
		m_body = new DynamicObject(X, Y, Base->DeathSprite);
		State = LIFEFORM_STATE_DYING;
	}
}

StaticObject* violetland::Enemy::getCorpse() {
	StaticObject * corpse = new StaticObject(X, Y, m_width, m_height,
			m_body->getFrame(), false);
	corpse->Scale = Scale;
	corpse->Angle = Object::fixAngle(180 - Angle);
	corpse->setMask(RMask, GMask, BMask, AMask);
	return corpse;
}

void violetland::Enemy::draw() {
	m_body->draw(X, Y, Angle, Scale, RMask, GMask, BMask, AMask);
}

violetland::Enemy::~Enemy() {
	delete m_body;
}
