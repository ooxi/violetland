#include "MainMenuWindow.h"
#include <libintl.h>
#include <locale.h>
#define _(STRING)            gettext(STRING)

MainMenuWindow::MainMenuWindow(Configuration* config, GameState* gameState,
		TextManager* text) :
	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {
	const int l = config->Screen.Width * 0.1f;
	const int h = text->getHeight();

	std::vector<Label> labels;
	if (gameState->Begun && !gameState->Lost)
		labels.push_back(Label("resume", _("Resume")));
	labels.push_back(Label("start", _("Start")));
	labels.push_back(Label("options", _("Options")));
	labels.push_back(Label("highscores", _("High scores")));
	labels.push_back(Label("exit", _("Exit")));
	
	addElements(labels, text, 
			l, 7*h, h, TextManager::LEFT, TextManager::MIDDLE);
}

MainMenuWindow::~MainMenuWindow() {
	// nothing
}
