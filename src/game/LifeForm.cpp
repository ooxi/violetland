#include "LifeForm.h"

LifeForm::LifeForm(float x, float y, int w, int h) :
	Object(x, y, w, h) {
	Strength = 1.0f;
	Agility = 1.0f;
	Vitality = 1.0f;
	m_health = MaxHealth();
	m_dead = false;
	m_lastAttackTime = SDL_GetTicks();
	TargetX = TargetY = 0.0f;
	Poisoned = false;
	Frozen = 0;
	Level = 1;
	Name = "Unknown lifeform";
}

void LifeForm::draw() {
	// nothing
}

void LifeForm::process(int deltaTime) {
	if (Frozen > 0) {
		Frozen -= deltaTime;
		if (Frozen < 0)
			Frozen = 0;
	}

	m_dead = m_dead || getHealth() == 0;

	if (!m_dead && Frozen == 0) {
		setHealth(getHealth() + HealthRegen() * deltaTime);

		if (Poisoned)
			setHealth(getHealth() - 0.0002 * deltaTime);
	}
}

const bool LifeForm::isDead() {
	return m_dead;
}

const float LifeForm::MaxHealth() {
	return Vitality > 0.8f ? 1.0f + (Vitality - 1.0f) * 2.0f : 0.4f;
}

const float LifeForm::ChanceToEvade() {
	return Agility > 1.0f ? (Agility - 1.0f) / 2.0f : 0.0f;
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

const float LifeForm::Damage() {
	return Strength / 8.0f;
}

const int LifeForm::AttackDelay() {
	return (1.0f - (Agility - 1.0f) / 2.0f) * 1000;
}

const float LifeForm::MaxSpeed() {
	return Agility / 5.0f;
}

const float LifeForm::HealthRegen() {
	return Vitality > 1.0f ? (Vitality - 1.0f) * 0.000004f : 0.0f;
}

const float LifeForm::ReloadSpeedMod() {
	return 1.0f / Agility;
}

const float LifeForm::WeaponRetForceMod() {
	return Strength > 1.0f ? 1.0f - (Strength - 1.0f) * 1.1f : 1.0f;
}

void LifeForm::setHealth(float value) {
	m_health = value;
	if (m_health > MaxHealth())
		m_health = MaxHealth();
	if (m_health < 0)
		m_health = 0;
}

const float LifeForm::getHealth() {
	return m_health;
}
