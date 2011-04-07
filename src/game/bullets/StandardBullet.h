#ifndef STANDARDBULLET_H_
#define STANDARDBULLET_H_

#include "SDL_opengl.h"
#include "Bullet.h"

namespace violetland {

class StandardBullet: public Bullet {
private:
	float Alpha;
	float prevX, prevY;
	bool m_explosive;
public:
	StandardBullet(float x, float y, float dX, float dY, bool explosive);
	bool isExplosive();
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
};

}

#endif /* STANDARDBULLET_H_ */
