#ifndef GAMESTATE_H_
#define GAMESTATE_H_

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
	bool Begun;
	bool Paused;
	float TimeOfDay;
	int Time;
};

#endif /* GAMESTATE_H_ */
