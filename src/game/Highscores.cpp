#include "Highscores.h"

using namespace std;

using namespace violetland;

HighscoresEntry::HighscoresEntry(float Strength, float Agility, 
	float Vitality, unsigned Xp, string Name, unsigned Time):
	Strength(Strength), Agility(Agility), Vitality(Vitality), 
	Xp(Xp), Name(Name), Time(Time) {}

HighscoresEntry::HighscoresEntry(Player* player, string Name, unsigned Time):
	Strength(player->Strength), Agility(player->Agility),
	Vitality(player->Vitality),	Xp(player->Xp), 
	Name(Name), Time(Time) {}

Highscores::Highscores(FileUtility* fileUtility):
	m_fileUtility(fileUtility), 
	hsFile(m_fileUtility->getFullPath(FileUtility::user, "highscores")), 
	hsTempFile(m_fileUtility->getFullPath(FileUtility::user, "highscores~")) {
	
	read();
}

void Highscores::read() {
	boost::filesystem::ifstream ifile(hsFile, ios::binary);
	if (!ifile.fail()) {
		while (true) {
			float Strength;
			float Agility;
			float Vitality;
			unsigned Time;
			unsigned Xp;
			
			ifile.read(reinterpret_cast<char*> (&Strength), sizeof(Strength));
			ifile.read(reinterpret_cast<char*> (&Agility), sizeof(Agility));
			ifile.read(reinterpret_cast<char*> (&Vitality), sizeof(Vitality));
			ifile.read(reinterpret_cast<char*> (&Time), sizeof(Time));
			ifile.read(reinterpret_cast<char*> (&Xp), sizeof(Xp));
				
			size_t size;
			ifile.read(reinterpret_cast<char*> (&size), sizeof(size));
			if (ifile.eof())
				break;
			if (size > 100)	// TODO: need reasonable value
				break;
				
			vector<char> Name(size + 1);
			ifile.read(&Name[0], size);
			Name[size] = '\0';
			if (ifile.eof())
				break;
				
			m_data.insert(m_data.begin(), 
				HighscoresEntry(Strength, Agility, Vitality, Xp, &Name[0], Time));
		}
		ifile.close();
		while (m_data.size() > MAX_SIZE)
			m_data.erase(m_data.begin());
	} else {
		cout << "Can't open file with scores." << endl;
	}
}

bool Highscores::isHighscore(unsigned Xp) {
	if (m_data.size() < MAX_SIZE)
		return true;
	
	if (Xp > m_data.begin()->Xp)
		return true;

	return false;
}

void Highscores::clear() {
	boost::filesystem::remove(hsFile);
}

bool Highscores::add(HighscoresEntry entry) {
	if (m_data.size() < MAX_SIZE)
		m_data.insert(entry);
	else if (*m_data.begin() < entry) {
		m_data.erase(m_data.begin());
		m_data.insert(entry);
	}
	else
		return false;
	
	
	boost::filesystem::ofstream ofile(hsTempFile, ios::binary);
	if (!ofile.fail()) {
		set<HighscoresEntry>::iterator it = m_data.begin();
		for (; it != m_data.end(); ++it) {
			const char* tmp = it->Name.c_str();
			size_t size = it->Name.size();
			ofile.write(reinterpret_cast<const char*> (&it->Strength), sizeof(it->Strength));
			ofile.write(reinterpret_cast<const char*> (&it->Agility), sizeof(it->Agility));
			ofile.write(reinterpret_cast<const char*> (&it->Vitality), sizeof(it->Vitality));
			ofile.write(reinterpret_cast<const char*> (&it->Time), sizeof(it->Time));
			ofile.write(reinterpret_cast<const char*> (&it->Xp), sizeof(it->Xp));
			
			ofile.write(reinterpret_cast<char*> (&size), sizeof(size));
			ofile.write(tmp, size);
		}
		ofile.close();

		boost::filesystem::copy_file(hsTempFile, hsFile, 
				boost::filesystem::copy_option::overwrite_if_exists);
		boost::filesystem::remove(hsTempFile);

		cout << "Scores was updated." << endl;
	} else {
		cout << "Can't write scores to file." << endl;
	}

	return true;
}
