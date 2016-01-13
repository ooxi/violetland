#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "StaticObject.h"

namespace violet {

class Particle: public StaticObject {
public:
	Particle(float x, float y, int w, int h, Texture* tex);
	void process(int deltaTime);
	bool checkFinish();
	float XSpeed, YSpeed, TSpeed;
	float ScaleMod, RMod, GMod, BMod, AMod;
};
}

#endif /* PARTICLE_H_ */
