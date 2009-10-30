#include "SoundManager.h"

SoundManager::SoundManager(FileUtility* fileUtility, int mastVol) {
	printf("SoundManager...\n");

	m_fileUtility = fileUtility;
	m_mastVol = mastVol;

	fprintf(stdout, "Mix_OpenAudio (freq %i)...\n",
			MIX_DEFAULT_FREQUENCY, 2, 2048);
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		m_enabled = false;
		fprintf(stderr, "Sound is disabled: %s\n", Mix_GetError());
	} else {
		m_enabled = true;
		printf("Sound is enabled.\n");
		fprintf(stdout, "Mix_Volume (master) %i...\n", m_mastVol);
		Mix_Volume(-1, m_mastVol);
	}
}

Sound* SoundManager::create(std::string name) {
	Mix_Chunk* chunk = m_enabled ? Mix_LoadWAV(m_fileUtility->getFullPath(FileUtility::sound,
					name).c_str()) : NULL;
	Sound* snd = new Sound(chunk, m_enabled);
	return snd;
}

SoundManager::~SoundManager() {
	if (m_enabled)
		Mix_CloseAudio();
}
