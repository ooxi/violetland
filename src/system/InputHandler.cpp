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

const char* InputHandler::getEventName(int eventNumber) {
	switch (eventNumber) {
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
		return "Unknown event";
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

