#ifndef HIGHSCORES_H_
#define HIGHSCORES_H_

#include "../system/utility/FileUtility.h"
#include "lifeforms/Player.h"

//class Player;

class HighscoresEntry {
public:
	HighscoresEntry();
	~HighscoresEntry();
	HighscoresEntry(violetland::Player* player, int Time);
	std::string *Name;
	float Strength;
	float Agility;
	float Vitality;
	int Time;
	int Xp;
};

class Highscores {
private:
	std::vector<HighscoresEntry*> m_data;
	FileUtility* m_fileUtility;
public:
	Highscores(FileUtility* fileUtility);
	void clear();
	void read();
	bool isHighscore(HighscoresEntry* entry);
	bool add(HighscoresEntry* entry);
	std::vector<HighscoresEntry*> getData();
	~Highscores();
};

#endif /* HIGHSCORES_H_ */
