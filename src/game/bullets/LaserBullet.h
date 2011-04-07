#ifndef LASERBULLET_H_
#define LASERBULLET_H_

#include "SDL_opengl.h"
#include "Bullet.h"

namespace violetland {

class LaserBullet: public Bullet {
private:
	float Alpha;
	bool m_hit;
public:
	LaserBullet(float x, float y, float dX, float dY);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
};

}

#endif /* LASERBULLET_H_ */
