#include "../system/graphic/StaticObject.h"

#ifndef POWERUP_H_
#define POWERUP_H_

class Powerup: public StaticObject {
public:
	enum PowerupType {
		medikit = 0, weapon, grenades, freeze
	};
	Powerup(float x, float y, Texture *tex);
	void *Object;
	Powerup::PowerupType Type;
	int Time;
	int Dir;
};

#endif /* POWERUP_H_ */
