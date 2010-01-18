#include "WeaponManager.h"

WeaponManager::WeaponManager(FileUtility* fileUtility, SoundManager* sndManager) {
	printf("Loading weapons...\n");

	ifstream in;
	in.open(fileUtility->getFullPath(FileUtility::common, "weapons").c_str());
	if (!in) {
		fprintf(stderr, "Couldn't load weapons list.\n");
		exit(4);
	}
	while (in) {
		int weaponType;
		in >> weaponType;
		char bulletPath[2000] = "";
		char droppedImagePath[2000] = "";
		char shotSound[2000] = "";
		char reloadSound[2000] = "";
		char shellSpriteFolder[2000] = "";
		char name[30] = "";
		in >> bulletPath;
		if (strlen(bulletPath) < 1)
			break;
		in >> droppedImagePath;
		in >> shellSpriteFolder;
		in >> shotSound;
		in >> reloadSound;
		in >> name;

		vector<SDL_Surface*> animSurfaces;

		char *buf;
		sprintf(buf = new char[100], "shells/%s/", shellSpriteFolder);
		unsigned int framesCount = fileUtility->getFilesCountFromDir(
				fileUtility->getFullPath(FileUtility::anima, buf));
		delete[] buf;

		fprintf(stdout, "Shell animation of %s, frames count: %i.\n", name,
				framesCount);

		for (unsigned i = 0; i < framesCount; i++) {
			sprintf(buf = new char[100], "shells/%s/%i.png", shellSpriteFolder,
					i);
			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::anima, buf));
			animSurfaces.push_back(surface);
			delete[] buf;
		}

		Sprite *shellSprite = new Sprite(animSurfaces);

		Weapon *weapon = new Weapon((Bullet::BulletType) weaponType,
				fileUtility->getFullPath(FileUtility::image, droppedImagePath),
				shellSprite, sndManager->create(fileUtility->getFullPath(
						FileUtility::sound, shotSound)), sndManager->create(
						fileUtility->getFullPath(FileUtility::sound,
								reloadSound)));
		if (weaponType > 2) {
			weapon->setBulletImage(fileUtility->getFullPath(FileUtility::image,
					bulletPath));
		}

		weapon->Name = name;
		in >> weapon->AmmoClipSize;
		weapon->Ammo = weapon->AmmoClipSize;
		in >> weapon->Damage;
		in >> weapon->FireDelayTime;
		in >> weapon->ReloadTime;
		in >> weapon->FireRange;
		in >> weapon->BulletSpeed;
		in >> weapon->ReturnForce;
		in >> weapon->BulletsAtOnce;

		Weapons.push_back(weapon);
	}
	in.close();
}

WeaponManager::~WeaponManager() {
	for (unsigned int i = 0; i < Weapons.size(); i++) {
		Weapons[i]->deleteResources();
		delete Weapons[i];
	}
	Weapons.clear();
}
