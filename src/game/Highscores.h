#include <fstream>
#include "../system/utility/FileUtility.h"
#include "Player.h"

#ifndef HIGHSCORES_H_
#define HIGHSCORES_H_

class Highscores {
private:
	vector<Player*> m_data;
	FileUtility* m_fileUtility;
public:
	Highscores(FileUtility* fileUtility);
	void read();
	void add(Player* player);
	std::vector<Player*> getData();
	~Highscores();
};

#endif /* HIGHSCORES_H_ */
