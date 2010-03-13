#include "MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(Configuration* config, GameState* gameState,
		TextManager* text) :
	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {
	const int l = config->Screen.Width * 0.1f;

	if (gameState->Begun && !gameState->Lost) {
		addElement("resume", text->getObject("Resume", l, text->getHeight()
				* 7.0f, TextManager::LEFT, TextManager::MIDDLE));
	}

	addElement("survival", text->getObject("New survival", l, text->getHeight()
			* 8.0f, TextManager::LEFT, TextManager::MIDDLE));

	addElement("options", text->getObject("Options", l, text->getHeight()
			* 9.0f, TextManager::LEFT, TextManager::MIDDLE));

	addElement("highscores", text->getObject("High scores", l,
			text->getHeight() * 10.0f, TextManager::LEFT, TextManager::MIDDLE));

	addElement("exit", text->getObject("Exit", l, text->getHeight() * 11.0f,
			TextManager::LEFT, TextManager::MIDDLE));
}

MainMenuWindow::~MainMenuWindow() {
	// nothing
}
