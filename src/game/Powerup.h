#include "../system/graphic/StaticObject.h"

#ifndef POWERUP_H_
#define POWERUP_H_

class Powerup: public StaticObject {
public:
	enum PowerupType {
		medikit = 0, weapon
	};
	Powerup(float x, float y, Texture *tex);
	void *Object;
	void resetTaking();
	void take(int deltaTime);
	Powerup::PowerupType Type;
	int Time;
	int TakeDelay;
};

#endif /* POWERUP_H_ */
