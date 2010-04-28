#ifndef ENEMY_H_
#define ENEMY_H_

#include "../system/graphic/DynamicObject.h"
#include "LifeForm.h"
#include "bullets/Bullet.h"
#include "MonsterTemplate.h"

class Enemy: public LifeForm {
private:
	DynamicObject* m_body;
	int m_hitSoundChannel;
	int m_bleedDelay;
	int m_bleedCount;
public:
	Enemy(MonsterTemplate* base, int lvl);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual void hit(float damage, bool poison, float pX, float pY);

	void setAppearance(float scale, float rMask, float gMask, float bMask,
			float aMask);
	void rollFrame(bool forward);
	bool isBleeding();
	virtual StaticObject* getCorpse();
	void destroy();
	~Enemy();
	bool DoNotDisturb;
	bool Angry;
	MonsterTemplate* Base;
};

#endif /* ENEMY_H_ */
