#include "Powerup.h"

Powerup::Powerup(float x, float y, Texture *tex) :
	StaticObject(x, y, 128, 128, tex, false) {
	Time = 15000;
	Dir = 1;
}
