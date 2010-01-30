#include "GameState.h"

GameState::GameState() {
	TimeOfDay = 1.0;
	Begun = false;
	Works = true;
	Lost = false;
	Time = 0;
}

void GameState::startSurvival() {
	Hardness = 9995.0;
	Lost = false;
	Paused = false;
	Begun = true;
	Time = 0;
}

void GameState::endGame() {
	Works = false;
}

GameState::~GameState() {
	// nothing
}
