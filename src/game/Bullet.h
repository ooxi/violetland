#include "./../system/Object.h"

#ifndef BULLET_H_
#define BULLET_H_

class Bullet : public Object {
protected:
	bool m_active, m_readyToRemove;
	float startX, startY;
	float m_range;
public:
	enum BulletType {
		standard
	};
	Bullet(float x, float y, BulletType type);
	virtual void process(int deltaTime) {};
	virtual void draw() {};
	bool isActive();
	bool isReadyToRemove();
	void deactivate();
	float Damage;
	float MaxRange;
	bool Poisoned;
	BulletType Type;
};

#endif /* BULLET_H_ */
