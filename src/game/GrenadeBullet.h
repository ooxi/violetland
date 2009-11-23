#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include <cmath>
#include <stdlib.h>
#include "SDL_opengl.h"
#include "Bullet.h"

#ifndef GRENADEBULLET_H_
#define GRENADEBULLET_H_

class GrenadeBullet: public Bullet {
private:
	float m_targetX, m_targetY;
	float m_distance;
public:
	GrenadeBullet(float x, float y, float targetX, float targetY);
	virtual void process(int deltaTime);
	virtual void draw();
	~GrenadeBullet();
};

#endif /* GRENADEBULLET_H_ */
