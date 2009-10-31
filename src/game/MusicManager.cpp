#include "MusicManager.h"

const std::string DEFAULT = "dzaibatsu.ogg";

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

	m_currentPlaying = "null";
}

void MusicManager::process(Player* player, std::vector<Enemy*>* enemies,
		bool paused) {
	if (paused) {
		play();
		return;
	}
	bool afterPause = m_currentPlaying == DEFAULT;
	if (player->Kills == 0) {
		play("morning.ogg", afterPause);
		return;
	} else {
		play("space-crusader.ogg", afterPause);
		return;
	}
}

void MusicManager::play() {
	play(DEFAULT, false);
}

void MusicManager::play(std::string name, bool now) {
	if (m_currentPlaying != "null") {
		if (m_music[m_currentPlaying]->isPlaying()) {
			if (now)
				m_music[m_currentPlaying]->stop(3000);
			else
				return;
		}
	}
	m_music[name]->play(0);
	m_currentPlaying = name;
}

MusicManager::~MusicManager() {
	std::map<std::string, Sound*>::const_iterator iter;
	for (iter = m_music.begin(); iter != m_music.end(); ++iter) {
		delete iter->second;
	}
	m_music.clear();
}
