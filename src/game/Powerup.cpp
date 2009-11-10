#include "Powerup.h"

Powerup::Powerup(float x, float y, Texture *tex) :
	StaticObject(x, y, 128, 128, tex, false) {
	Time = 15000;
	resetTaking();
}

void Powerup::take(int deltaTime) {
	TakeDelay -= deltaTime;
	if (TakeDelay < 0)
		TakeDelay = 0;
}

void Powerup::resetTaking() {
	TakeDelay = 1700;
}
