#ifndef MAINMENUWINDOW_H_
#define MAINMENUWINDOW_H_

#include "../system/graphic/Window.h"
#include "../system/Configuration.h"
#include "../game/GameState.h"
#include "../system/graphic/text/TextManager.h"

class MainMenuWindow: public Window {
public:
	MainMenuWindow(Configuration* config, GameState* gameState,
			TextManager* text);
	~MainMenuWindow();
};

#endif /* MAINMENUWINDOW_H_ */
