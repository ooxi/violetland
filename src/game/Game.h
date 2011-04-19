#ifndef GAME_H_
#define GAME_H_

#include "GameState.h"
#include "HUD.h"
#include "../system/InputHandler.h"

namespace violetland {
class Game {
public:
	InputHandler*& input;
	HUD*& hud;
	Configuration*& config;
	GameState*& gameState;
	Resources*& resources;
	vector<ParticleSystem*>& particleSystems;
	Game(InputHandler*& input, HUD*& hud, Configuration*& config, 
		GameState*& gameState, Resources*& resources, 
		vector<ParticleSystem*>& particleSystems):
		input(input), hud(hud), config(config), gameState(gameState),
		resources(resources), particleSystems(particleSystems) {}
};
}

#endif /* GAME_H_ */
