#include "Highscores.h"
#include "../system/utility/Templates.h"

using namespace std;
using namespace boost;
using namespace violetland;

HighscoresEntry::HighscoresEntry() {
	Agility = 0;
	Strength = 0;
	Vitality = 0;
	Time = 0;
	Xp = 0;
	Name = new string();
}

HighscoresEntry::HighscoresEntry(Player* player, int Time) {
	Agility = player->Agility;
	Strength = player->Strength;
	Vitality = player->Vitality;
	Time = Time;
	Xp = player->Xp;
	Name = new string();
}

HighscoresEntry::~HighscoresEntry() {
	delete Name;
}

Highscores::Highscores(FileUtility* fileUtility):
	m_fileUtility(fileUtility), 
	hsFile(m_fileUtility->getFullPath(FileUtility::user, "highscores")), 
	hsTempFile(m_fileUtility->getFullPath(FileUtility::user, "highscores~")) {
	
	read();
}

void Highscores::read() {
	filesystem::ifstream ifile(hsFile, ios::binary);
	if (!ifile.fail()) {
		while (true) {
			HighscoresEntry* p = new HighscoresEntry();
			int size;
			ifile.read(reinterpret_cast<char*> (p), sizeof(*p));
			if (ifile.eof())
				break;
			ifile.read(reinterpret_cast<char*> (&size), sizeof(int));
			if (ifile.eof())
				break;
			char* name = (char*) malloc((size + 1) * sizeof(char));
			ifile.read(reinterpret_cast<char*> (name), size);
			delete p->Name;
			p->Name = new string(name);
			free(name);
			if (ifile.eof())
				break;
			m_data.push_back(p);
		}
		ifile.close();
	} else {
		std::cout << "Can't open file with scores." << std::endl;
	}
}

bool Highscores::isHighscore(HighscoresEntry* entry) {
	bool placed = false;
	for (unsigned int i = 0; i < m_data.size(); i++) {
		if (entry->Xp > m_data[i]->Xp) {
			placed = true;
			break;
		}
	}

	if ((!placed) && (m_data.size() < 10)) {
		placed = true;
	}
	return placed;
}

void Highscores::clear() {
	filesystem::remove(hsFile);
}

bool Highscores::add(HighscoresEntry* entry) {
	bool placed = false;
	for (unsigned int i = 0; i < m_data.size(); i++) {
		if (entry->Xp > m_data[i]->Xp) {
			m_data.insert(m_data.begin() + i, entry);
			placed = true;
			break;
		}
	}

	if ((!placed) && (m_data.size() < 10)) {
		m_data.push_back(entry);
		placed = true;
	}

	if (!placed)
		return false;

	filesystem::ofstream ofile(hsTempFile, ios::binary);
	if (!ofile.fail()) {
		for (unsigned int i = 0; i < (m_data.size() < 10 ? m_data.size() : 10); i++) {
			string temp = *(m_data[i]->Name);
			m_data[i]->Name = NULL;
			ofile.write(reinterpret_cast<char*> (m_data[i]), sizeof(*entry));
			int a = strlen(temp.c_str()) + 1;
			ofile.write(reinterpret_cast<char*> (&a), sizeof(int));
			ofile.write(reinterpret_cast<const char*> (temp.c_str()), a);
		}
		ofile.close();

		filesystem::remove(hsFile);

		FileUtility::copyFile(hsTempFile, hsFile);

		filesystem::remove(hsTempFile);

		std::cout << "Scores was updated." << std::endl;
	} else {
		std::cout << "Can't write scores to file." << std::endl;
	}

	for (unsigned int i = 0; i < m_data.size(); i++) {
		if (m_data[i] != entry)
			delete m_data[i];
	}
	m_data.clear();

	read();

	return true;
}

vector<HighscoresEntry*> Highscores::getData() {
	return m_data;
}

Highscores::~Highscores() {
	clearVector<HighscoresEntry*>(&m_data);
}
