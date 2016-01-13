#ifndef VIOLET_CONFIGURATION_H_
#define VIOLET_CONFIGURATION_H_

#include <boost/filesystem/fstream.hpp>

#include "utility/FileUtility.h"
#include "ConfigFile.h"
#include "ControlStyle.h"
#include "InputHandler.h"

namespace violet {

struct VideoMode {
public:
	int Width;
	int Height;
	bool Full;
	int Color;
};

class Configuration {
private:
	FileUtility* m_fileUtility;
public:
	Configuration(FileUtility* fileUtility);
	void read();
	void write();
	~Configuration();
	VideoMode Screen;
	int FrameDelay;
	bool ShowFps;
	bool AutoReload;
	int SoundVolume;
	int MusicVolume;
	unsigned int MonstersAtStart;
	int AimColorA, AimColorB;
	bool AutoWeaponPickup;
	bool FriendlyFire;
	violet::ControlStyle Control;
	InputHandler::Binding
			PlayerInputBinding[InputHandler::GameInputEventsCount];
	void ReadPlayerBinding(ConfigFile* cFile, InputHandler::Binding* binding,
			std::string actionName);
	void WritePlayerBinding(ConfigFile* cFile, InputHandler::Binding* binding,
			std::string actionName);
};
}

#endif /* VIOLET_CONFIGURATION_H_ */
