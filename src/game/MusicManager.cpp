#include "MusicManager.h"

const std::string DEFAULT = "04.ogg";

violetland::MusicManager::MusicManager(FileUtility* fileUtility,
		SoundManager* soundManager, Configuration* config) {
	std::cout << "MusicManager..." << std::endl;
	m_fileUtility = fileUtility;
	m_soundManager = soundManager;
	m_config = config;

	std::vector<std::string> musicFiles = m_fileUtility->getFilesFromDir(
			m_fileUtility->getFullPath(FileUtility::music, "."));

	for (unsigned int i = 0; i < musicFiles.size(); i++) {
		Sound* snd = m_soundManager->create(m_fileUtility->getFullPath(
				FileUtility::music, musicFiles[i]));
		m_music.insert(std::map<std::string, Sound*>::value_type(musicFiles[i],
				snd));
	}

	std::cout << "\tloaded " << musicFiles.size() << " tracks" << std::endl;

	m_currentPlaying = "null";
}

void violetland::MusicManager::process(Player* player, GameState* gameState) {
	if (gameState->Paused) {
		play();
		return;
	}
	bool afterPause = m_currentPlaying == DEFAULT;
	if (player->getHealth() / player->MaxHealth() < 0.4f) {
		play(0, "03.ogg", afterPause);
	} else if (gameState->Time < 100000) {
		play(0, "05.ogg", afterPause);
	} else if (player->getWeapon()->Name == "Laser") {
		play(0, "02.ogg", afterPause);
	} else {
		play(0, "01.ogg", afterPause);
	}
}

void violetland::MusicManager::play() {
	play(0, DEFAULT, false);
}

void violetland::MusicManager::play(int chan, std::string name, bool now) {
	if (m_music.size() > 0) {
		if (m_currentPlaying != "null" && m_currentPlaying != name) {
			if (m_music[m_currentPlaying]->isPlaying()) {
				m_music[m_currentPlaying]->stop(3000);
			}
		}
		
		if (m_currentPlaying != name) {
			std::map<std::string, Sound*>::iterator it = m_music.find(name);
			if (it != m_music.end()) {
				it->second->play(chan, 3000, -1);
				it->second->setVol(m_config->MusicVolume * 12);
				m_currentPlaying = name;
			}
		}
	}
}

violetland::MusicManager::~MusicManager() {
	std::map<std::string, Sound*>::const_iterator iter;
	for (iter = m_music.begin(); iter != m_music.end(); ++iter) {
		delete iter->second;
	}
	m_music.clear();
}
