#ifndef HIGHSCORES_H_
#define HIGHSCORES_H_

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "../system/utility/FileUtility.h"
#include "lifeforms/Player.h"

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
	const FileUtility* m_fileUtility;
	const filesystem::path hsFile;
	const filesystem::path hsTempFile;
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
