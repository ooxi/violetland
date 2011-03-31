#ifndef LIFEFORM_H_
#define LIFEFORM_H_

#ifdef _WIN32
#include "windows.h"
#endif //_WIN32W

#include "../../system/graphic/StaticObject.h"
#include "../../system/graphic/DynamicObject.h"
#include "../../system/sound/Sound.h"
#include "../../system/utility/UidGenerator.h"

using namespace std;

namespace violetland {
enum LifeFormType {
	LIFEFORM_PLAYER = 0, LIFEFORM_MONSTER
};

enum LifeFormState {
	LIFEFORM_STATE_ALIVE = 0,
	LIFEFORM_STATE_SMITTEN,
	LIFEFORM_STATE_DYING,
	LIFEFORM_STATE_DIED,
	LIFEFORM_STATE_BURST
};

class LifeForm: public Object {
private:
	int m_lastAttackTime;
	float m_health;
protected:
	DynamicObject *m_body;
	bool m_walking;
	int m_walkTime;
	int m_walkDelay;
public:
	LifeForm(float x, float y, int w, int h);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual Sound* hit(float damage, bool poison);

	void move(float direction, int deltaTime);

	std::string Id;
	std::string Name;
	int Level;
	float Strength;
	float Agility;
	float Vitality;
	float TargetX, TargetY;
	LifeFormState State;
	void setHealth(float value);
	virtual float getStrength();
	virtual float getAgility();
	virtual float getVitality();
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
	LifeFormType Type;
	virtual StaticObject* getCorpse();
	bool Poisoned;
	int Frozen;
};
}

#endif /* LIFEFORM_H_ */
