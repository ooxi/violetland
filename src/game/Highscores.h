#include <fstream>
#include <vector>
#include "../system/utility/FileUtility.h"

#ifndef HIGHSCORES_H_
#define HIGHSCORES_H_

class HighscoresEntry {
public:
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
	void read();
	void add(HighscoresEntry* entry);
	std::vector<HighscoresEntry*> getData();
	~Highscores();
};

#endif /* HIGHSCORES_H_ */
