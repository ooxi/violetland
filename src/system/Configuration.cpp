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

	read();
}

void Configuration::read() {
	ConfigFile cFile(m_fileUtility->getFullPath(FileUtility::user, "config"));

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
}

void Configuration::write() {
	ConfigFile cFile(m_fileUtility->getFullPath(FileUtility::user, "config"));

	cFile.add("screenWidth", ScreenWidth);
	cFile.add("screenHeight", ScreenHeight);
	cFile.add("screenColor", ScreenColor);
	cFile.add("fullScreen", FullScreen);
	cFile.add("frameDelay", FrameDelay);
	cFile.add("showFps", ShowFps);
	cFile.add("autoReload", AutoReload);
	cFile.add("masterVolume", MasterVolume);
	cFile.add("aimColorA", AimColorA);
	cFile.add("aimColorB", AimColorB);

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
