#include "MusicManager.h"

const std::string DEFAULT = "04.ogg";

MusicManager::MusicManager(FileUtility* fileUtility,
		SoundManager* soundManager, Configuration* config) {
	printf("MusicManager...\n");
	m_fileUtility = fileUtility;
	m_soundManager = soundManager;
	m_config = config;

	std::vector<std::string> musicFiles = m_fileUtility->getFilesFromDir(
			m_fileUtility->getFullPath(FileUtility::music, "."));

	for (unsigned int i = 0; i < musicFiles.size(); i++) {
		Sound* snd = m_soundManager->create(m_fileUtility->getFullPath(
				FileUtility::music, musicFiles[i]));
		m_music .insert(std::map<std::string, Sound*>::value_type(
				musicFiles[i], snd));
	}

	fprintf(stdout, "\tloaded %i tracks\n", (int) musicFiles.size());

	m_currentPlaying = "null";
}

void MusicManager::process(Player* player, std::vector<LifeForm*> enemies,
		bool paused) {
	if (paused) {
		play();
		return;
	}
	bool afterPause = m_currentPlaying == DEFAULT;
	if (player->getHealth() / player->MaxHealth() < 0.4f) {
		play(0, "03.ogg", afterPause);
	} else if (player->Time < 100000) {
		play(0, "05.ogg", afterPause);
	} else if (player->getWeapon()->Name == "Laser") {
		play(0, "02.ogg", afterPause);
	} else {
		play(0, "01.ogg", afterPause);
	}
}

void MusicManager::play() {
	play(0, DEFAULT, false);
}

void MusicManager::play(int chan, std::string name, bool now) {
	if (m_music.size() > 0) {
		if (m_currentPlaying != "null" && m_currentPlaying != name) {
			if (m_music[m_currentPlaying]->isPlaying()) {
				m_music[m_currentPlaying]->stop(3000);
			}
		}
		if (m_currentPlaying != name && m_music.count(name) > 0) {
			m_music[name]->play(chan, 3000, -1);
			m_music[name]->setVol(m_config->MusicVolume * 12);
			m_currentPlaying = name;
		}
	}
}

MusicManager::~MusicManager() {
	std::map<std::string, Sound*>::const_iterator iter;
	for (iter = m_music.begin(); iter != m_music.end(); ++iter) {
		delete iter->second;
	}
	m_music.clear();
}
