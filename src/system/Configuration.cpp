#include "Configuration.h"

Configuration::Configuration(FileUtility* fileUtility) {
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

	PlayerInputBinding[InputHandler::Teleport].Value = SDLK_q;
	PlayerInputBinding[InputHandler::MoveLeft].Value = SDLK_a;
	PlayerInputBinding[InputHandler::MoveUp].Value = SDLK_w;
	PlayerInputBinding[InputHandler::MoveRight].Value = SDLK_d;
	PlayerInputBinding[InputHandler::MoveDown].Value = SDLK_s;
	PlayerInputBinding[InputHandler::Restart].Value = SDLK_RETURN;
	PlayerInputBinding[InputHandler::Menu].Value = SDLK_ESCAPE;
	PlayerInputBinding[InputHandler::MenuClickA].Type = InputHandler::Mouse;
	PlayerInputBinding[InputHandler::MenuClickA].Value = SDL_BUTTON_LEFT;
	PlayerInputBinding[InputHandler::MenuClickB].Type = InputHandler::Mouse;
	PlayerInputBinding[InputHandler::MenuClickB].Value = SDL_BUTTON_RIGHT;
	PlayerInputBinding[InputHandler::Exit].Value = SDLK_F12;
	PlayerInputBinding[InputHandler::ToggleLight].Value = SDLK_f;
	PlayerInputBinding[InputHandler::ToggleLaser].Value = SDLK_g;
	PlayerInputBinding[InputHandler::Pause].Value = SDLK_p;
	PlayerInputBinding[InputHandler::ShowChar].Value = SDLK_c;
	PlayerInputBinding[InputHandler::Help].Value = SDLK_F1;
	PlayerInputBinding[InputHandler::Pickup].Value = SDLK_e;
	PlayerInputBinding[InputHandler::ThrowGrenade].Value = SDLK_SPACE;
	PlayerInputBinding[InputHandler::Fire].Value = SDL_BUTTON_LEFT;
	PlayerInputBinding[InputHandler::Fire].Type = InputHandler::Mouse;
	PlayerInputBinding[InputHandler::Reload].Value = SDL_BUTTON_RIGHT;
	PlayerInputBinding[InputHandler::Reload].Type = InputHandler::Mouse;
}

void Configuration::read() {
	try {
		ConfigFile cFile(
				m_fileUtility->getFullPath(FileUtility::user, "config"));

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

		for (int i = 0; i < InputHandler::GameInputEventsCount; i++) {
			ReadPlayerBinding(&cFile, &PlayerInputBinding[i],
					InputHandler::getEventName(i));
		}

	} catch (...) {
		std::cout << "Can't open config file." << std::endl;
	}
}

void Configuration::ReadPlayerBinding(ConfigFile* cFile,
		InputHandler::Binding* binding, std::string actionName) {
	int type;
	std::string keyType = "playerInputBinding_" + actionName + "Type";
	std::string keyValue = "playerInputBinding_" + actionName + "Value";

	if (cFile->keyExists(keyType)) {
		cFile->readInto(type, keyType);
		binding->Type = (InputHandler::BindingType) type;
		cFile->readInto(binding->Value, keyValue);
	}
}

void Configuration::WritePlayerBinding(ConfigFile* cFile,
		InputHandler::Binding* binding, std::string actionName) {
	std::string keyType = "playerInputBinding_" + actionName + "Type";
	std::string keyValue = "playerInputBinding_" + actionName + "Value";

	cFile->add(keyType, (int) binding->Type);
	cFile->add(keyValue, binding->Value);
}

void Configuration::write() {
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

	for (int i = 0; i < InputHandler::GameInputEventsCount; i++) {
		WritePlayerBinding(&cFile, &PlayerInputBinding[i],
				InputHandler::getEventName(i));
	}

	filesystem::ofstream ofile(
			m_fileUtility->getFullPath(FileUtility::user, "config"));
	if (ofile) {
		ofile << cFile;
		ofile.close();
	}
}

Configuration::~Configuration() {
	// nothing
}
