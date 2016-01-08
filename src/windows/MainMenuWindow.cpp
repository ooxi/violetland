#include <boost/bind.hpp>

#include "MainMenuWindow.h"
#include <libintl.h>
#include <locale.h>
#define _(STRING)            gettext(STRING)

using namespace violetland;
using namespace std;

MainMenuWindow::MainMenuWindow(Configuration* config, GameState* gameState,
		TextManager* text) :
	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {

	m_gameState = gameState;

	const int l = config->Screen.Width * 0.1f;
	const int h = text->getHeight();

	std::vector<Label> labels;
	if (m_gameState->Begun && !m_gameState->Lost)
		labels.push_back(Label("resume", _("Resume")));
	labels.push_back(Label("start", _("Start")));
	labels.push_back(Label("options", _("Options")));
	labels.push_back(Label("highscores", _("High scores")));
	labels.push_back(Label("exit", _("Exit")));
	
	addElements(labels, text, 
			l, 7*h, h, TextManager::LEFT, TextManager::MIDDLE);

	addHandler(Window::hdl_lclick, "exit", boost::bind(&MainMenuWindow::onMenuItemClick, this, _1));
}

void MainMenuWindow::exitGame()
{
	m_gameState->end();
}

void MainMenuWindow::onMenuItemClick(string menuItem)
{
	if (menuItem == "exit")
		exitGame();
}

MainMenuWindow::~MainMenuWindow() {
	// nothing
}
