#include <stdlib.h>
#include <iostream>
#include <vector>
#include "SDL_opengl.h"
#include "SDL_mixer.h"
#include "../system/graphic/Texture.h"
#include "../system/utility/ImageUtility.h"
#include "../system/sound/Sound.h"
#include "bullets/StandardBullet.h"
#include "bullets/LaserBullet.h"
#include "bullets/GrenadeBullet.h"

#ifndef WEAPON_H_
#define WEAPON_H_

class Weapon {
private:
	int m_fireDelay;
	int m_reload;
	Texture *m_droppedTex;
	Texture *m_bulletTex;
	Sound* m_shotSound;
	Sound* m_reloadSound;
	Sprite* m_shellSprite;
	int m_reloadSndCh;
public:
	Weapon(Bullet::BulletType Type, std::string droppedImage,
			Sprite* shellSprite, Sound* shotSound, Sound* reloadSound);
	void setBulletImage(std::string image);
	void process(int deltaTime);
	std::vector<Bullet*> *fire(float x, float y, float dX, float dY);
	bool reload(float timeMod);
	void pickup();
	float getReloadState();
	void deleteResources();
	Texture *getDroppedTex();
	Sprite* getShellSprite();
	~Weapon();
	Bullet::BulletType Type;
	std::string Name;
	int FireDelayTime;
	int ReloadTime;
	int AmmoClipSize;
	int Ammo;
	float Damage;
	float BulletSpeed;
	float FireRange;
	float ReturnForce;
	int BulletsAtOnce;

	bool Poisoned;
};

#endif /* WEAPON_H_ */
