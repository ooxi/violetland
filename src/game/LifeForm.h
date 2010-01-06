#include "../system/Object.h"
#include "SDL.h"

#ifndef LIVEOBJECT_H_
#define LIVEOBJECT_H_

class LifeForm: public Object {
private:
	int m_lastAttackTime;
	float m_health;
protected:
	bool m_dead;
public:
	enum LifeFormType {
		player = 0, monster
	};
	LifeForm(float x, float y, int w, int h);
	virtual void process(int deltaTime);
	virtual void draw();

	float Strength;
	float Agility;
	float Vitality;
	float TargetX, TargetY;
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
	const bool isDead();
	LifeForm::LifeFormType Type;
	bool Poisoned;
	int Frozen;
};

#endif /* LIVEOBJECT_H_ */
