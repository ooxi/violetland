#include <sstream>

#include "WeaponManager.h"

namespace violetland {

WeaponManager::WeaponManager(FileUtility* fileUtility, SoundManager* sndManager) {
	std::cout << "Loading weapons..." << std::endl;

	m_fileUtility = fileUtility;
	m_sndManager = sndManager;

	boost::filesystem::path weaponsPath = 
			m_fileUtility->getFullPath(FileUtility::weapon, "");
	std::vector<std::string> weapons = m_fileUtility->getSubDirsFromDir(
			weaponsPath);

	std::cout << "Total weapons found: " << weapons.size() << std::endl;

	if (weapons.size() == 0) {
		std::cout << "Couldn't load weapons, the program won't run!"
				<< std::endl;
		exit(6);
	}

	for (int i = 0; i < (int) weapons.size(); i++) {
		if (weapons.at(i) == "PM") {
			weapons.at(i) = weapons.back();
			weapons.back() = "PM";
			std::cout << "EDIT: Putting PM to the end of list!" << std::endl;
			break;
		}
	}

	for (unsigned int j = 0; j < weapons.size(); j++) {
		boost::filesystem::path weaponPath(weaponsPath);
		weaponPath /= weapons[j];
		Texture* wImage = new Texture(ImageUtility::loadImage(
				boost::filesystem::path(weaponPath) /= "image.png"), 
				GL_TEXTURE_2D, GL_LINEAR, true);

		Texture* pImage = new Texture(ImageUtility::loadImage(
				boost::filesystem::path(weaponPath) /= "player.png"), 
				GL_TEXTURE_2D, GL_LINEAR, true);

		Weapon *weapon = new Weapon(wImage, pImage, 
				sndManager->create(boost::filesystem::path(weaponPath) /= "shot.ogg"), 
				sndManager->create(boost::filesystem::path(weaponPath) /= "reload.ogg"));

		weapon->Name = weapons[j];

		boost::filesystem::ifstream in;
		in.open(boost::filesystem::path(weaponPath) /= "stats");
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
					boost::filesystem::path(weaponPath) /= "bullet.png"), 
					GL_TEXTURE_2D, GL_LINEAR, true));

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

		if (shellName != "no")
			loadShellSprite(weapon, shellName);

		Weapons.push_back(weapon);
	}

	std::cout << "Loading of weapons is completed." << std::endl;
}

void WeaponManager::loadShellSprite(Weapon* _weapon, std::string& _shellName)
{
	std::vector<SDL_Surface*> animSurfaces;

	boost::filesystem::path shellDir =
			m_fileUtility->getFullPath(FileUtility::anima, "shells");
	shellDir /= _shellName;
	unsigned framesCount =
			m_fileUtility->getFilesCountFromDir(shellDir);

	std::cout << "Shell animation of " << _weapon->Name << " - " << _shellName
			<< ", frames count: " << framesCount << '.' << std::endl;

	for (unsigned i = 0; i < framesCount; i++) {
		std::ostringstream filename;
		filename << i << ".png";

		SDL_Surface *surface =
				ImageUtility::loadImage(boost::filesystem::path(shellDir) /= filename.str());

		animSurfaces.push_back(surface);
	}

	_weapon->ShellSprite = new Sprite(animSurfaces);
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
