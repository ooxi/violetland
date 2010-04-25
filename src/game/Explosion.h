#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "../system/graphic/ParticleSystem.h"
#include "../system/sound/Sound.h"

class Explosion: public ParticleSystem {
private:
	Sound* m_sound;
public:
	Explosion(float x, float y, int range, float damage, Texture* sparkTex,
			Texture* gruelTex, Sound* sound);
	float calcDamage(Object* objRef);
	float X, Y;
	float Damage;
	float Range;
	bool Active;
};

#endif /* EXPLOSION_H_ */
