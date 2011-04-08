#include <sstream>

#include "WeaponManager.h"

namespace violetland {

WeaponManager::WeaponManager(FileUtility* fileUtility, SoundManager* sndManager) {
	std::cout << "Loading weapons..." << std::endl;

	m_fileUtility = fileUtility;
	m_sndManager = sndManager;

	std::vector<std::string> weapons = m_fileUtility->getSubDirsFromDir(
			m_fileUtility->getFullPath(FileUtility::weapon, "."));

	std::cout << "Total weapons found: " << weapons.size() << std::endl;

	if (weapons.size() == 0) {
		std::cout << "Couldn't load weapons, the program won't run!"
				<< std::endl;
		exit(6);
	}

	for (int i = 0; i < (int) weapons.size(); i++) {
		if (weapons.at(i).compare("PM") == 0) {
			weapons.at(i) = weapons.back();
			weapons.back() = "PM";
			std::cout << "EDIT: Putting PM to the end of list!" << std::endl;
			break;
		}
	}

	for (unsigned int j = 0; j < weapons.size(); j++) {
		Texture* wImage = new Texture(ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::weapon, weapons[j]
						+ "/image.png")), GL_TEXTURE_2D, GL_LINEAR, true);

		Texture* pImage = new Texture(ImageUtility::loadImage(
				fileUtility->getFullPath(FileUtility::weapon, weapons[j]
						+ "/player.png")), GL_TEXTURE_2D, GL_LINEAR, true);

		Weapon *weapon = new Weapon(wImage, pImage, sndManager->create(
				fileUtility->getFullPath(FileUtility::weapon, weapons[j]
						+ "/shot.ogg")), sndManager->create(
				fileUtility->getFullPath(FileUtility::weapon, weapons[j]
						+ "/reload.ogg")));

		weapon->Name = weapons[j];

		std::ifstream in;
		in.open(m_fileUtility->getFullPath(FileUtility::weapon, weapons[j]
				+ "/stats").c_str());
		if (!in) {
			std::cerr << "Couldn't load stats of weapon " << weapons[j] << '.'
					<< std::endl;
			exit(4);
		}

		std::string shellName;
		std::string strbuf;

		getline(in, strbuf, ' ');
		weapon->Type = (BulletType) strtol(strbuf.c_str(), NULL, 10);

		if (weapon->Type == BULLET_FLAME)
			weapon->setBulletImage(new Texture(ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::weapon, weapons[j]
							+ "/bullet.png")), GL_TEXTURE_2D, GL_LINEAR, true));

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

		in.close();

		std::vector<SDL_Surface*> animSurfaces;

		std::string shellDir = "shells/" + shellName;
		unsigned int framesCount = fileUtility->getFilesCountFromDir(
				fileUtility->getFullPath(FileUtility::anima, shellDir));

		std::cout << "Shell animation of " << weapons[j] << " - " << shellName
				<< ", frames count: " << framesCount << '.' << std::endl;

		shellDir += '/';
		for (unsigned i = 0; i < framesCount; i++) {
			std::ostringstream filename;
			filename << i << ".png";

			SDL_Surface *surface = ImageUtility::loadImage(
					fileUtility->getFullPath(FileUtility::anima, shellDir
							+ filename.str()));

			animSurfaces.push_back(surface);
		}
		weapon->ShellSprite = new Sprite(animSurfaces);

		Weapons.push_back(weapon);
	}

	std::cout << "Loading of weapons is completed." << std::endl;
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

}
