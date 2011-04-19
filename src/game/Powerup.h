#ifndef POWERUP_H_
#define POWERUP_H_

#include <sstream>

#include <boost/format.hpp>

//#include "Game.h"
#include "Weapon.h"
#include "lifeforms/Player.h"
#include "../system/graphic/StaticObject.h"
#include "../system/InputHandler.h"


namespace violetland {
class Game;
	
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

class BasePowerup: public StaticObject {
	const std::string m_hudInfo;
public:
	BasePowerup(float x, float y, Texture *tex);
	
	virtual const std::string getHudInfo() const = 0;
	virtual BonusType getType() const = 0;
	virtual bool modify(Game* game, Player* player) = 0;
	void process(int timeDelta);
	int Time;
	int Dir;
};

/*class Powerup: public BasePowerup {
	const std::string m_hudInfo;
	const BonusType m_type;
public:
	Powerup(float x, float y, Texture *tex, std::string hudInfo, 
				BonusType type, void* object): 
				BasePowerup(x, y, tex), m_hudInfo(hudInfo), 
				m_type(type), Object(object)
				{}
	~Powerup();
	const std::string getHudInfo() const {
		return m_hudInfo;
	}
	BonusType getType() const {
		return m_type;
	}
	void *Object;
};*/

class MedikitPowerup: public BasePowerup {
	const float m_value;
public:
	MedikitPowerup(float x, float y, Texture *tex, float value): 
				BasePowerup(x, y, tex), m_value(value)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_MEDIKIT;
	}
	bool modify(Game* game, Player* player);
};

class GrenadePowerup: public BasePowerup {
public:
	GrenadePowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_GRENADES;
	}
	bool modify(Game* game, Player* player);
};


class FreezePowerup: public BasePowerup {
public:
	FreezePowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_FREEZE;
	}
	bool modify(Game* game, Player* player);
};

class NukePowerup: public BasePowerup {
public:
	NukePowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_NUKE;
	}
	bool modify(Game* game, Player* player);
};

class PenetrationBulletsPowerup: public BasePowerup {
public:
	PenetrationBulletsPowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_PENBULLETS;
	}
	bool modify(Game* game, Player* player);
};


class VitalityPowerup: public BasePowerup {
public:
	VitalityPowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_VITALITYROIDS;
	}
	bool modify(Game* game, Player* player);
};

class StrengthPowerup: public BasePowerup {
public:
	StrengthPowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_STRENGTHROIDS;
	}
	bool modify(Game* game, Player* player);
};

class AgilityPowerup: public BasePowerup {
public:
	AgilityPowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_AGILITYROIDS;
	}
	bool modify(Game* game, Player* player);
};

class TeleportPowerup: public BasePowerup {
public:
	TeleportPowerup(float x, float y, Texture *tex): 
				BasePowerup(x, y, tex)
				{}
	const std::string getHudInfo() const;
	BonusType getType() const {
		return BONUS_TELEPORTS;
	}
	bool modify(Game* game, Player* player);
};


class WeaponPowerup: public BasePowerup {
public:
	Weapon* weapon;

	WeaponPowerup(float x, float y, Weapon* weapon): 
		BasePowerup(x, y, weapon->getDroppedTex()), weapon(weapon) {}
		
	const std::string getHudInfo() const {
		return weapon->Name;
	}
	BonusType getType() const {
		return BONUS_WEAPON;
	}
	
	bool modify(Game* game, Player* player);
};

}

#endif /* POWERUP_H_ */
