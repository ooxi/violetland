#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include <vector>
#include "Particle.h"

class ParticleSystem {
public:
	ParticleSystem();
	void process(int deltaTime);
	void draw();
	bool isEmpty();
	std::vector<Particle*> Particles;
	~ParticleSystem();
};

#endif /* PARTICLESYSTEM_H_ */
