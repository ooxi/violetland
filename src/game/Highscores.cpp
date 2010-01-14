#include "Highscores.h"

Highscores::Highscores(FileUtility* fileUtility) {
	m_fileUtility = fileUtility;

	read();
}

void Highscores::read() {
	std::string hsFile = m_fileUtility->getFullPath(FileUtility::user,
			"highscores");

	std::ifstream ifile(hsFile.c_str(), std::ios::binary);
	if (!ifile.fail()) {
		while (!ifile.eof()) {
			HighscoresEntry* p = new HighscoresEntry();
			ifile.read(reinterpret_cast<char*> (p), sizeof(*p));
			m_data.push_back(p);
		}
		ifile.close();
	} else {
		printf("Can't open file with scores.\n");
	}
}

void Highscores::add(HighscoresEntry* entry) {
	std::string hsFile = m_fileUtility->getFullPath(FileUtility::user,
			"highscores");
	std::string hsTempFile = m_fileUtility->getFullPath(FileUtility::user,
			"highscores~");

	bool placed = false;
	for (unsigned int i = 0; i < m_data.size(); i++) {
		if (entry->Xp > m_data[i]->Xp) {
			m_data.insert(m_data.begin() + i, entry);
			placed = true;
			break;
		}
	}

	if (!placed)
		m_data.push_back(entry);

	std::ofstream ofile(hsTempFile.c_str(), std::ios::binary);
	if (!ofile.fail()) {
		for (unsigned int i = 0; i < (m_data.size() < 10 ? m_data.size() : 10); i++) {
			ofile.write(reinterpret_cast<char*> (m_data[i]), sizeof(*entry));
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
}

std::vector<HighscoresEntry*> Highscores::getData() {
	return m_data;
}

Highscores::~Highscores() {
	for (unsigned int i = 0; i < m_data.size(); i++) {
		delete m_data[i];
	}
	m_data.clear();
}
