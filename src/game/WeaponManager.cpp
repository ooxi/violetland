#include <sstream>

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

	for (int i = 0; i < (int) weapons.size(); i++) {
		if (weapons.at(i).compare("PM") == 0) {
			weapons.at(i) = weapons.back();
			weapons.back() = "PM";
			fprintf(stdout, "EDIT: Putting PM to the end of list!\n");
			break;
		}
	}

	for (unsigned int j = 0; j < weapons.size(); j++) {
		std::string buf;
		
		/*imagePath += "/image.png";
		shotSoundPath += "/shot.png";
		reloadSoundPath += "/reload.png";
		playerPath += "/player.png";
		statsPath += "/stats";*/

		buf = weapons[j];
		Texture* wImage = new Texture(ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::weapon, buf += "/image.png")),
				GL_TEXTURE_2D, GL_LINEAR, true);
				
		buf = weapons[j];		
		Texture* pImage = new Texture(ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::weapon, buf += "/player.png")),
				GL_TEXTURE_2D, GL_LINEAR, true);
				
		buf = weapons[j];
		Weapon *weapon = new Weapon(wImage, pImage, sndManager->create(
				fileUtility->getFullPath(FileUtility::weapon, buf += "/shot.ogg")),
				sndManager->create(fileUtility->getFullPath(
						FileUtility::weapon, std::string(buf) += "/reload.ogg")));

		weapon->Name = weapons[j];

		buf = weapons[j];
		std::ifstream in;
		in.open(m_fileUtility->getFullPath(FileUtility::weapon, buf += "/stats").c_str());
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

		buf = "shells/";
		unsigned int framesCount = fileUtility->getFilesCountFromDir(
				fileUtility->getFullPath(FileUtility::anima, buf += shellName));

		fprintf(stdout, "Shell animation of %s - %s, frames count: %i.\n",
				weapons[j].c_str(), shellName.c_str(), framesCount);
		
		buf += '/';
		for (unsigned i = 0; i < framesCount; i++) {
			std::ostringstream oss;
			oss << i << ".png";
			std::string tmp = buf;

			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::anima, tmp += oss.str()));

			animSurfaces.push_back(surface);
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
