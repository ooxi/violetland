#ifndef VIOLET_GAME_H_
#define VIOLET_GAME_H_

#include <vector>

#include "GameState.h"
#include "HUD.h"
#include "../system/InputHandler.h"

namespace violet {
class Game {
public:
	InputHandler*& input;
	HUD*& hud;
	Configuration*& config;
	GameState*& gameState;
	Resources*& resources;
	std::vector<ParticleSystem*>& particleSystems;
	Game(InputHandler*& input, HUD*& hud, Configuration*& config, 
		GameState*& gameState, Resources*& resources, 
		std::vector<ParticleSystem*>& particleSystems):
		input(input), hud(hud), config(config), gameState(gameState),
		resources(resources), particleSystems(particleSystems) {}
};
}

#endif /* VIOLET_GAME_H_ */
