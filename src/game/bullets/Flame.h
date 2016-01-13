#ifndef VIOLET_FLAME_H_
#define VIOLET_FLAME_H_

#include "Bullet.h"
#include "../../system/graphic/Texture.h"
#include "../../system/graphic/StaticObject.h"

namespace violet {

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

#endif /* VIOLET_FLAME_H_ */
