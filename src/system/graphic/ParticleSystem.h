#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>
#include "Particle.h"

class ParticleSystem {
public:
	ParticleSystem();
	virtual void process(int deltaTime);
	virtual void draw();
	bool isEmpty();
	std::vector<Particle*> Particles;
	virtual ~ParticleSystem();
};

#endif /* PARTICLESYSTEM_H_ */
