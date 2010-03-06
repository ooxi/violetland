#include "utility/FileUtility.h"
#include "ConfigFile.h"
#include "InputHandler.h"

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
	int SoundVolume;
	int MusicVolume;
	unsigned int MonstersAtStart;
	int AimColorA, AimColorB;
	bool AutoWeaponPickup;
	bool FriendlyFire;
	int GameAreaSize;
	InputHandler::Binding
			PlayerInputBinding[InputHandler::GameInputEventsCount];
	void ReadPlayerBinding(ConfigFile* cFile, InputHandler::Binding* binding,
			std::string actionName);
	void WritePlayerBinding(ConfigFile* cFile, InputHandler::Binding* binding,
			std::string actionName);
};

#endif /* CONFIGURATION_H_ */
