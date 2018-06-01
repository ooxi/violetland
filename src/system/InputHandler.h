#ifndef VIOLET_INPUTHANDLER_H_
#define VIOLET_INPUTHANDLER_H_
#define VIOLET_CONTROL_PRESET_NUMBER 2

#include <string>

#include "SDL.h"

namespace violet {

class InputHandler {
public:
	enum BindingType {
		Keyboard = 0, Mouse
	};
	struct Binding {
	public:
		int Value[VIOLET_CONTROL_PRESET_NUMBER];
		BindingType Type[VIOLET_CONTROL_PRESET_NUMBER];

		Binding() {
		    for(int i = 0; i < VIOLET_CONTROL_PRESET_NUMBER; ++i)
			Type[i] = Keyboard;
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
	const std::string& getTextToShow() const {
		return m_textContent;
	}
	bool hasBeenValidated() const {
		return m_textValidated;
	}
	bool getDownInput(GameInputEvents evnt) const {
		return m_event[evnt];
	}
	bool getPressInput(GameInputEvents evnt);
	void resetMouseButtons();
	int mouseX, mouseY;
	static std::string getEventName(int eventNumber);
	static std::string getEventIdentifier(int eventNumber);
	static const unsigned getEventNumber(std::string eventIdentifier);
	static std::string getKeyName(BindingType Type, int Value);
	static void initEventNames();
private:
	void processEvent(BindingType type, bool down, int value);
	void processTextInput(SDL_Event sdlEvent);
	bool m_event[GameInputEventsCount];
	static std::string m_eventNames[GameInputEventsCount];
	static std::string m_eventIdentifiers[GameInputEventsCount];
	InputMode m_mode;
	bool m_textValidated;
	std::string m_textContent;
	int m_curTextPos;
	Binding* m_binding;
};
}

#endif /* VIOLET_INPUTHANDLER_H_ */
