#include "InputHandler.h"

InputHandler::InputHandler() {
	printf("InputHandler...\n");

	for (int i = 0; i < GameInputEventsCount; i++) {
		m_event[i] = false;
	}

	m_binding[MoveLeft].Value = SDLK_a;
	m_binding[MoveUp].Value = SDLK_w;
	m_binding[MoveRight].Value = SDLK_d;
	m_binding[MoveDown].Value = SDLK_s;
	m_binding[Restart].Value = SDLK_RETURN;
	m_binding[Menu].Value = SDLK_ESCAPE;
	m_binding[Exit].Value = SDLK_F12;
	m_binding[ToggleLight].Value = SDLK_f;
	m_binding[ToggleLaser].Value = SDLK_g;
	m_binding[Pause].Value = SDLK_p;
	m_binding[ShowChar].Value = SDLK_c;
	m_binding[Help].Value = SDLK_F1;
	m_binding[Pickup].Value = SDLK_e;
	m_binding[ThrowGrenade].Value = SDLK_SPACE;
	m_binding[Fire].Value = SDL_BUTTON_LEFT;
	m_binding[Fire].Type = Mouse;
	m_binding[Reload].Value = SDL_BUTTON_RIGHT;
	m_binding[Reload].Type = Mouse;

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
