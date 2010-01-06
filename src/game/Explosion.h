#include <vector>
#include "../system/graphic/Particle.h"
#include "../system/graphic/Texture.h"
#include "../system/sound/Sound.h"

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

class Explosion {
private:
	vector<Particle*> m_particles;
	Sound* m_sound;
public:
	Explosion(float x, float y, int range, Texture* sparkTex,
			Texture* gruelTex, Sound* sound);
	void process(int deltaTime);
	void draw();
	float calcDamage(Object* objRef);
	bool isEmpty();
	~Explosion();
	float Damage;
	float Range;
	float X, Y;
	bool Active;
};

#endif /* EXPLOSION_H_ */
