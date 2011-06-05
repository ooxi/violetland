#include <sstream>

#include "LifeForm.h"

using namespace violetland;

LifeForm::LifeForm(float x, float y, int w, int h) :
	Object(x, y, w, h) {
	std::ostringstream buf;
	buf << UidGenerator::getInstance()->getId();
	Id = buf.str();
	Strength = 1.0f;
	Agility = 1.0f;
	Vitality = 1.0f;
	m_health = MaxHealth();
	State = LIFEFORM_STATE_ALIVE;
	m_lastAttackTime = SDL_GetTicks();
	TargetX = TargetY = 0.0f;
	Poisoned = false;
	m_walking = false;
	Frozen = 0;
	Burning = false;
	Level = 1;
	Name = "Unknown lifeform";
}

void LifeForm::draw() {
	// nothing
}

Sound* LifeForm::hit(float damage, bool poison) {
	setHealth(getHealth() - damage);

	if (getHealth() == 0 && State == LIFEFORM_STATE_ALIVE) {
		State = LIFEFORM_STATE_SMITTEN;
	}

	return NULL;
}

void LifeForm::process(int deltaTime) {
	if (Frozen > 0) {
		Frozen -= deltaTime;
		if (Frozen < 0)
			Frozen = 0;
	}

	if (State == LIFEFORM_STATE_ALIVE && Frozen == 0) {
		setHealth(getHealth() + HealthRegen() * deltaTime);

		if (Poisoned)
			setHealth(getHealth() - 0.0002 * deltaTime);

		if (Burning) {
			RMask -= 0.02 / MaxHealth();
			if (RMask < 0)
				RMask = 0;
			GMask -= 0.02 / MaxHealth();
			if (GMask < 0)
				GMask = 0;
			BMask -= 0.02 / MaxHealth();
			if (BMask < 0)
				BMask = 0;
			AMask += 0.01 / MaxHealth();
			if (AMask > 1)
				AMask = 1;

			setHealth(getHealth() - 0.0004 * deltaTime);
		}

		if (!m_walking) {
			Speed -= Acceleration * deltaTime;
			if (Speed < 0)
				Speed = 0;
		}

		m_walking = false;

		Object::move(deltaTime);
	}
}

void LifeForm::move(float direction, int deltaTime) {
	if (m_walkDelay > 0)
		return;

	m_walking = true;

	Speed += Acceleration * deltaTime;
	if (Speed > MaxSpeed())
		Speed = MaxSpeed();

	turn(direction, MaxSpeed(), deltaTime);
}

const float LifeForm::MaxHealth() const {
	return getVitality() > 0.8f ? 1.0f + (getVitality() - 1.0f) * 2.0f
			+ (getStrength() - 1.0f) : 0.4f;
}

const float LifeForm::ChanceToEvade() const {
	return getAgility() > 1.0f ? (getAgility() - 1.0f) / 2.0f : 0.0f;
}

const bool LifeForm::Attack() {
	int now = SDL_GetTicks();

	if (now - m_lastAttackTime > AttackDelay()) {
		m_lastAttackTime = now;
		return true;
	} else {
		return false;
	}
}

const float LifeForm::Damage() const {
	return getStrength() / 8.0f;
}

const int LifeForm::AttackDelay() const {
	return (1.0f - (getAgility() - 1.0f) / 2.0f) * 1000;
}

const float LifeForm::MaxSpeed() const {
	return getAgility() / 5.0f;
}

const float LifeForm::HealthRegen() const {
	return getVitality() > 1.0f ? (getVitality() - 1.0f) * 0.000005f : 0.0f;
}

const float LifeForm::ReloadSpeedMod() const {
	return 1.0f / getAgility();
}

const float LifeForm::WeaponRetForceMod() const {
	return getStrength() > 1.0f ? 1.0f - (getStrength() - 1.0f) * 1.1f : 1.0f;
}

const float LifeForm::fixHealth(float health) const {
	if (health > MaxHealth())
		return MaxHealth();
	else if (health < 0)
		return 0;
	else
		return health;
}

LifeForm::~LifeForm()
{
	// nothing
}
