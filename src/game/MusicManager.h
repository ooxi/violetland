#include <map>
#include <string>
#include "../system/utility/FileUtility.h"
#include "../system/sound/SoundManager.h"
#include "Enemy.h"
#include "Player.h"

#ifndef MUSICMANAGER_H_
#define MUSICMANAGER_H_

class MusicManager {
private:
	FileUtility* m_fileUtility;
	SoundManager* m_soundManager;
	std::map<std::string, Sound*> m_music;
public:
	MusicManager(FileUtility* fileUtility, SoundManager* soundManager);
	void process(Player* player, std::vector<Enemy*>* enemies, int gameTime);
	void process();
	~MusicManager();
};

#endif /* MUSICMANAGER_H_ */
