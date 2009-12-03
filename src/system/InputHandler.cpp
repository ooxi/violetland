#include "InputHandler.h"

InputHandler::InputHandler() {
	printf("InputHandler...\n");

	for (int i = 0; i < GameInputEventsCount; i++) {
		m_event[i] = false;
	}

	m_eventMap[MoveLeft] = SDLK_a;
	m_eventMap[MoveUp] = SDLK_w;
	m_eventMap[MoveRight] = SDLK_d;
	m_eventMap[MoveDown] = SDLK_s;
	m_eventMap[Restart] = SDLK_RETURN;
	m_eventMap[Menu] = SDLK_ESCAPE;
	m_eventMap[Exit] = SDLK_F12;
	m_eventMap[ToggleLight] = SDLK_f;
	m_eventMap[ToggleLaser] = SDLK_g;
	m_eventMap[Pause] = SDLK_p;
	m_eventMap[ShowChar] = SDLK_c;
	m_eventMap[Help] = SDLK_F1;
	m_eventMap[Pickup] = SDLK_e;
	m_eventMap[ThrowGrenade] = SDLK_SPACE;

	m_eventMap[Fire] = SDL_BUTTON_LEFT;
	m_eventMap[Reload] = SDL_BUTTON_RIGHT;

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

void InputHandler::process() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			for (int i = 0; i < GameInputEventsCount - 2; i++) {
				if (m_eventMap[i] == event.key.keysym.sym)
					m_event[i] = true;
			}
			break;
		case SDL_KEYUP:
			for (int i = 0; i < GameInputEventsCount - 2; i++) {
				if (m_eventMap[i] == event.key.keysym.sym)
					m_event[i] = false;
			}
			break;
		case SDL_QUIT:
			m_event[Exit] = true;
			break;
		case SDL_MOUSEMOTION:
			mouseX = event.motion.x;
			mouseY = event.motion.y;
			break;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (m_eventMap[Fire] == event.button.button)
				m_event[Fire] = true;
			if (m_eventMap[Reload] == event.button.button)
				m_event[Reload] = true;
		}
		if (event.type == SDL_MOUSEBUTTONUP) {
			if (m_eventMap[Fire] == event.button.button)
				m_event[Fire] = false;
			if (m_eventMap[Reload] == event.button.button)
				m_event[Reload] = false;
		}
	}
}
