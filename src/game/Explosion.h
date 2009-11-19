#include <vector>
#include "../system/graphic/Particle.h"
#include "../system/graphic/Texture.h"

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

class Explosion {
private:
	vector<Particle*> m_particles;
public:
	Explosion(float x, float y, Texture* sparkTex, Texture* gruelTex);
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
