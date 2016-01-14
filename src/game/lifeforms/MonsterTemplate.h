#ifndef VIOLET_MONSTERTEMPLATE_H_
#define VIOLET_MONSTERTEMPLATE_H_

#include "../../system/graphic/Sprite.h"
#include "../../system/sound/Sound.h"

namespace violet {
class MonsterTemplate {
public:
	MonsterTemplate(Sprite* walkSprite, Sprite* deathSprite);
	~MonsterTemplate();
	std::vector<Sound*> HitSounds;
	Sprite* WalkSprite;
	Sprite* DeathSprite;
	std::string Name;
	float Strength;
	float Agility;
	float Vitality;
	int WalkTime;
	int WalkDelay;
};
}

#endif /* VIOLET_MONSTERTEMPLATE_H_ */
