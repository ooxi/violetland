#ifndef FLAME_H_
#define FLAME_H_

#include "Bullet.h"
#include "../../system/graphic/Texture.h"
#include "../../system/graphic/StaticObject.h"

namespace violetland {

class Flame: public Bullet {
private:
	StaticObject* m_img;
public:
	Flame(float x, float y, Texture* tex);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
	virtual ~Flame();
};

}

#endif /* FLAME_H_ */
