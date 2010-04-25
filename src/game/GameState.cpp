#include "GameState.h"

GameState::GameState() {
	TimeOfDay = 1.0;
	Begun = false;
	Works = true;
	Lost = false;
	Time = 0;
}

void GameState::start(GameMode mode) {
	switch (mode) {
	case GameState::Survival:
		Hardness = 9995.0;
		Lost = false;
		Paused = false;
		Begun = true;
		Time = 0;
		break;
	case GameState::Scenario:
		break;
	}
}

void GameState::end() {
	Works = false;
}

GameState::~GameState() {
	// nothing
}
