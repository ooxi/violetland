#include "SDL_opengl.h"
#include "Bullet.h"

#ifndef LASERBULLET_H_
#define LASERBULLET_H_

class LaserBullet: public Bullet {
private:
	float Alpha;
public:
	LaserBullet(float x, float y);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
};

#endif /* LASERBULLET_H_ */