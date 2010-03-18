#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "../system/graphic/Sprite.h"
#include "../system/sound/SoundManager.h"
#include "Powerup.h"

class Resources {
private:
	FileUtility* m_fileUtil;
	SoundManager* m_sndManager;
	void loadPowerupTex(Powerup::PowerupType type, std::string fileName);
	Texture* loadTex(std::string fileName);
	Sound* loadSnd(std::string fileName);
	Sprite* loadSprite(unsigned int frames, std::string pattern);
public:
	Resources(FileUtility* fileUtility, SoundManager* sndManager);
	Sprite* PlayerWalkSprite;
	std::vector<Sprite*> PlayerDeathSprites;
	Sound* PlayerDeathSound;
	std::vector<Sound*> PlayerHitSounds;
	std::vector<Sound*> ExplSounds;
	std::map<Powerup::PowerupType, Texture*> PowerupTex;
	Sprite* GrenadeSprite;
	std::vector<Texture*> BloodTex;
	std::vector<Texture*> ExplTex;
	StaticObject* Crystal;
	StaticObject* HealthIndicator;
	StaticObject* LevelUpIndicator;

	~Resources();
};

#endif /* RESOURCES_H_ */
