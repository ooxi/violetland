#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "ParticleSystem.h"

class Explosion: public ParticleSystem {
public:
	Explosion(bool nuclear, float x, float y, int range, Texture* sparkTex,
			Texture* gruelTex);
};

#endif /* EXPLOSION_H_ */
