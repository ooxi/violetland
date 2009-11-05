#include "utility/FileUtility.h"
#include "ConfigFile.h"

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

class Configuration {
private:
	FileUtility* m_fileUtility;
public:
	Configuration(FileUtility* fileUtility);
	void read();
	void write();
	~Configuration();
	int ScreenWidth, ScreenHeight;
	int ScreenColor;
	bool FullScreen;
	int FrameDelay;
	bool ShowFps;
	bool AutoReload;
	int MasterVolume;
	unsigned int MonstersAtStart;
	int AimColorA, AimColorB;
	bool AutoWeaponPickup;
};

#endif /* CONFIGURATION_H_ */
