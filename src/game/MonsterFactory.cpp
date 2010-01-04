#include "MonsterFactory.h"

MonsterFactory::MonsterFactory(FileUtility* fileUtility) {
	for (unsigned int j = 0; j < 2; j++) {
		vector<SDL_Surface*> animSurfaces;

		for (unsigned i = 0; i < 24; i++) {
			char *buf;
			sprintf(buf = new char[100], "monsters/%i/move-%i.png", j, i);
			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::anima, buf));
			animSurfaces.push_back(surface);
			delete[] buf;
		}
		Sprite *monsterSprite = new Sprite(animSurfaces);
		MoveSprites.push_back(monsterSprite);

		animSurfaces.clear();

		for (unsigned i = 0; i < 24; i++) {
			char *buf;
			sprintf(buf = new char[100], "monsters/%i/death-%i.png", j, i);
			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::anima, buf));
			animSurfaces.push_back(surface);
			delete[] buf;
		}
		monsterSprite = new Sprite(animSurfaces);
		DeathSprites.push_back(monsterSprite);
	}
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
