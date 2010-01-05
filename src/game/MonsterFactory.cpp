#include "MonsterFactory.h"

Sprite* MonsterFactory::loadMonsterSprite(unsigned int index, string animType) {
	vector<SDL_Surface*> animSurfaces;

	char *buf;
	sprintf(buf = new char[100], "monsters/%i/%s/", index, animType.c_str());
	unsigned int framesCount = m_fileUtility->getFilesCountFromDir(
			m_fileUtility->getFullPath(FileUtility::anima, buf));
	delete[] buf;

	fprintf(stdout, "\tMonster %i, animation of %s, frames count: %i.\n",
			index, animType.c_str(), framesCount);

	for (unsigned i = 0; i < framesCount; i++) {
		sprintf(buf = new char[100], "monsters/%i/%s/%i.png", index,
				animType.c_str(), i);
		SDL_Surface *surface = ImageUtility::loadImage(
				m_fileUtility->getFullPath(FileUtility::anima, buf));
		animSurfaces.push_back(surface);
		delete[] buf;
	}

	Sprite *monsterSprite = new Sprite(animSurfaces);
	return monsterSprite;
}

MonsterFactory::MonsterFactory(FileUtility* fileUtility) {
	printf("Loading monsters...\n");

	m_fileUtility = fileUtility;

	unsigned int monstersCount = fileUtility->getSubDirsCountFromDir(
			fileUtility->getFullPath(FileUtility::anima, "monsters/"));

	fprintf(stdout, "Total monsters found: %i\n", monstersCount);

	for (unsigned int j = 0; j < monstersCount; j++) {
		MoveSprites.push_back(loadMonsterSprite(j, "walk"));
		DeathSprites.push_back(loadMonsterSprite(j, "death"));
	}

	fprintf(stdout, "Loading of monsters is completed.\n");
}

MonsterFactory::~MonsterFactory() {
	for (unsigned int i = 0; i < MoveSprites.size(); i++) {
		delete MoveSprites[i];
	}
	MoveSprites.clear();

	for (unsigned int i = 0; i < DeathSprites.size(); i++) {
		delete DeathSprites[i];
	}
	DeathSprites.clear();
}
