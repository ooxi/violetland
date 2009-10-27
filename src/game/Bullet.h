#include "./../system/Object.h"

#ifndef BULLET_H_
#define BULLET_H_

class Bullet: public Object {
protected:
	bool m_active, m_readyToRemove;
	float startX, startY;
	float m_range;
public:
	enum BulletType {
		standard = 0, laser
	};
	Bullet(float x, float y, BulletType type);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual bool checkHit(Object* objRef);
	bool isActive();
	bool isReadyToRemove();
	void deactivate();
	float Damage;
	float MaxRange;
	bool Poisoned;
	Bullet::BulletType Type;
	virtual ~Bullet();
};

#endif /* BULLET_H_ */
