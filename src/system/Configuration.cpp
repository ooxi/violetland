#include "Configuration.h"
#include <sstream>

violet::Configuration::Configuration(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

	Screen.Width = 800;
	Screen.Height = 600;
	Screen.Color = 16;
	Screen.Full = false;

	FrameDelay = 10;
	ShowFps = false;
	AutoReload = true;
	SoundVolume = 4;
	MusicVolume = 4;
	MonstersAtStart = 12;
	AimColorA = 0x000000;
	AimColorB = 0xFFFFFF;
	AutoWeaponPickup = true;
	FriendlyFire = false;
	Control = violet::E_CONTROL_STYLE_MODERN;
	ControlPreset = 1;

	for(int i = 0; i < VIOLET_CONTROL_PRESET_NUMBER; ++i)
	{
	    PlayerInputBinding[InputHandler::Teleport].Value[i] = SDLK_q;
	    PlayerInputBinding[InputHandler::MoveLeft].Value[i] = SDLK_a;
	    PlayerInputBinding[InputHandler::MoveUp].Value[i] = SDLK_w;
	    PlayerInputBinding[InputHandler::MoveRight].Value[i] = SDLK_d;
	    PlayerInputBinding[InputHandler::MoveDown].Value[i] = SDLK_s;
	    PlayerInputBinding[InputHandler::Restart].Value[i] = SDLK_RETURN;
	    PlayerInputBinding[InputHandler::Menu].Value[i] = SDLK_ESCAPE;
	    PlayerInputBinding[InputHandler::MenuClickA].Type[i] = InputHandler::Mouse;
	    PlayerInputBinding[InputHandler::MenuClickA].Value[i] = SDL_BUTTON_LEFT;
	    PlayerInputBinding[InputHandler::MenuClickB].Type[i] = InputHandler::Mouse;
	    PlayerInputBinding[InputHandler::MenuClickB].Value[i] = SDL_BUTTON_RIGHT;
	    PlayerInputBinding[InputHandler::Exit].Value[i] = SDLK_F12;
	    PlayerInputBinding[InputHandler::ToggleLight].Value[i] = SDLK_f;
	    PlayerInputBinding[InputHandler::ToggleLaser].Value[i] = SDLK_g;
	    PlayerInputBinding[InputHandler::Pause].Value[i] = SDLK_p;
	    PlayerInputBinding[InputHandler::ShowChar].Value[i] = SDLK_c;
	    PlayerInputBinding[InputHandler::Help].Value[i] = SDLK_F1;
	    PlayerInputBinding[InputHandler::Pickup].Value[i] = SDLK_e;
	    PlayerInputBinding[InputHandler::ThrowGrenade].Value[i] = SDLK_SPACE;
	    PlayerInputBinding[InputHandler::Fire].Value[i] = SDL_BUTTON_LEFT;
	    PlayerInputBinding[InputHandler::Fire].Type[i] = InputHandler::Mouse;
	    PlayerInputBinding[InputHandler::Reload].Value[i] = SDL_BUTTON_RIGHT;
	    PlayerInputBinding[InputHandler::Reload].Type[i] = InputHandler::Mouse;
	}
}

void violet::Configuration::read() {
	try {
		ConfigFile cFile(m_fileUtility->getFullPath(
			FileUtility::user, "config"
		));

		cFile.readInto(Screen.Width, "screenWidth");
		cFile.readInto(Screen.Height, "screenHeight");
		cFile.readInto(Screen.Color, "screenColor");
		cFile.readInto(Screen.Full, "fullScreen");
		cFile.readInto(FrameDelay, "frameDelay");
		cFile.readInto(ShowFps, "showFps");
		cFile.readInto(AutoReload, "autoReload");
		cFile.readInto(SoundVolume, "soundVolume");
		cFile.readInto(MusicVolume, "musicVolume");
		cFile.readInto(AimColorA, "aimColorA");
		cFile.readInto(AimColorB, "aimColorB");
		cFile.readInto(AutoWeaponPickup, "autoWeaponPickup");
		cFile.readInto(FriendlyFire, "friendlyFire");

		int controlStyle = 0;
		cFile.readInto(controlStyle, "controlStyle");
		Control = violet::ControlStyleFromInt(controlStyle);

		for (int i = 0; i < InputHandler::GameInputEventsCount; i++) {
			ReadPlayerBinding(&cFile, &PlayerInputBinding[i],
					InputHandler::getEventIdentifier(i));
		}

	} catch (...) {
		std::cout << "Can't open config file." << std::endl;
	}
}

void violet::Configuration::ReadPlayerBinding(ConfigFile* cFile,
		InputHandler::Binding* binding, std::string eventIdentifier) {
	int type;
	for(int i = 0; i < VIOLET_CONTROL_PRESET_NUMBER; ++i)
	{
	    std::ostringstream ss;
	    ss << i+1;
	    std::string keyType = "playerInputBinding_" + eventIdentifier + "Type_" + ss.str();
	    std::string keyValue = "playerInputBinding_" + eventIdentifier + "Value_" + ss.str();

	    if (cFile->keyExists(keyType)) {
		cFile->readInto(type, keyType);
		binding->Type[i] = (InputHandler::BindingType) type;
		cFile->readInto(binding->Value[i], keyValue);
	    }
	}
}

void violet::Configuration::WritePlayerBinding(ConfigFile* cFile,
		InputHandler::Binding* binding, std::string eventIdentifier) {
	for(int i = 0; i < VIOLET_CONTROL_PRESET_NUMBER; ++i)
	{
	    std::ostringstream ss;
	    ss << i+1;
	    std::string keyType = "playerInputBinding_" + eventIdentifier + "Type_" + ss.str();
	    std::string keyValue = "playerInputBinding_" + eventIdentifier + "Value_" + ss.str();

	    cFile->add(keyType, (int) binding->Type[i]);
	    cFile->add(keyValue, binding->Value[i]);
	}
}

void violet::Configuration::write() {
	ConfigFile cFile;

	cFile.add("aimColorB", AimColorB);
	cFile.add("aimColorA", AimColorA);
	cFile.add("soundVolume", SoundVolume);
	cFile.add("musicVolume", MusicVolume);
	cFile.add("autoReload", AutoReload);
	cFile.add("showFps", ShowFps);
	cFile.add("frameDelay", FrameDelay);
	cFile.add("fullScreen", Screen.Full);
	cFile.add("screenColor", Screen.Color);
	cFile.add("screenHeight", Screen.Height);
	cFile.add("screenWidth", Screen.Width);
	cFile.add("autoWeaponPickup", AutoWeaponPickup);
	cFile.add("friendlyFire", FriendlyFire);
	cFile.add("controlStyle", Control);

	for (int i = 0; i < InputHandler::GameInputEventsCount; i++) {
		WritePlayerBinding(&cFile, &PlayerInputBinding[i],
				InputHandler::getEventIdentifier(i));
	}

	boost::filesystem::ofstream ofile(
			m_fileUtility->getFullPath(FileUtility::user, "config"));
	if (ofile) {
		ofile << cFile;
		ofile.close();
	}
}

violet::Configuration::~Configuration() {
	// nothing
}
