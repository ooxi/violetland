#include "WeaponManager.h"

WeaponManager::WeaponManager(FileUtility* fileUtility, SoundManager* sndManager) {
	printf("Loading weapons...\n");

	m_fileUtility = fileUtility;
	m_sndManager = sndManager;

	std::vector<std::string> weapons = m_fileUtility->getSubDirsFromDir(
			m_fileUtility->getFullPath(FileUtility::weapon, "."));

	fprintf(stdout, "Total weapons found: %i\n", (int) weapons.size());

	if (weapons.size() == 0) {
		printf("Couldn't load weapons, the program won't run!\n");
		exit(6);
	}

	for (unsigned int j = 0; j < weapons.size(); j++) {
		char *imagePath;
		char *shotSoundPath;
		char *reloadSoundPath;
		char *playerPath;
		sprintf(imagePath = new char[1000], "%s/image.png", weapons[j].c_str());
		sprintf(playerPath = new char[1000], "%s/player.png",
				weapons[j].c_str());
		sprintf(shotSoundPath = new char[1000], "%s/shot.ogg",
				weapons[j].c_str());
		sprintf(reloadSoundPath = new char[1000], "%s/reload.ogg",
				weapons[j].c_str());

		Texture* wImage = new Texture(ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::weapon, imagePath)),
				GL_TEXTURE_2D, GL_LINEAR, true);
		Texture* pImage = new Texture(ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::weapon, playerPath)),
				GL_TEXTURE_2D, GL_LINEAR, true);

		Weapon *weapon = new Weapon(wImage, pImage, sndManager->create(
				fileUtility->getFullPath(FileUtility::weapon, shotSoundPath)),
				sndManager->create(fileUtility->getFullPath(
						FileUtility::weapon, reloadSoundPath)));

		weapon->Name = weapons[j];

		std::ifstream in;
		char *buf;
		sprintf(buf = new char[100], "%s/stats", weapons[j].c_str());
		in.open(m_fileUtility->getFullPath(FileUtility::weapon, buf).c_str());
		delete[] buf;
		if (!in) {
			fprintf(stderr, "Couldn't load stats of weapon %s.\n",
					weapons[j].c_str());
			exit(4);
		}

		std::string shellName;
		std::string strbuf;
		while (in) {
			getline(in, strbuf, ' ');
			weapon->Type
					= (Bullet::BulletType) strtol(strbuf.c_str(), NULL, 10);
			getline(in, shellName, ' ');
			in >> weapon->AmmoClipSize;
			weapon->Ammo = weapon->AmmoClipSize;
			in >> weapon->Damage;
			in >> weapon->FireDelayTime;
			in >> weapon->ReloadTime;
			in >> weapon->FireRange;
			in >> weapon->BulletSpeed;
			in >> weapon->ReturnForce;
			in >> weapon->BulletsAtOnce;
			in >> weapon->XDiff;
			in >> weapon->YDiff;
		}

		in.close();

		std::vector<SDL_Surface*> animSurfaces;

		sprintf(buf = new char[100], "shells/%s", shellName.c_str());
		unsigned int framesCount = fileUtility->getFilesCountFromDir(
				fileUtility->getFullPath(FileUtility::anima, buf));
		delete[] buf;

		fprintf(stdout, "Shell animation of %s - %s, frames count: %i.\n",
				weapons[j].c_str(), shellName.c_str(), framesCount);

		for (unsigned i = 0; i < framesCount; i++) {
			sprintf(buf = new char[100], "shells/%s/%i.png", shellName.c_str(),
					i);

			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::anima, buf));

			animSurfaces.push_back(surface);
			delete[] buf;
		}
		weapon->ShellSprite = new Sprite(animSurfaces);

		Weapons.push_back(weapon);
	}

	fprintf(stdout, "Loading of weapons is completed.\n");
}

Weapon* WeaponManager::getWeaponByName(std::string name) {
	for (unsigned int i = 0; i < Weapons.size(); i++) {
		if (Weapons[i]->Name == name)
			return Weapons[i];
	}
	return NULL;
}

WeaponManager::~WeaponManager() {
	for (unsigned int i = 0; i < Weapons.size(); i++) {
		Weapons[i]->deleteResources();
		delete Weapons[i];
	}
	Weapons.clear();
}
