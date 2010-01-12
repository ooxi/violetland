#include "MonsterTemplate.h"

MonsterTemplate::MonsterTemplate(Sprite* walkSprite, Sprite* deathSprite,
		Sound* hitSound) {
	WalkSprite = walkSprite;
	DeathSprite = deathSprite;
	HitSound = hitSound;
	Strength = 1.0f;
	Agility = 1.0f;
	Vitality = 1.0f;
}

MonsterTemplate::~MonsterTemplate() {
	delete WalkSprite;
	delete DeathSprite;
	delete HitSound;
}
