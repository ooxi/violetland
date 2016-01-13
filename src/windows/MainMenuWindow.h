#ifndef VIOLET_MAINMENUWINDOW_H_
#define VIOLET_MAINMENUWINDOW_H_

#include "Window.h"
#include "../system/Configuration.h"
#include "../game/GameState.h"
#include "../system/graphic/text/TextManager.h"

namespace violet {

class MainMenuWindow: public Window {
private:
	GameState* m_gameState;

	void onMenuItemClick(std::string menuItem);
	void exitGame();

public:

	MainMenuWindow(Configuration* config, GameState* gameState,
			TextManager* text);
	virtual ~MainMenuWindow();
};
}

#endif /* VIOLET_MAINMENUWINDOW_H_ */
