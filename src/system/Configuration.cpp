#include "Configuration.h"

Configuration::Configuration(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

	ScreenWidth = 800;
	ScreenHeight = 600;
	ScreenColor = 16;
	FullScreen = false;
	FrameDelay = 10;
	ShowFps = false;
	AutoReload = true;
	MasterVolume = 40;
	MonstersAtStart = 8;
	AimColorA = 0x000000;
	AimColorB = 0xFFFFFF;
}

void Configuration::read() {
	try {
		ConfigFile cFile(
				m_fileUtility->getFullPath(FileUtility::user, "config"));

		cFile.readInto(ScreenWidth, "screenWidth");
		cFile.readInto(ScreenHeight, "screenHeight");
		cFile.readInto(ScreenColor, "screenColor");
		cFile.readInto(FullScreen, "fullScreen");
		cFile.readInto(FrameDelay, "frameDelay");
		cFile.readInto(ShowFps, "showFps");
		cFile.readInto(AutoReload, "autoReload");
		cFile.readInto(MasterVolume, "masterVolume");
		cFile.readInto(AimColorA, "aimColorA");
		cFile.readInto(AimColorB, "aimColorB");
	} catch (...) {
		printf("Can't open config file.\n");
	}
}

void Configuration::write() {
	ConfigFile cFile;

	cFile.add("aimColorB", AimColorB);
	cFile.add("aimColorA", AimColorA);
	cFile.add("masterVolume", MasterVolume);
	cFile.add("autoReload", AutoReload);
	cFile.add("showFps", ShowFps);
	cFile.add("frameDelay", FrameDelay);
	cFile.add("fullScreen", FullScreen);
	cFile.add("screenColor", ScreenColor);
	cFile.add("screenHeight", ScreenHeight);
	cFile.add("screenWidth", ScreenWidth);

	std::ofstream ofile(
			m_fileUtility->getFullPath(FileUtility::user, "config").c_str());
	if (ofile) {
		ofile << cFile;
		ofile.close();
	}
}

Configuration::~Configuration() {
	// nothing
}
