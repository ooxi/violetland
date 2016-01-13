#ifndef VIOLET_PARTICLESYSTEM_H_
#define VIOLET_PARTICLESYSTEM_H_

#include <vector>
#include "Particle.h"

namespace violet {

class ParticleSystem {
public:
	ParticleSystem();
	virtual void process(int deltaTime);
	virtual void draw();
	bool isEmpty();
	std::vector<Particle*> Particles;
	virtual ~ParticleSystem();
};
}

#endif /* VIOLET_PARTICLESYSTEM_H_ */
