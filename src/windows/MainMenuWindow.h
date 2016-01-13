#ifndef VIOLET_MAINMENUWINDOW_H_
#define VIOLET_MAINMENUWINDOW_H_

#include "Window.h"

namespace violet {

class Configuration;
class GameState;
class TextManager;

class MainMenuWindow: public Window {

private:
	Configuration* const m_config;
	GameState* const m_state;
	TextManager* const m_text;

	void onResumeClick();
	void onStartClick();
	void onOptionsClick();
	void onHighScoresClick();
	void onExitClick();
	void onGameModeClick();
	
	void refresh();

public:

	MainMenuWindow(	Configuration* config,
			GameState* state,
			TextManager* text
		);
	
	virtual ~MainMenuWindow();
};
}

#endif /* VIOLET_MAINMENUWINDOW_H_ */
