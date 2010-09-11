#include "MonsterTemplate.h"

violetland::MonsterTemplate::MonsterTemplate(Sprite* walkSprite,
		Sprite* deathSprite) {
	WalkSprite = walkSprite;
	DeathSprite = deathSprite;
	Strength = 1.0f;
	Agility = 1.0f;
	Vitality = 1.0f;
}

violetland::MonsterTemplate::~MonsterTemplate() {
	delete WalkSprite;
	delete DeathSprite;
	for (unsigned int i = 0; i < HitSounds.size(); i++) {
		delete HitSounds[i];
	}
	HitSounds.clear();
}
