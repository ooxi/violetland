#include "MonsterFactory.h"

violetland::MonsterFactory::MonsterFactory(FileUtility* fileUtility,
		SoundManager* sndManager) {
	std::cout << "Loading monsters..." << std::endl;

	m_fileUtility = fileUtility;
	m_sndManager = sndManager;

	filesystem::path monstersPath = 
			m_fileUtility->getFullPath(FileUtility::monsters, "");
	std::vector<std::string> monsters = 
			m_fileUtility->getSubDirsFromDir(monstersPath);

	std::cout << "Total monsters found: " << monsters.size() << std::endl;

	if (monsters.size() == 0) {
		std::cout << "Couldn't load monsters, program won't run!" << std::endl;
		exit(5);
	}

	for (unsigned int j = 0; j < monsters.size(); j++) {
		MonsterTemplate* mt = new MonsterTemplate(loadMonsterSprite(
				monsters[j], "walk"), loadMonsterSprite(monsters[j], "death"));

		filesystem::path tmp(monstersPath);
		tmp /= monsters[j];
		tmp /= "sounds";
		tmp /= "hit";
		std::vector<std::string> hitSounds = 
				m_fileUtility->getFilesFromDir(tmp);

		for (unsigned int i = 0; i < hitSounds.size(); i++) {
			mt->HitSounds.push_back(loadMonsterSound("hit", monsters[j],
					hitSounds[i]));
		}

		mt->Name = monsters[j];
		fillMonsterStats(mt, monsters[j]);
		m_monsters.push_back(mt);
	}

	std::cout << "Loading of monsters is completed." << std::endl;
}

Sprite* violetland::MonsterFactory::loadMonsterSprite(std::string name,
		std::string animType) {
	std::vector<SDL_Surface*> animSurfaces;
	
	filesystem::path animPath = 
			m_fileUtility->getFullPath(FileUtility::monsters, name) /= animType;
	
	unsigned int framesCount = 
		m_fileUtility->getFilesCountFromDir(filesystem::path(animPath));

	std::cout << "Monster " << name << ", animation of " << animType << 
		", frames count: " << framesCount << '.' << std::endl;

	for (unsigned i = 0; i < framesCount; i++) {
		ostringstream oss;
		oss << i << ".png";
		SDL_Surface *surface = 
			ImageUtility::loadImage(filesystem::path(animPath) /= oss.str());
		animSurfaces.push_back(surface);
	}

	Sprite *monsterSprite = new Sprite(animSurfaces);
	return monsterSprite;
}

Sound* violetland::MonsterFactory::loadMonsterSound(std::string soundType,
		std::string monsterName, std::string soundName) {
	string buf = monsterName + "/sounds/" + soundType + '/' + soundName;
	Sound* snd = m_sndManager->create(m_fileUtility->getFullPath(
			FileUtility::monsters, buf));
	return snd;
}

void violetland::MonsterFactory::fillMonsterStats(MonsterTemplate* t,
		std::string name) {
	filesystem::ifstream in;
	string buf = name + "/stats";
	in.open(m_fileUtility->getFullPath(FileUtility::monsters, buf));
	if (!in) {
		std::cout << "Couldn't load monster stats." << std::endl;
		exit(4);
	}
	while (in) {
		in >> t->Strength;
		in >> t->Agility;
		in >> t->Vitality;
		in >> t->WalkTime;
		in >> t->WalkDelay;
	}
	in.close();
}

violetland::Monster* violetland::MonsterFactory::create(int baseLvl, int lvl) {
	int monsterIndex = (rand() % m_monsters.size());

	Monster *newMonster = new Monster(m_monsters[monsterIndex], lvl);

	newMonster->Scale = pow((float) lvl / baseLvl, 0.2f);

	return newMonster;
}

violetland::MonsterFactory::~MonsterFactory() {
	for (unsigned int i = 0; i < m_monsters.size(); i++) {
		delete m_monsters[i];
	}
	m_monsters.clear();
}
