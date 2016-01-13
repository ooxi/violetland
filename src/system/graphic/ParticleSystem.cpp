#include "ParticleSystem.h"

violet::ParticleSystem::ParticleSystem() {
}

void violet::ParticleSystem::process(int deltaTime) {
	for (int i = Particles.size() - 1; i >= 0; i--) {
		Particles[i]->process(deltaTime);
		if (Particles[i]->checkFinish()) {
			delete Particles[i];
			Particles.erase(Particles.begin() + i);
		}
	}
}

void violet::ParticleSystem::draw() {
	for (unsigned int i = 0; i < Particles.size(); i++) {
		Particles[i]->draw(false, false);
	}
}

bool violet::ParticleSystem::isEmpty() {
	return Particles.empty();
}

violet::ParticleSystem::~ParticleSystem() {
	for (unsigned int i = 0; i < Particles.size(); i++) {
		delete Particles[i];
	}
	Particles.clear();
}
