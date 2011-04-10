#include <iostream>

#include "InputHandler.h"
#include <libintl.h>
#include <locale.h>
#define _(STRING)            gettext(STRING)
#define MAX_CHARACTERS 20
using namespace std;

InputHandler::InputHandler(Binding* binding) {
	std::cout << "InputHandler..." << std::endl;

	for (int i = 0; i < GameInputEventsCount; i++) {
		m_event[i] = false;
	}

	m_binding = binding;

	mouseX = mouseY = 0;

	m_mode = Direct;
	m_textValidated = false;
	m_textContent = "";
	m_curTextPos = 0;
}

void InputHandler::setInputMode(InputMode mode) {
	if (m_mode == mode)
		return;

	if (mode == Text || mode == TextMandatory) {
		m_textValidated = false;
		m_textContent = "";
		m_curTextPos = 0;
	}

	m_mode = mode;
}

void InputHandler::setInputModeText(bool mandatory, std::string text) {
	if ((!mandatory && m_mode == Text)
			|| (mandatory && m_mode == TextMandatory))
		return;

	if (mandatory)
		setInputMode(TextMandatory);
	else
		setInputMode(Text);

	m_textContent = text;
	m_curTextPos = strlen(m_textContent.c_str());
}

string InputHandler::getTextToShow() {
	return m_textContent;
}

bool InputHandler::hasBeenValidated() {
	return m_textValidated;
}

bool InputHandler::getDownInput(GameInputEvents evnt) {
	return m_event[evnt];
}

bool InputHandler::getPressInput(GameInputEvents evnt) {
	if (m_event[evnt]) {
		m_event[evnt] = false;
		return true;
	} else {
		return false;
	}
}

void InputHandler::resetMouseButtons() {
	processEvent(Mouse, false, SDL_BUTTON_LEFT);
	processEvent(Mouse, false, SDL_BUTTON_RIGHT);
	processEvent(Mouse, false, SDL_BUTTON_MIDDLE);
}

void InputHandler::processEvent(BindingType type, bool down, int value) {
	for (int i = 0; i < GameInputEventsCount; i++) {
		if (m_binding[i].Type == type && m_binding[i].Value == value)
			m_event[i] = down;
	}
}

void InputHandler::processTextInput(SDL_Event event) {
	switch (event.key.keysym.sym) {
	case SDLK_ESCAPE:
		setInputMode(Direct);
		break;
	case SDLK_BACKSPACE:
		if (m_curTextPos > 0)
			m_textContent.erase(--m_curTextPos, 1);
		break;
	case SDLK_RETURN:
		if ((m_mode == TextMandatory && m_textContent.size() >= 1) || m_mode
				== Text) {
			m_textValidated = true;
		}
		break;
	default:
		if (event.key.keysym.unicode < 127 && m_curTextPos < MAX_CHARACTERS) {
			char c = event.key.keysym.unicode;
			if (' ' <= c && c <= '~')
				m_textContent.insert(m_curTextPos++, 1, c);
		}
		break;
	}
}

void InputHandler::process() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (m_mode == Direct)
				processEvent(Keyboard, true, event.key.keysym.sym);

			if (m_mode == Text || m_mode == TextMandatory)
				processTextInput(event);

			break;
		case SDL_KEYUP:
			processEvent(Keyboard, false, event.key.keysym.sym);

			break;
		case SDL_MOUSEBUTTONDOWN:
			processEvent(Mouse, true, event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			processEvent(Mouse, false, event.button.button);
			break;
		case SDL_MOUSEMOTION:
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			break;
		case SDL_QUIT:
			m_event[Exit] = true;
			break;
		}
	}
}

string InputHandler::getEventName(int eventNumber) {
	switch (eventNumber) {
	case Restart:
		return _("Restart");
		break;
	case Exit:
		return _("Exit");
		break;
	case Menu:
		return _("Menu");
		break;
	case MenuClickA:
		return _("MenuClickA");
		break;
	case MenuClickB:
		return _("MenuClickB");
		break;
	case ToggleLight:
		return _("Toggle Light");
		break;
	case ToggleLaser:
		return _("Toggle Laser");
		break;
	case ShowChar:
		return _("Show Char");
		break;
	case Pause:
		return _("Pause");
		break;
	case MoveLeft:
		return _("Move Left");
		break;
	case MoveRight:
		return _("Move Right");
		break;
	case MoveUp:
		return _("Move Up");
		break;
	case MoveDown:
		return _("Move Down");
		break;
	case Help:
		return _("Help");
		break;
	case Pickup:
		return _("Pick Up");
		break;
	case ThrowGrenade:
		return _("Throw Grenade");
		break;
	case Fire:
		return _("Fire");
		break;
	case Reload:
		return _("Reload");
		break;
	case Teleport:
		return _("Teleport");
		break;
	case GameInputEventsCount:
		return "GameInputEventsCount";
		break;
	default:
		return _("Unknown event");
		break;
	}
}

const int InputHandler::getEventNumber(std::string eventName) {
	if (eventName.compare("Restart") == 0)
		return Restart;

	if (eventName.compare("Exit") == 0)
		return Exit;

	if (eventName.compare("Menu") == 0)
		return Menu;

	if (eventName.compare("MenuClickA") == 0)
		return MenuClickA;

	if (eventName.compare("MenuClickB") == 0)
		return MenuClickB;

	if (eventName.compare("Toggle Light") == 0)
		return ToggleLight;

	if (eventName.compare("Toggle Laser") == 0)
		return ToggleLaser;

	if (eventName.compare("Show Char") == 0)
		return ShowChar;

	if (eventName.compare("Pause") == 0)
		return Pause;

	if (eventName.compare("Move Left") == 0)
		return MoveLeft;

	if (eventName.compare("Move Right") == 0)
		return MoveRight;

	if (eventName.compare("Move Up") == 0)
		return MoveUp;

	if (eventName.compare("Move Down") == 0)
		return MoveDown;

	if (eventName.compare("Help") == 0)
		return Help;

	if (eventName.compare("Pick Up") == 0)
		return Pickup;

	if (eventName.compare("Throw Grenade") == 0)
		return ThrowGrenade;

	if (eventName.compare("Fire") == 0)
		return Fire;

	if (eventName.compare("Reload") == 0)
		return Reload;

	if (eventName.compare("Teleport") == 0)
		return Teleport;

	return GameInputEventsCount;
}

const char* InputHandler::getKeyName(Binding bind) {
	if (bind.Type == InputHandler::Keyboard)
		return SDL_GetKeyName(SDLKey(bind.Value));
	else if (bind.Type == InputHandler::Mouse)
		switch (bind.Value) {
		default:
		case SDL_BUTTON_LEFT:
			return "left mouse";
			break;
		case SDL_BUTTON_RIGHT:
			return "right mouse";
			break;
		case SDL_BUTTON_MIDDLE:
			return "middle mouse";
			break;
		}

	return NULL;
}

