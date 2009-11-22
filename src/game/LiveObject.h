#include "../system/Object.h"
#include "SDL.h"

#ifndef LIVEOBJECT_H_
#define LIVEOBJECT_H_

class LiveObject: public Object {
private:
	int m_lastAttackTime;
	float m_health;
public:
	enum LifeFormType {
		player = 0, zombie
	};
	LiveObject(float x, float y, int w, int h);
	float Strength;
	float Agility;
	float Vitality;
	virtual void draw();
	void setHealth(float value);
	const float getHealth();
	const float MaxHealth();
	const float MaxSpeed();
	const float ChanceToEvade();
	const bool Attack();
	const float Damage();
	const int AttackDelay();
	const float ReloadSpeedMod();
	const float WeaponRetForceMod();
	const float HealthRegen();
	LiveObject::LifeFormType Type;
};

#endif /* LIVEOBJECT_H_ */
