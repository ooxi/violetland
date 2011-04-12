#ifndef MONSTERFACTORY_H_
#define MONSTERFACTORY_H_

#include <sstream>

#include <boost/filesystem/fstream.hpp>

#include "../../system/sound/SoundManager.h"
#include "Monster.h"

namespace violetland {
class MonsterFactory {
private:
	FileUtility* m_fileUtility;
	SoundManager* m_sndManager;
	std::vector<MonsterTemplate*> m_monsters;
	Sprite* loadMonsterSprite(std::string name, std::string animType);
	Sound* loadMonsterSound(std::string soundType, std::string monsterName,
			std::string soundName);
	void fillMonsterStats(MonsterTemplate* t, std::string name);
public:
	MonsterFactory(FileUtility* fileUtility, SoundManager* sndManager);
	Monster* create(int baseLvl, int lvl);
	~MonsterFactory();
};
}

#endif /* MONSTERFACTORY_H_ */
