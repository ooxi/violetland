#ifndef MUSICMANAGER_H_
#define MUSICMANAGER_H_

#include "../system/sound/SoundManager.h"
#include "GameState.h"
#include "Player.h"

class MusicManager {
private:
	FileUtility* m_fileUtility;
	SoundManager* m_soundManager;
	Configuration* m_config;
	std::map<std::string, Sound*> m_music;
	std::string m_currentPlaying;
public:
	MusicManager(FileUtility* fileUtility, SoundManager* soundManager,
			Configuration* config);
	void process(Player* player, GameState* gameState);
	void play();
	void play(int chan, std::string name, bool now);
	~MusicManager();
};

#endif /* MUSICMANAGER_H_ */
