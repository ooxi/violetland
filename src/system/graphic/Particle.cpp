#include "Particle.h"

Particle::Particle(float x, float y, int w, int h, Texture* tex) :
	StaticObject(x, y, w, h, tex, false) {
	XSpeed = YSpeed = TSpeed = 0.0f;
	ScaleMod = 0.0f;
	RMod = GMod = BMod = AMod = 0.0f;
}

void Particle::process(int deltaTime) {
	X += XSpeed * deltaTime;
	Y += YSpeed * deltaTime;
	Scale += ScaleMod * deltaTime;
	RMask += RMod * deltaTime;
	if (RMask < 0)
		RMask = 0;
	GMask += GMod * deltaTime;
	if (GMask < 0)
		GMask = 0;
	BMask += BMod * deltaTime;
	if (BMask < 0)
		BMask = 0;
	AMask += AMod * deltaTime;
	Angle = Object::fixAngle(Angle + TSpeed * deltaTime);
}

bool Particle::checkFinish() {
	return Scale < 0 || AMask < 0;
}
