#ifndef WEAPONMANAGER_H_
#define WEAPONMANAGER_H_

#include <boost/filesystem/fstream.hpp>

#include "../system/sound/SoundManager.h"
#include "Weapon.h"

namespace violetland {

class WeaponManager {
public:
	FileUtility* m_fileUtility;
	SoundManager* m_sndManager;
	WeaponManager(FileUtility* fileUtility, SoundManager* sndManager);
	~WeaponManager();
	std::vector<Weapon*> Weapons;
	Weapon* getWeaponByName(std::string name);
};

}

#endif /* WEAPONMANAGER_H_ */
