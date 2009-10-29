#include "MusicManager.h"

MusicManager::MusicManager(FileUtility* fileUtility, SoundManager* soundManager) {
	printf("MusicManager...\n");
	m_fileUtility = fileUtility;
	m_soundManager = soundManager;
	m_music = m_soundManager->create("../music/Dzaibatsu2.ogg");
}

void MusicManager::process(Player* player, std::vector<Enemy*>* enemies,
		int gameTime) {
	process();
}

void MusicManager::process() {
	if (!m_music->isPlaying()) {
		m_music->play();
	}
}

MusicManager::~MusicManager() {
	delete m_music;
}
