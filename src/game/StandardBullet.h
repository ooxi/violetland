#include "SDL_opengl.h"
#include "Bullet.h"

#ifndef STANDARDBULLET_H_
#define STANDARDBULLET_H_

class StandardBullet: public Bullet {
private:
	float Alpha;
public:
	StandardBullet(float x, float y);
	virtual void process(int deltaTime);
	virtual void draw();
	~StandardBullet();
};

#endif /* STANDARDBULLET_H_ */