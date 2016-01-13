#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "ParticleSystem.h"

namespace violet {

class Explosion: public ParticleSystem {
public:
	Explosion(bool nuclear, float x, float y, int range, Texture* sparkTex,
			Texture* gruelTex);
	virtual ~Explosion();
};
}

#endif /* EXPLOSION_H_ */
