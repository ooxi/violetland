#include "HelpWindow.h"

HelpWindow::HelpWindow(Configuration* config, TextManager* text) :
	Window(0.0f, 0.0f, config->VMode.Width, config->VMode.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {
	const int l = config->VMode.Width * 0.1f;

	addElement("label1", text->getObject("Game controls:", l, text->getHeight()
			* 4, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label3", text->getObject("Move up: W", l,
			text->getHeight() * 6, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label4", text->getObject("Move left: A", l, text->getHeight()
			* 7, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label5", text->getObject("Move down: S", l, text->getHeight()
			* 8, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label6", text->getObject("Move right: D", l, text->getHeight()
			* 9, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label7", text->getObject("Fire: Left mouse button", l,
			text->getHeight() * 10, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label8", text->getObject("Reload: Right mouse button", l,
			text->getHeight() * 11, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label2", text->getObject("Pick up weapon: E", l,
			text->getHeight() * 12, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label14", text->getObject("Throw grenade: SPACE", l,
			text->getHeight() * 13, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label9", text->getObject("Toggle flashlight: F", l,
			text->getHeight() * 14, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label10", text->getObject("Toggle laser aim: G", l,
			text->getHeight() * 15, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label13", text->getObject("Open player char stats: C", l,
			text->getHeight() * 16, TextManager::LEFT, TextManager::MIDDLE));
	addElement("label12", text->getObject("Main menu: Esc", l,
			text->getHeight() * 17, TextManager::LEFT, TextManager::MIDDLE));
}

HelpWindow::~HelpWindow() {
	// nothing
}
