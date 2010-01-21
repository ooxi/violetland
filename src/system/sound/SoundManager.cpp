#include "SoundManager.h"

SoundManager::SoundManager(FileUtility* fileUtility, Configuration* config) {
	printf("SoundManager...\n");

	m_fileUtility = fileUtility;
	m_config = config;

	fprintf(stdout, "Mix_OpenAudio (freq %i)...\n", MIX_DEFAULT_FREQUENCY);
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		m_enabled = false;
		fprintf(stderr, "\tsound is disabled\n\t%s\n", Mix_GetError());
	} else {
		m_enabled = true;
		Mix_AllocateChannels(16);
		printf("\tsound is enabled\n");
		Mix_Volume(0, m_config->MusicVolume * 12);
		for (unsigned int a = 1; a <= 8; a++) {
			Mix_Volume(a, m_config->SoundVolume * 12);
		}
	}
}

Sound* SoundManager::create(std::string path) {
	Mix_Chunk * chunk = m_enabled ? Mix_LoadWAV(path.c_str()) : NULL;
	Sound* snd = new Sound(chunk, m_enabled);
	return snd;
}

SoundManager::~SoundManager() {
	if (m_enabled)
		Mix_CloseAudio();
}
