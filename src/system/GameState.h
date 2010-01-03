#ifndef GAMESTATE_H_
#define GAMESTATE_H_

class GameState {
public:
	GameState();
	void startSurvival();
	void endGame();
	~GameState();
	double Hardness;
	bool Works;
	bool Lost;
	bool Begun;
	bool Paused;
	float TimeOfDay;
};

#endif /* GAMESTATE_H_ */
