#ifndef MAINMENUWINDOW_H_
#define MAINMENUWINDOW_H_

#include "../system/graphic/Window.h"
#include "../system/Configuration.h"
#include "../game/GameState.h"
#include "../system/graphic/text/TextManager.h"

namespace violetland {
class MainMenuWindow: public Window {
private:
	GameState* m_gameState;

	void exitGame();

public:
	static void onMenuItemClick(void* sender, std::string menuItem);

	MainMenuWindow(Configuration* config, GameState* gameState,
			TextManager* text);
	~MainMenuWindow();
};
}

#endif /* MAINMENUWINDOW_H_ */
