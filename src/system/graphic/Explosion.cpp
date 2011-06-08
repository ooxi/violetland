#include "Explosion.h"

Explosion::Explosion(bool nuclear, float x, float y, int range,
		Texture* sparkTex, Texture* gruelTex) :
	ParticleSystem() {

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
			gruel->RMod = -0.0001f;
			gruel->GMod = -0.0001f;
			gruel->AMod = -0.0003f;
			gruel->ScaleMod = 0.0001f;
			Particles.push_back(gruel);
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
			spark->AMod = -0.001f;
			spark->ScaleMod = -0.0002f;
			Particles.push_back(spark);
		}
	}

	if (nuclear) {
		Particle* baseSpark = new Particle(x, y, 128, 128, sparkTex);
		baseSpark->RMask = 0.8f;
		baseSpark->BMask = 0.6f;
		baseSpark->GMask = 1.0f;
		baseSpark->AMask = 0.8f;
		baseSpark->Scale = range * 0.02f;
		baseSpark->AMod = -0.0002f;
		baseSpark->ScaleMod = -0.0002f;
		Particles.push_back(baseSpark);
	} else {
		Particle* baseSpark = new Particle(x, y, 128, 128, sparkTex);
		baseSpark->RMask = baseSpark->GMask = 1.0f;
		baseSpark->BMask = 0.8f;
		baseSpark->AMask = 0.5f;
		baseSpark->Scale = range * 0.01f;
		baseSpark->AMod = -0.0003f;
		Particles.push_back(baseSpark);
	}
}
