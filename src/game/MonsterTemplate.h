#ifndef MONSTERTEMPLATE_H_
#define MONSTERTEMPLATE_H_

#include "../system/graphic/Sprite.h"
#include "../system/sound/Sound.h"

class MonsterTemplate {
public:
	MonsterTemplate(Sprite* walkSprite, Sprite* deathSprite, Sound* hitSound);
	~MonsterTemplate();
	Sound* HitSound;
	Sprite* WalkSprite;
	Sprite* DeathSprite;
	float Strength;
	float Agility;
	float Vitality;
};

#endif /* MONSTERTEMPLATE_H_ */
