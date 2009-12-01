#include "SDL.h"

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

class InputHandler {
public:
	enum GameInputEvents {
		Restart = 0,
		Escape,
		Menu,
		ToggleLight,
		ToggleLaser,
		ShowChar,
		Pause,
		MoveLeft,
		MoveRight,
		MoveUp,
		MoveDown,
		Help,
		Pickup,
		ThrowGrenade,
		Fire,
		Reload,
		GameInputEventsCount
	};
	InputHandler();
	void process();
	bool getDownInput(GameInputEvents evnt);
	bool getPressInput(GameInputEvents evnt);
	int mouseX, mouseY;
private:
	bool m_event[GameInputEventsCount];
	int m_eventMap[GameInputEventsCount];
};

#endif /* INPUTHANDLER_H_ */
