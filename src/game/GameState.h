#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <string>

class GameState {
public:
	enum GameMode {
		Survival = 0, Scenario
	};
	GameState();
	void start(GameMode mode);
	void end();
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
};

#endif /* GAMESTATE_H_ */
