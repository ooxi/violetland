#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <string>
#include <map>
#include <vector>

#include "../system/utility/Templates.h"
#include "Terrain.h"
#include "lifeforms/Player.h"
#include "Powerup.h"
#include "../system/graphic/Explosion.h"

using namespace std;

namespace violetland {
class BasePowerup;

struct Blood {
	float x, y, angle, scale;
	bool poisoned;
};

enum GameMode {
	GAMEMODE_SURVIVAL = 0, GAMEMODE_WAVES
};

class GameState {
public:
	GameState();
	void start(GameMode mode);
	void end();
	vector<violetland::Blood> processExplosion(float x, float y, float damage,
			float range, bool affectPlayer);
	void reset();
	void process(int deltaTime);
	LifeForm* getLifeForm(string id);
	~GameState();

	GameMode Mode;
	double Hardness;
	bool Works;
	bool Lost;
	bool HighScore;
	bool Begun;
	bool JustBegun;
	bool Paused;
	std::string PlayerName;
	float TimeOfDay;
	int Time;
	int GameAreaSize;

	map<string, LifeForm*> lifeForms;
	vector<BasePowerup*> powerups;
	vector<Bullet*> bullets;
};
}

#endif /* GAMESTATE_H_ */
