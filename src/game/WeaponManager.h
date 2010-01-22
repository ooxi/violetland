#ifndef WEAPONMANAGER_H_
#define WEAPONMANAGER_H_

#include "../system/utility/FileUtility.h"
#include "../system/sound/SoundManager.h"
#include "Weapon.h"

class WeaponManager {
public:
	FileUtility* m_fileUtility;
	SoundManager* m_sndManager;
	WeaponManager(FileUtility* fileUtility, SoundManager* sndManager);
	~WeaponManager();
	std::vector<Weapon*> Weapons;
};

#endif /* WEAPONMANAGER_H_ */
