#include "InputHandler.h"

InputHandler::InputHandler(Binding* binding) {
	printf("InputHandler...\n");

	for (int i = 0; i < GameInputEventsCount; i++) {
		m_event[i] = false;
	}

	m_binding = binding;

	mouseX = mouseY = 0;
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

void InputHandler::processEvent(BindingType type, bool down, int value) {
	for (int i = 0; i < GameInputEventsCount; i++) {
		if (m_binding[i].Type == type && m_binding[i].Value == value)
			m_event[i] = down;
	}
}

void InputHandler::process() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			processEvent(Keyboard, true, event.key.keysym.sym);
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

const char* InputHandler::getName(int event) {
	switch (event) {
	case Restart:
		return "Restart";
		break;
	case Exit:
		return "Exit";
		break;
	case Menu:
		return "Menu";
		break;
	case MenuClickA:
		return "MenuClickA";
		break;
	case MenuClickB:
		return "MenuClickB";
		break;
	case ToggleLight:
		return "Toggle Light";
		break;
	case ToggleLaser:
		return "Toggle Laser";
		break;
	case ShowChar:
		return "Show Char";
		break;
	case Pause:
		return "Pause";
		break;
	case MoveLeft:
		return "Move Left";
		break;
	case MoveRight:
		return "Move Right";
		break;
	case MoveUp:
		return "Move Up";
		break;
	case MoveDown:
		return "Move Down";
		break;
	case Help:
		return "Help";
		break;
	case Pickup:
		return "Pick Up";
		break;
	case ThrowGrenade:
		return "Throw Grenade";
		break;
	case Fire:
		return "Fire";
		break;
	case Reload:
		return "Reload";
		break;
	case Teleport:
		return "Teleport";
		break;
	case GameInputEventsCount:
		return "GameInputEventsCount";
		break;
	default:
		return "Uknown event";
		break;
	}
}

const char* InputHandler::getKeyName(Binding bind) {
	if (bind.Type == InputHandler::Keyboard)
		return SDL_GetKeyName(SDLKey(bind.Value));
	else if (bind.Type == InputHandler::Mouse)
		switch (bind.Value) {
		default:
		case SDL_BUTTON_LEFT:
			return "left mouse button";
			break;
		case SDL_BUTTON_RIGHT:
			return "right mouse button";
			break;
		case SDL_BUTTON_MIDDLE:
			return "middle mouse button";
			break;
		}

	return NULL;
}

