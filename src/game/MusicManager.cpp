#include "MusicManager.h"

MusicManager::MusicManager(FileUtility* fileUtility, SoundManager* soundManager) {
	printf("MusicManager...\n");
	m_fileUtility = fileUtility;
	m_soundManager = soundManager;

	std::vector<std::string> musicFiles = m_fileUtility->getFilesFromDir(
			m_fileUtility->getFullPath(FileUtility::music, ""));

	for (unsigned int i = 0; i < musicFiles.size(); i++) {
		m_music.insert(std::map<std::string, Sound*>::value_type(musicFiles[i],
				m_soundManager->create(m_fileUtility->getFullPath(
						FileUtility::music, musicFiles[i]))));
	}

	fprintf(stdout, "\tfound %i tracks\n", (int) musicFiles.size());
}

void MusicManager::process(Player* player, std::vector<Enemy*>* enemies,
		int gameTime) {
	process();
}

void MusicManager::process() {
	if (!m_music.empty())
		if (!m_music["Dzaibatsu2.ogg"]->isPlaying()) {
			m_music["Dzaibatsu2.ogg"]->play();
		}
}

MusicManager::~MusicManager() {
	std::map<std::string, Sound*>::const_iterator iter;
	for (iter = m_music.begin(); iter != m_music.end(); ++iter) {
		delete iter->second;
	}

	m_music.clear();
}
