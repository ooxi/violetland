#ifndef INPUTHANDLER_H_
#define INPUTHANDLER_H_

#include <string>
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
	enum InputMode {
		Direct = 0, Text, TextMandatory
	};
	InputHandler(Binding binding[]);
	void process();
	void setInputMode(InputMode mode);
	void setInputModeText(bool mandatory, std::string text);
	std::string getTextToShow();
	bool hasBeenValidated();
	bool getDownInput(GameInputEvents evnt);
	bool getPressInput(GameInputEvents evnt);
	void resetMouseButtons();
	int mouseX, mouseY;
	static std::string getEventName(int eventNumber);
	static const int getEventNumber(std::string eventName);
	static const char* getKeyName(Binding bind);
private:
	void processEvent(BindingType type, bool down, int value);
	void processTextInput(SDL_Event event);
	bool m_event[GameInputEventsCount];
	InputMode m_mode;
	bool m_textValidated;
	std::string m_textContent;
	int m_curTextPos;
	Binding* m_binding;
};

#endif /* INPUTHANDLER_H_ */
