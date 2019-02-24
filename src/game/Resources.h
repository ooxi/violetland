#ifndef VIOLET_RESOURCES_H_
#define VIOLET_RESOURCES_H_

#include <sstream>

#include <boost/format.hpp>

#include "../system/graphic/Sprite.h"
#include "../system/sound/SoundManager.h"
#include "../system/utility/Templates.h"
#include "Powerup.h"

namespace violet {
class Resources {
private:
	FileUtility* m_fileUtil;
	SoundManager* m_sndManager;
	void loadPowerupTex(BonusType type, std::string fileName);
	Texture* loadTex(std::string fileName);
	Sound* loadSnd(std::string fileName);
	Sprite* loadSprite(unsigned int frames, std::string pattern);
public:
	Resources(FileUtility* fileUtility, SoundManager* sndManager);
	Sprite* PlayerWalkSprite;
	Sprite* PlayerShieldSprite;
	std::vector<Sprite*> PlayerDeathSprites;
	Sound* PlayerDeathSound;
	std::vector<Sound*> PlayerHitSounds;
	std::vector<Sound*> ExplSounds;
	std::map<BonusType, Texture*> PowerupTex;
	Sprite* GrenadeSprite;
	std::vector<Texture*> BloodTex;
	std::vector<Texture*> ExplTex;
	StaticObject* Crystal;
	StaticObject* HealthIndicator;
	StaticObject* LevelUpIndicator;
	~Resources();
};
}

#endif /* VIOLET_RESOURCES_H_ */
