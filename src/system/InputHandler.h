#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include "SDL.h"

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
		MenuClickA,
		MenuClickB,
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
		Teleport,
		GameInputEventsCount
	};
	InputHandler(Binding binding[]);
	void process();
	bool getDownInput(GameInputEvents evnt);
	bool getPressInput(GameInputEvents evnt);
	int mouseX, mouseY;
	static const char* getName(int event);
	static const char* getKeyName(Binding bind);
private:
	void processEvent(BindingType type, bool down, int value);
	bool m_event[GameInputEventsCount];
	Binding* m_binding;
};

#endif /* INPUTHANDLER_H_ */
