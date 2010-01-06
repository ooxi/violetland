#include "Explosion.h"

Explosion::Explosion(float x, float y, int range, Texture* sparkTex,
		Texture* gruelTex, Sound* sound) {
	m_sound = sound;

	if (range < 100)
		range = 100;

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 15; i++) {
			Particle* gruel = new Particle(x + (rand() % range) - range * 0.5f,
					y + (rand() % (int) range) - range / 2.0, 128, 128,
					gruelTex);
			gruel->RMask = (float) (rand() % 30) / 100 + 0.3f;
			gruel->GMask = (float) (rand() % 30) / 100;
			gruel->BMask = 0.0f;
			gruel->AMask = (float) (rand() % 50) / 100 + 0.2f;
			gruel->Scale = (float) (rand() % (int) (range * 0.8f)) / range
					+ 0.2f;
			gruel->XSpeed = (float) ((rand() % 100) - 50) / 1000;
			gruel->YSpeed = (float) ((rand() % 100) - 50) / 1000;
			gruel->TSpeed = (float) ((rand() % 20) - 10) / 100;
			gruel->RMod = -0.0001;
			gruel->GMod = -0.0001;
			gruel->AMod = -0.0003;
			gruel->ScaleMod = 0.0001;
			m_particles.push_back(gruel);
		}
		for (int i = 0; i < 10; i++) {
			Particle* spark = new Particle(x + (rand() % (int) (range * 0.6f))
					- range * 0.3f, y + (rand() % (int) (range * 0.6f)) - range
					* 0.3f, 128, 128, sparkTex);
			spark->RMask = spark->GMask = 1.0f;
			spark->BMask = 0.6f;
			spark->AMask = (float) (rand() % 40) / 100 + 0.6f;
			spark->Scale = (float) (rand() % (int) (range * 0.4f)) / range;
			spark->XSpeed = (float) ((rand() % 250) - 125) / 1000;
			spark->YSpeed = (float) ((rand() % 250) - 125) / 1000;
			spark->AMod = -0.001;
			spark->ScaleMod = -0.0002;
			m_particles.push_back(spark);
		}
	}
	Particle* baseSpark = new Particle(x, y, 128, 128, sparkTex);
	baseSpark->RMask = baseSpark->GMask = 1.0f;
	baseSpark->BMask = 0.8f;
	baseSpark->AMask = 0.5f;
	baseSpark->Scale = range * 0.01f;
	baseSpark->AMod = -0.0003;
	m_particles.push_back(baseSpark);

	Active = true;
	Damage = 5.0f;
	Range = range;
	X = x;
	Y = y;

	sound->play(0, 0);
}

void Explosion::process(int deltaTime) {
	for (int i = m_particles.size() - 1; i >= 0; i--) {
		m_particles[i]->process(deltaTime);
		if (m_particles[i]->checkFinish()) {
			delete m_particles[i];
			m_particles.erase(m_particles.begin() + i);
		}
	}
}

void Explosion::draw() {
	for (unsigned int i = 0; i < m_particles.size(); i++) {
		m_particles[i]->draw(false, false);
	}
}

bool Explosion::isEmpty() {
	return m_particles.empty();
}

float Explosion::calcDamage(Object* refObj) {
	float d = Object::calculateDistance(X, Y, refObj->X, refObj->Y)
			- refObj->HitR * refObj->Scale * refObj->getWidth();
	if (d < Range) {
		return (Range - d) / Range * Damage;
	} else {
		return 0.0f;
	}
}

Explosion::~Explosion() {
	for (unsigned int i = 0; i < m_particles.size(); i++) {
		delete m_particles[i];
	}
	m_particles.clear();
}
