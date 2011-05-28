#include "MusicManager.h"

const std::string DEFAULT = "04.ogg";

violetland::MusicManager::MusicManager(FileUtility* fileUtility,
		SoundManager* soundManager, Configuration* config) {
	std::cout << "MusicManager..." << std::endl;
	m_fileUtility = fileUtility;
	m_soundManager = soundManager;
	m_config = config;

	m_currentPlaying = "null";
}

void violetland::MusicManager::process(Player* player, GameState* gameState) {
	if (gameState->Paused) {
		play();
		return;
	}
	bool afterPause = m_currentPlaying == DEFAULT;
	if (player->getHealth() / player->MaxHealth() < 0.4f) {
		play("03.ogg", afterPause);
	} else if (gameState->Time < 100000) {
		play("05.ogg", afterPause);
	} else if (player->getWeapon()->Name == "Laser") {
		play("02.ogg", afterPause);
	} else {
		play("01.ogg", afterPause);
	}
}

void violetland::MusicManager::play() {
	play(DEFAULT, false);
}

void violetland::MusicManager::play(std::string name, bool now) {
	if (m_currentPlaying == name)
		return;
	
	if (m_currentPlaying != "null")
		Mix_FreeMusic(m_current);
	
	m_currentPlaying = name;
	m_current = Mix_LoadMUS(m_fileUtility->getFullPath(FileUtility::music, name).string().c_str());
	if(m_current) 
	{
		if(Mix_PlayMusic(m_current, -1) == -1)
			cout << "Mix_PlayMusic: " << Mix_GetError() << endl;
	}
	else
		cout << "Mix_LoadMUS: " << Mix_GetError() << endl;
}

violetland::MusicManager::~MusicManager() {
	if (m_currentPlaying != "null" && m_current)
		Mix_FreeMusic(m_current);
}
