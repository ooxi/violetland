#ifndef MONSTERFACTORY_H_
#define MONSTERFACTORY_H_

#include "../system/sound/SoundManager.h"
#include "../system/utility/FileUtility.h"
#include "Enemy.h"
#include "MonsterTemplate.h"

class MonsterFactory {
private:
	FileUtility* m_fileUtility;
	SoundManager* m_sndManager;
	vector<MonsterTemplate*> m_monsters;
	Sprite* loadMonsterSprite(string name, string animType);
	Sound* loadMonsterSound(string soundType, string monsterName,
			string soundName);
	void fillMonsterStats(MonsterTemplate* t, string name);
public:
	MonsterFactory(FileUtility* fileUtility, SoundManager* sndManager);
	Enemy* create(int baseLvl, int lvl);
	~MonsterFactory();
};

#endif /* MONSTERFACTORY_H_ */
