#include <iostream>
#include "SDL_mixer.h"
#include "../utility/FileUtility.h"
#include "Sound.h"

#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

class SoundManager {
private:
	bool m_enabled;
	int m_mastVol;
	FileUtility* m_fileUtility;
public:
	SoundManager(FileUtility* fileUtility, int mastVol);
	Sound* create(std::string name);
	~SoundManager();
};

#endif /* SOUNDMANAGER_H_ */
