#include "Powerup.h"

violetland::Powerup::Powerup(float x, float y, Texture *tex) :
	StaticObject(x, y, 128, 128, tex, false) {
	Time = 15000;
	Dir = 1;
}

violetland::Powerup::~Powerup() {
	if(Type != BONUS_WEAPON)
		delete Object;
}
