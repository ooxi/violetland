#ifndef WEAPONMANAGER_H_
#define WEAPONMANAGER_H_

#include <boost/filesystem/fstream.hpp>

#include "../system/sound/SoundManager.h"
#include "Weapon.h"

namespace violetland {

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

#endif /* WEAPONMANAGER_H_ */
