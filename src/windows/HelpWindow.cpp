#include "HelpWindow.h"

HelpWindow::HelpWindow(Configuration* config, TextManager* text) :
	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {
	const int l = config->Screen.Width * 0.1f;
	const int h = text->getHeight();
	const char* labels[] = {
		"Game controls:",
		"Move up: W",
		"Move left: A",
		"Move down: S",
		"Move right: D",
		"Fire/teleport: Left mouse button",
		"Reload: Right mouse button",
		"Pick up weapon: E",
		"Throw grenade: SPACE",
		"Toggle fire/teleport mode: Q",
		"Toggle flashlight: F",
		"Toggle laser aim: G",
		"Open player char stats: C",
		"Main menu: Esc",
	};
	std::ostringstream oss;
	for (unsigned i = 0; i < sizeof(labels)/sizeof(char*); ++i) {
		oss.str("");
		oss << i;
		addElement("label" + oss.str(), labels[i], text,
				l, (4+i)*h, TextManager::LEFT, TextManager::MIDDLE);
	}
}

HelpWindow::~HelpWindow() {
	// nothing
}
