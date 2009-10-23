#include "./../system/StaticObject.h"
#include "./../system/Texture.h"

#ifndef BULLET_H_
#define BULLET_H_

class Bullet: public StaticObject {
private:
	float m_range;
	Texture *m_tex;
public:
	Bullet(float x, float y, Texture *texture);
	void process(int deltaTime);
	Texture *getTextureRef();
	bool isFall();
	float Damage;
	float MaxRange;
	bool Poisoned;
};

#endif /* BULLET_H_ */
