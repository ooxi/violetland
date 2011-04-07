#ifndef GRENADEBULLET_H_
#define GRENADEBULLET_H_

#ifdef _WIN32
#define _USE_MATH_DEFINES
#endif //_WIN32W
#include "../../system/graphic/DynamicObject.h"
#include "Bullet.h"

namespace violetland {

class GrenadeBullet: public Bullet {
private:
	float m_targetX, m_targetY;
	float m_distance;
	float m_scale;
	DynamicObject* m_image;
public:
	GrenadeBullet(float x, float y, float targetX, float targetY,
			Sprite* sprite);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual ~GrenadeBullet();
};

}

#endif /* GRENADEBULLET_H_ */
