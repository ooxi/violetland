#include "LiveObject.h"

LiveObject::LiveObject(float x, float y, int w, int h) :
	Object(x, y, w, h) {
	Strength = 1.0f;
	Agility = 1.0f;
	Vitality = 1.0f;
	m_health = MaxHealth();
	m_lastAttackTime = SDL_GetTicks();
}

const float LiveObject::MaxHealth() {
	return Vitality > 0.8f ? 1.0f + (Vitality - 1.0f) * 2.0f : 0.4f;
}

const float LiveObject::ChanceToEvade() {
	return Agility > 1.0f ? (Agility - 1.0f) / 2.0f : 0.0f;
}

const bool LiveObject::Attack() {
	int now = SDL_GetTicks();

	if (now - m_lastAttackTime > AttackDelay()) {
		m_lastAttackTime = now;
		return true;
	} else {
		return false;
	}
}

const float LiveObject::Damage() {
	return Strength / 10.0f;
}

const int LiveObject::AttackDelay() {
	return (1.0f - (Agility - 1.0f) / 2.0f) * 1000;
}

const float LiveObject::MaxSpeed() {
	return Agility / 5.0f;
}

const float LiveObject::HealthRegen() {
	return Vitality > 1.0f ? (Vitality - 1.0f) * 0.000003f : 0.0f;
}

const float LiveObject::ReloadSpeedMod() {
	return 1.0f / Agility;
}

const float LiveObject::WeaponRetForceMod() {
	return Strength > 1.0f ? 1.0f - (Strength - 1.0f) : 1.0f;
}

void LiveObject::setHealth(float value) {
	m_health = value;
	if (m_health > MaxHealth())
		m_health = MaxHealth();
	if (m_health < 0)
		m_health = 0;
}

const float LiveObject::getHealth() {
	return m_health;
}
