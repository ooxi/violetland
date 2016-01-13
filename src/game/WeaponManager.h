#ifndef VIOLET_WEAPONMANAGER_H_
#define VIOLET_WEAPONMANAGER_H_

#include <boost/filesystem/fstream.hpp>

#include "../system/sound/SoundManager.h"
#include "Weapon.h"

namespace violet {

class WeaponManager {
private:
	FileUtility* m_fileUtility;
	SoundManager* m_sndManager;
public:
	std::vector<Weapon*> Weapons;

	WeaponManager(FileUtility* fileUtility, SoundManager* sndManager);
	~WeaponManager();
	void loadShellSprite(Weapon* _weapon, std::string& _shellName);
	Weapon* getWeaponByName(std::string name);
};

}

#endif /* VIOLET_WEAPONMANAGER_H_ */
