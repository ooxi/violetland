#include <string>
#include "../../system/Object.h"

#ifndef BULLET_H_
#define BULLET_H_

class Bullet: public Object {
protected:
	bool m_active, m_readyToRemove;
	float startX, startY;
	float dStartX, dStartY;
	float m_range;
public:
	enum BulletType {
		standard = 0, laser, grenade
	};
	Bullet(float x, float y, float dX, float dY, BulletType type);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
	bool isActive();
	bool isReadyToRemove();
	void deactivate();
	float Damage;
	float MaxRange;
	Bullet::BulletType Type;
	std::string OwnerId;

	bool Poisoned;
	bool BigCalibre;

	virtual ~Bullet();
};

#endif /* BULLET_H_ */
