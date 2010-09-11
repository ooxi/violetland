#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <string>
#include <map>
#include <vector>
#include "../system/utility/Templates.h"
#include "Terrain.h"
#include "lifeforms/Player.h"
#include "Powerup.h"
#include "Explosion.h"

using namespace std;

namespace violetland {
enum GameMode {
	GAMEMODE_SURVIVAL = 0, GAMEMODE_SCENARIO
};

class GameState {
public:
	GameState();
	void start(GameMode mode);
	void end();
	void process(int deltaTime);
	LifeForm* getLifeForm(string id);
	~GameState();

	GameMode Mode;
	double Hardness;
	bool Works;
	bool Lost;
	bool HighScore;
	bool Begun;
	bool Paused;
	std::string PlayerName;
	float TimeOfDay;
	int Time;

	map<string, LifeForm*> lifeForms;
	vector<Powerup*> powerups;
	vector<Bullet*> bullets;

	vector<StaticObject*> bloodStains;
	vector<Explosion*> explosions;
	vector<ParticleSystem*> particleSystems;
	Terrain* terrain;
};
}

#endif /* GAMESTATE_H_ */
