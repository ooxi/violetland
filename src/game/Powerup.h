#ifndef POWERUP_H_
#define POWERUP_H_

#include "../system/graphic/StaticObject.h"

namespace violetland {
enum BonusType {
	BONUS_MEDIKIT = 0,
	BONUS_WEAPON,
	BONUS_GRENADES,
	BONUS_FREEZE,
	BONUS_PENBULLETS,
	BONUS_NUKE,
	BONUS_VITALITYROIDS,
	BONUS_AGILITYROIDS,
	BONUS_STRENGTHROIDS,
	BONUS_TELEPORTS
};

class Powerup: public StaticObject {
public:
	Powerup(float x, float y, Texture *tex);
	~Powerup();
	void *Object;
	BonusType Type;
	int Time;
	int Dir;
};
}

#endif /* POWERUP_H_ */
