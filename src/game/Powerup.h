#include "./../system/StaticObject.h"

#ifndef POWERUP_H_
#define POWERUP_H_

class Powerup: public StaticObject {
public:
	enum PowerupType {
		medikit, weapon
	};
	Powerup(float x, float y, Texture *tex);
	void *Object;
	PowerupType Type;
	int Time;
};

#endif /* POWERUP_H_ */
