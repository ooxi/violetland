#include "MonsterFactory.h"

Sprite* MonsterFactory::loadMonsterSprite(unsigned int index, string animType) {
	vector<SDL_Surface*> animSurfaces;

	char *buf;
	sprintf(buf = new char[100], "%i/%s/", index, animType.c_str());
	unsigned int framesCount = m_fileUtility->getFilesCountFromDir(
			m_fileUtility->getFullPath(FileUtility::monsters, buf));
	delete[] buf;

	fprintf(stdout, "Monster %i, animation of %s, frames count: %i.\n", index,
			animType.c_str(), framesCount);

	for (unsigned i = 0; i < framesCount; i++) {
		sprintf(buf = new char[100], "%i/%s/%i.png", index, animType.c_str(), i);
		SDL_Surface *surface = ImageUtility::loadImage(
				m_fileUtility->getFullPath(FileUtility::monsters, buf));
		animSurfaces.push_back(surface);
		delete[] buf;
	}

	Sprite *monsterSprite = new Sprite(animSurfaces);
	return monsterSprite;
}

Sound* MonsterFactory::loadMonsterSound(unsigned int index) {
	char *buf;
	sprintf(buf = new char[100], "%i/hit.ogg", index);
	Sound* snd = m_sndManager->create(m_fileUtility->getFullPath(
			FileUtility::monsters, buf));
	delete[] buf;
	return snd;
}

MonsterFactory::MonsterFactory(FileUtility* fileUtility,
		SoundManager* sndManager) {
	printf("Loading monsters...\n");

	m_fileUtility = fileUtility;
	m_sndManager = sndManager;

	unsigned int monstersCount = m_fileUtility->getSubDirsCountFromDir(
			m_fileUtility->getFullPath(FileUtility::monsters, ""));

	fprintf(stdout, "Total monsters found: %i\n", monstersCount);

	for (unsigned int j = 0; j < monstersCount; j++) {
		m_moveSprites.push_back(loadMonsterSprite(j, "walk"));
		m_deathSprites.push_back(loadMonsterSprite(j, "death"));
		m_hitSounds.push_back(loadMonsterSound(j));
	}

	fprintf(stdout, "Loading of monsters is completed.\n");
}

Enemy* MonsterFactory::create(int baseLvl, int lvl, float* param) {
	float scale = pow((float) lvl / baseLvl, 0.2f);

	if (lvl > 1)
		for (int i = 0; i < lvl; i++) {
			int s = (rand() % 299) / 100;
			param[s] += 0.1f;
		}

	float hi = 0.0f;
	for (int i = 0; i < 3; i++) {
		if (param[i] > hi)
			hi = param[i];
	}

	int monsterIndex = param[1] + 0.3f > (param[0] + param[1] + param[2])
			/ 3.0f ? 1 : 0;

	Enemy *newMonster = new Enemy(m_moveSprites[monsterIndex],
			m_deathSprites[monsterIndex], m_hitSounds[monsterIndex]);

	newMonster->Strength = param[0];
	newMonster->Agility = param[1];
	newMonster->Vitality = param[2];

	newMonster->HitR = 0.3;

	newMonster->RMask = newMonster->Vitality / hi;
	newMonster->GMask = newMonster->Strength / hi;
	newMonster->BMask = newMonster->Agility / hi * 0.7f;
	newMonster->Scale = scale;

	newMonster->setHealth(newMonster->MaxHealth());
	newMonster->Speed = newMonster->MaxSpeed();

	return newMonster;
}

MonsterFactory::~MonsterFactory() {
	for (unsigned int i = 0; i < m_moveSprites.size(); i++) {
		delete m_moveSprites[i];
	}
	m_moveSprites.clear();

	for (unsigned int i = 0; i < m_deathSprites.size(); i++) {
		delete m_deathSprites[i];
	}
	m_deathSprites.clear();
	for (unsigned int i = 0; i < m_hitSounds.size(); i++) {
		delete m_hitSounds[i];
	}
	m_hitSounds.clear();
}
