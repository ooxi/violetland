#include "SDL.h"

#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

class InputHandler {
public:
	enum BindingType {
		Keyboard = 0, Mouse
	};
	struct Binding {
	public:
		int Value;
		BindingType Type;

		Binding() {
			Type = Keyboard;
		}
	};
	enum GameInputEvents {
		Restart = 0,
		Exit,
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
	void processEvent(BindingType type, bool down, int value);
	bool m_event[GameInputEventsCount];
	Binding m_binding[GameInputEventsCount];
};

#endif /* INPUTHANDLER_H_ */
