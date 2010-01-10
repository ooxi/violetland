#include "MonsterTemplate.h"

MonsterTemplate::MonsterTemplate(Sprite* walkSprite, Sprite* deathSprite,
		Sound* hitSound) {
	WalkSprite = walkSprite;
	DeathSprite = deathSprite;
	HitSound = hitSound;
}

MonsterTemplate::~MonsterTemplate() {
	delete WalkSprite;
	delete DeathSprite;
	delete HitSound;
}
