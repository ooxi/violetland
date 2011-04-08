#ifndef ENEMY_H_
#define ENEMY_H_

#include "../../system/graphic/DynamicObject.h"
#include "../bullets/Bullet.h"
#include "LifeForm.h"
#include "MonsterTemplate.h"

using namespace std;

namespace violetland {
class Monster: public LifeForm {
private:
	int m_hitSoundChannel;
	int m_bleedDelay;
	int m_bleedCount;
public:
	Monster(MonsterTemplate* base, int lvl);
	virtual void process(int deltaTime);
	virtual void draw();
	virtual Sound* hit(float damage, bool poison, bool stop);

	void setAppearance(float scale, float rMask, float gMask, float bMask,
			float aMask);
	void rollFrame(bool forward);
	bool isBleeding();
	virtual StaticObject* getCorpse();
	void destroy();
	virtual ~Monster();
	bool Angry;
	string targetId;
	MonsterTemplate* Base;
};
}

#endif /* ENEMY_H_ */
