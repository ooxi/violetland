#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include <boost/filesystem.hpp>

#include "Sound.h"
#include "../Configuration.h"

using namespace boost;

class SoundManager {
private:
	bool m_enabled;
	Configuration* m_config;
	FileUtility * m_fileUtility;
public:
	SoundManager(FileUtility* fileUtility, Configuration* config);
	Sound* create(filesystem::path name);
	~SoundManager();
};

#endif /* SOUNDMANAGER_H_ */
