#include "MonsterFactory.h"

Sprite* MonsterFactory::loadMonsterSprite(string name, string animType) {
	vector<SDL_Surface*> animSurfaces;

	char *buf;
	sprintf(buf = new char[100], "%s/%s/", name.c_str(), animType.c_str());
	unsigned int framesCount = m_fileUtility->getFilesCountFromDir(
			m_fileUtility->getFullPath(FileUtility::monsters, buf));
	delete[] buf;

	fprintf(stdout, "Monster %s, animation of %s, frames count: %i.\n",
			name.c_str(), animType.c_str(), framesCount);

	for (unsigned i = 0; i < framesCount; i++) {
		sprintf(buf = new char[100], "%s/%s/%i.png", name.c_str(),
				animType.c_str(), i);
		SDL_Surface *surface = ImageUtility::loadImage(
				m_fileUtility->getFullPath(FileUtility::monsters, buf));
		animSurfaces.push_back(surface);
		delete[] buf;
	}

	Sprite *monsterSprite = new Sprite(animSurfaces);
	return monsterSprite;
}

Sound* MonsterFactory::loadMonsterSound(string soundType, string monsterName,
		string soundName) {
	char *buf;
	sprintf(buf = new char[100], "%s/sounds/%s/%s", monsterName.c_str(),
			soundType.c_str(), soundName.c_str());
	Sound* snd = m_sndManager->create(m_fileUtility->getFullPath(
			FileUtility::monsters, buf));
	delete[] buf;
	return snd;
}

void MonsterFactory::fillMonsterStats(MonsterTemplate* t, string name) {
	ifstream in;
	char *buf;
	sprintf(buf = new char[100], "%s/stats", name.c_str());
	in.open(m_fileUtility->getFullPath(FileUtility::monsters, buf).c_str());
	delete[] buf;
	if (!in) {
		fprintf(stderr, "Couldn't load monster stats.\n");
		exit(4);
	}
	while (in) {
		in >> t->Strength;
		in >> t->Agility;
		in >> t->Vitality;
	}
	in.close();
}

MonsterFactory::MonsterFactory(FileUtility* fileUtility,
		SoundManager* sndManager) {
	printf("Loading monsters...\n");

	m_fileUtility = fileUtility;
	m_sndManager = sndManager;

	vector<string> monsters = m_fileUtility->getSubDirsFromDir(
			m_fileUtility->getFullPath(FileUtility::monsters, "."));

	fprintf(stdout, "Total monsters found: %i\n", (int) monsters.size());

	if (monsters.size() == 0) {
		printf("Couldn't load monsters, program won't run!\n");
		exit(5);
	}

	for (unsigned int j = 0; j < monsters.size(); j++) {
		MonsterTemplate* mt = new MonsterTemplate(loadMonsterSprite(
				monsters[j], "walk"), loadMonsterSprite(monsters[j], "death"));

		char *buf;
		sprintf(buf = new char[100], "%s/sounds/hit/", monsters[j].c_str());
		vector<string> hitSounds = m_fileUtility->getFilesFromDir(
				m_fileUtility->getFullPath(FileUtility::monsters, buf));
		delete[] buf;

		for (unsigned int i = 0; i < hitSounds.size(); i++) {
			mt ->HitSounds.push_back(loadMonsterSound("hit", monsters[j],
					hitSounds[i]));
		}

		mt->Name = monsters[j];
		fillMonsterStats(mt, monsters[j]);
		m_monsters.push_back(mt);
	}

	fprintf(stdout, "Loading of monsters is completed.\n");
}

Enemy* MonsterFactory::create(int baseLvl, int lvl) {
	int monsterIndex = (rand() % m_monsters.size());

	Enemy *newMonster = new Enemy(m_monsters[monsterIndex], lvl);

	newMonster->Scale = pow((float) lvl / baseLvl, 0.2f);

	return newMonster;
}

MonsterFactory::~MonsterFactory() {
	for (unsigned int i = 0; i < m_monsters.size(); i++) {
		delete m_monsters[i];
	}
	m_monsters.clear();
}
