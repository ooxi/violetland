#ifndef BULLET_H_
#define BULLET_H_

#include <string>
#include "../../system/Object.h"

namespace violetland {

enum BulletType {
	BULLET_STANDARD = 0, BULLET_LASER, BULLET_GRENADE, BULLET_FLAME
};

class Bullet: public Object {
protected:
	bool m_active, m_readyToRemove;
	float startX, startY;
	float dStartX, dStartY;
	float m_range;
public:
	Bullet(float x, float y, float dX, float dY, BulletType type);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
	bool isActive();
	bool isReadyToRemove();
	void deactivate();
	float Damage;
	float MaxRange;
	BulletType Type;
	std::string OwnerId;

	bool Poisoned;
	bool BigCalibre;
	bool Penetrating;

	virtual ~Bullet();
};

}

#endif /* BULLET_H_ */
