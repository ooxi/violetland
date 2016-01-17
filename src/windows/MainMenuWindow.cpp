#include "MainMenuWindow.h"

#include <boost/bind.hpp>
#include <libintl.h>

#include "../game/GameState.h"
#include "../system/Configuration.h"
#include "../system/graphic/text/TextManager.h"

#define _(STRING) gettext(STRING)

using namespace violet;
using namespace std;





MainMenuWindow::MainMenuWindow(
			Configuration* config,
			GameState* state,
			TextManager* text
		) :	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f),
			m_config(config),
			m_state(state),
			m_text(text)
	{
	
	const int l = m_config->Screen.Width * 0.1f;
	const int h = m_text->getHeight();

	std::vector<Label> labels;
	if (m_state->Begun && !m_state->Lost)
		labels.push_back(Label("resume", _("Resume")));
	labels.push_back(Label("start", _("Start")));
	labels.push_back(Label("options", _("Options")));
	labels.push_back(Label("highscores", _("High scores")));
	labels.push_back(Label("exit", _("Exit")));
	
	addElements(labels, m_text, 
			l, 7*h, h, TextManager::LEFT, TextManager::MIDDLE);
	
	
	addHandler(Window::hdl_lclick, "resume", boost::bind(&MainMenuWindow::onResumeClick, this));
	addHandler(Window::hdl_lclick, "start", boost::bind(&MainMenuWindow::onStartClick, this));
	addHandler(Window::hdl_lclick, "options", boost::bind(&MainMenuWindow::onOptionsClick, this));
	addHandler(Window::hdl_lclick, "highscores", boost::bind(&MainMenuWindow::onHighScoresClick, this));
	addHandler(Window::hdl_lclick, "exit", boost::bind(&MainMenuWindow::onExitClick, this));
	
	
	refresh();
}



void MainMenuWindow::refresh() {
	const int r = m_config->Screen.Width * 0.3f;

	string strGameMode = _("Unknown");
	switch (m_state->Mode) {
	case GAMEMODE_SURVIVAL:
		strGameMode = _("Violetland Survival");
		break;
	case GAMEMODE_WAVES:
		strGameMode = _("Attack waves");
		break;
	}

	addElement("gamemode", strGameMode, m_text, r,
			m_text->getHeight() * 8.0f, TextManager::LEFT,
			TextManager::MIDDLE);
	
	addHandler(Window::hdl_lclick, "gamemode", boost::bind(&MainMenuWindow::onGameModeClick, this));
}



void MainMenuWindow::onExitClick()
{
	m_state->end();
}



void MainMenuWindow::onGameModeClick() {
	switch (m_state->Mode) {
	case GAMEMODE_SURVIVAL:
		m_state->Mode = GAMEMODE_WAVES;
		break;
	case GAMEMODE_WAVES:
		m_state->Mode = GAMEMODE_SURVIVAL;
		break;
	}

	refresh();
}



MainMenuWindow::~MainMenuWindow() {
	// nothing
}
