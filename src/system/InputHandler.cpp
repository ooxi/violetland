#include <iostream>
#include <exception>

#include <libintl.h>
#include <locale.h>
#include <boost/static_assert.hpp>

#include "InputHandler.h"

#define _(STRING)            gettext(STRING)
#define MAX_CHARACTERS 20
using namespace std;

string InputHandler::m_eventIdentifiers[] = {
	"Restart", 
	"Exit", 
	"Menu", 
	"MenuClickA", 
	"MenuClickB", 
	"Toggle Light", 
	"Toggle Laser", 
	"Show Char", 
	"Pause", 
	"Move Left", 
	"Move Right", 
	"Move Up", 
	"Move Down", 
	"Help", 
	"Pick Up", 
	"Throw Grenade", 
	"Fire", 
	"Reload", 
	"Teleport", 
};

string InputHandler::m_eventNames[GameInputEventsCount];

void InputHandler::initEventNames() {
	string names[] = {
		_("Restart"), 
		_("Exit"), 
		_("Menu"), 
		_("MenuClickA"), 
		_("MenuClickB"), 
		_("Toggle Light"), 
		_("Toggle Laser"), 
		_("Show Char"), 
		_("Pause"), 
		_("Move Left"), 
		_("Move Right"), 
		_("Move Up"), 
		_("Move Down"), 
		_("Help"), 
		_("Pick Up"), 
		_("Throw Grenade"), 
		_("Fire"), 
		_("Reload"), 
		_("Teleport"), 
	};
	BOOST_STATIC_ASSERT(sizeof(names)/sizeof(string) == GameInputEventsCount);
	
	for (unsigned i = 0; i < GameInputEventsCount; ++i)
		m_eventNames[i] = names[i];
}

InputHandler::InputHandler(Binding* binding) {
	BOOST_STATIC_ASSERT(sizeof(m_eventNames)/sizeof(string) == GameInputEventsCount);
	
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

void InputHandler::processTextInput(SDL_Event sdlEvent) {
	switch (sdlEvent.key.keysym.sym) {
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
		if (sdlEvent.key.keysym.unicode < 127 && m_curTextPos < MAX_CHARACTERS) {
			char c = sdlEvent.key.keysym.unicode;
			if (' ' <= c && c <= '~')
				m_textContent.insert(m_curTextPos++, 1, c);
		}
		break;
	}
}

void InputHandler::process() {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			if (m_mode == Direct)
				processEvent(Keyboard, true, sdlEvent.key.keysym.sym);

			if (m_mode == Text || m_mode == TextMandatory)
				processTextInput(sdlEvent);

			break;
		case SDL_KEYUP:
			processEvent(Keyboard, false, sdlEvent.key.keysym.sym);

			break;
		case SDL_MOUSEBUTTONDOWN:
			processEvent(Mouse, true, sdlEvent.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			processEvent(Mouse, false, sdlEvent.button.button);
			break;
		case SDL_MOUSEMOTION:
			mouseX = sdlEvent.motion.x;
			mouseY = sdlEvent.motion.y;
			break;
		case SDL_QUIT:
			m_event[Exit] = true;
			break;
		}
	}
}

string InputHandler::getEventName(int eventNumber) {
	if (eventNumber > GameInputEventsCount)
		throw exception();
	else
		return m_eventNames[eventNumber];
}

string InputHandler::getEventIdentifier(int eventNumber) {
	if (eventNumber > GameInputEventsCount)
		throw exception();
	else
		return m_eventIdentifiers[eventNumber];
}

const unsigned InputHandler::getEventNumber(std::string eventIdentifier) {
	for (unsigned i = 0; i < GameInputEventsCount; ++i)
		if (m_eventIdentifiers[i] == eventIdentifier)
			return i;

	throw exception();
}

string InputHandler::getKeyName(Binding bind) {
	if (bind.Type == InputHandler::Keyboard)
	{
		string keyName = SDL_GetKeyName(SDLKey(bind.Value));
		return keyName;
	}
	else 
	{
		if (bind.Type == InputHandler::Mouse)
		{
			switch (bind.Value) {
			default:
			case SDL_BUTTON_LEFT:
				return "left mouse";
			case SDL_BUTTON_RIGHT:
				return "right mouse";
			case SDL_BUTTON_MIDDLE:
				return "middle mouse";
			}
		}
	}

	return NULL;
}

