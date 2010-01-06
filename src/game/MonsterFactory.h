#ifndef MONSTERFACTORY_H_
#define MONSTERFACTORY_H_

#include "../system/graphic/Sprite.h"
#include "../system/sound/SoundManager.h"
#include "../system/sound/Sound.h"
#include "../system/utility/FileUtility.h"
#include "Enemy.h"

class MonsterFactory {
private:
	FileUtility* m_fileUtility;
	SoundManager* m_sndManager;
	vector<Sprite*> m_moveSprites;
	vector<Sprite*> m_deathSprites;
	vector<Sound*> m_hitSounds;
	Sprite* m_bleedSprite;
	Sprite* loadMonsterSprite(unsigned int index, string animType);
	Sound* loadMonsterSound(unsigned int index);
public:
	MonsterFactory(FileUtility* fileUtility, SoundManager* sndManager);
	Enemy* create(int baseLvl, int lvl, float* param);
	~MonsterFactory();
};

#endif /* MONSTERFACTORY_H_ */
