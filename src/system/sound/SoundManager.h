#include <iostream>
#include "SDL_mixer.h"
#include "../utility/FileUtility.h"
#include "Sound.h"
#include "../Configuration.h"

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

class SoundManager {
private:
	bool m_enabled;
	Configuration* m_config;
	FileUtility * m_fileUtility;
public:
	SoundManager(FileUtility* fileUtility, Configuration* config);
	Sound* create(std::string name);
	~SoundManager();
};

#endif /* SOUNDMANAGER_H_ */
