#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "Monster.h"

violetland::Monster::Monster(MonsterTemplate* base, int lvl) :
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

	setHealth(MaxHealth());

	HitR = 0.3;
	Acceleration = 0.0004f;

	if (Base->WalkTime > 0)
		m_walkTime = (rand() % Base->WalkTime);
	else
		m_walkTime = 0;

	m_walkDelay = 0;

	m_body = new DynamicObject(0, 0, Base->WalkSprite);
	m_hitSoundChannel = 0;
	m_bleedDelay = 0;
	m_bleedCount = 0;
	Angry = false;
	targetId = "ambient";
	Type = LIFEFORM_MONSTER;
}

void violetland::Monster::rollFrame(bool forward) {
	m_body->rollFrame(forward);
}

Sound* violetland::Monster::hit(float damage, bool poison, bool stop) {
	LifeForm::hit(damage, poison);

	m_bleedCount += damage * 5;
	Poisoned = poison || Poisoned;

	if (stop)
		Speed = 0.0f;

	Angle += ((rand() % 50) - 25) * damage;

	if (m_walkDelay > 0)
		m_walkDelay = 1;

	if (!Base->HitSounds.empty()) {
		int s = rand() % (int) Base->HitSounds.size();
		return Base->HitSounds[s];
	}

	return NULL;
}

bool violetland::Monster::isBleeding() {
	if (m_bleedCount > 0 && m_bleedDelay > 600) {
		m_bleedDelay = 0;
		m_bleedCount--;
		return true;
	} else {
		return false;
	}
}

void violetland::Monster::process(int deltaTime) {
	LifeForm::process(deltaTime);

	if (m_walkTime > 0) {
		m_walkTime -= deltaTime;
		if (m_walkTime <= 0) {
			m_walkTime = 0;
			m_walkDelay = Base->WalkDelay;
		}
	}

	if (m_walkDelay > 0) {
		m_walkDelay -= deltaTime;
		if (m_walkDelay <= 0) {
			m_walkTime = Base->WalkTime;
			m_walkDelay = 0;
		}
	}

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
		delete m_body;
		m_body = new DynamicObject(X, Y, Base->DeathSprite);
		State = LIFEFORM_STATE_DYING;
	}
}

StaticObject* violetland::Monster::getCorpse() {
	StaticObject * corpse = new StaticObject(X, Y, m_width, m_height,
			m_body->getFrame(), false);
	corpse->Scale = Scale;
	corpse->Angle = Object::fixAngle(180 - Angle);
	corpse->setMask(RMask, GMask, BMask, AMask);
	return corpse;
}

void violetland::Monster::draw() {
	m_body->draw(X, Y, Angle, Scale, RMask, GMask, BMask, AMask);
}

violetland::Monster::~Monster() {
	delete m_body;
}
