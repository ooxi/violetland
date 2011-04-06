#include "Highscores.h"
#include "../system/utility/Templates.h"

using namespace violetland;
using namespace std;

HighscoresEntry::HighscoresEntry() {
	this->Agility = 0;
	this->Strength = 0;
	this->Vitality = 0;
	this->Time = 0;
	this->Xp = 0;
	this->Name = new string();
}

HighscoresEntry::HighscoresEntry(Player* player, int Time) {
	this->Agility = player->Agility;
	this->Strength = player->Strength;
	this->Vitality = player->Vitality;
	this->Time = Time;
	this->Xp = player->Xp;
	this->Name = new string();
}

HighscoresEntry::~HighscoresEntry() {
	delete Name;
}

Highscores::Highscores(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

	read();
}

void Highscores::read() {
	string hsFile = m_fileUtility->getFullPath(FileUtility::user, "highscores");

	ifstream ifile(hsFile.c_str(), ios::binary);
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
		printf("Can't open file with scores.\n");
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
	std::string hsFile = m_fileUtility->getFullPath(FileUtility::user,
			"highscores");

	remove(hsFile.c_str());
}

bool Highscores::add(HighscoresEntry* entry) {
	string hsFile = m_fileUtility->getFullPath(FileUtility::user, "highscores");
	string hsTempFile = m_fileUtility->getFullPath(FileUtility::user,
			"highscores~");

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

	ofstream ofile(hsTempFile.c_str(), ios::binary);
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

		remove(hsFile.c_str());

		FileUtility::copyFile(hsTempFile.c_str(), hsFile.c_str());

		remove(hsTempFile.c_str());

		printf("Scores was updated.\n");
	} else {
		printf("Can't write scores to file.\n");
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
