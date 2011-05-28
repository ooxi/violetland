#include <sstream>

#include "SoundManager.h"



SoundManager::SoundManager(FileUtility* fileUtility, Configuration* config) {
	std::ostringstream oss;
	
	std::cout << "SoundManager..." << std::endl;

	m_fileUtility = fileUtility;
	m_config = config;

	std::cout << "Mix_OpenAudio (freq " << MIX_DEFAULT_FREQUENCY << ")..." << std::endl;
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
		m_enabled = false;
		std::cerr << "\tsound is disabled" << std::endl;
		std::cerr << '\t' << Mix_GetError() << std::endl;
	} else {
		m_enabled = true;
		Mix_AllocateChannels(16);
		std::cout << "\tsound is enabled" << std::endl;
		Mix_VolumeMusic(m_config->MusicVolume * 12);
		for (unsigned int a = 1; a <= 8; ++a) {
			Mix_Volume(a, m_config->SoundVolume * 12);
		}
	}
}

Sound* SoundManager::create(boost::filesystem::path path) {
	Mix_Chunk * chunk = m_enabled ? Mix_LoadWAV(path.string().c_str()) : NULL;
	Sound* snd = new Sound(chunk, m_enabled);
	return snd;
}

SoundManager::~SoundManager() {
	if (m_enabled)
		Mix_CloseAudio();
}
