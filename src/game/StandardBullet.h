#include "SDL_opengl.h"
#include "Bullet.h"

#ifndef STANDARDBULLET_H_
#define STANDARDBULLET_H_

class StandardBullet: public Bullet {
private:
	float Alpha;
	float prevX, prevY;
public:
	StandardBullet(float x, float y);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
};

#endif /* STANDARDBULLET_H_ */
