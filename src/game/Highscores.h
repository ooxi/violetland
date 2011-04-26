#ifndef HIGHSCORES_H_
#define HIGHSCORES_H_

#include <set>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "../system/utility/FileUtility.h"
#include "../system/utility/Templates.h"
#include "lifeforms/Player.h"

class HighscoresEntry {
public:
	HighscoresEntry(float Strength, float Agility, float Vitality, 
			unsigned Xp, std::string Name, unsigned Time);
	HighscoresEntry(violetland::Player* player, std::string Name, 
			unsigned Time);
	const float Strength;
	const float Agility;
	const float Vitality;
	const unsigned Time;
	const unsigned Xp;
	const std::string Name;
	bool operator<(const HighscoresEntry& e) const {
		return Xp < e.Xp;
	}
};

class Highscores {
	static const size_t MAX_SIZE = 10;
private:
	std::set<HighscoresEntry> m_data;
	const FileUtility* m_fileUtility;
	const filesystem::path hsFile;
	const filesystem::path hsTempFile;
public:
	Highscores(FileUtility* fileUtility);
	void clear();
	void read();
	bool isHighscore(unsigned Time);
	bool add(HighscoresEntry entry);
	const set<HighscoresEntry>& getData() {
		return m_data;
	}
};

#endif /* HIGHSCORES_H_ */
