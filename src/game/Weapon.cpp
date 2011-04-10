#include "Weapon.h"

namespace violetland {

Weapon::Weapon(Texture* droppedTex, Texture* playerTex, Sound* shotSound,
		Sound* reloadSound) {
	m_playerTex = playerTex;
	m_droppedTex = droppedTex;
	m_shotSound = shotSound;
	m_reloadSound = reloadSound;
	Damage = 1.0;
	FireDelayTime = 1000;
	ReloadTime = 1000;
	AmmoClipSize = 1;
	Ammo = 1;
	BulletSpeed = 3;
	BulletsAtOnce = 1;
	FireRange = 1000;
	ReturnForce = 0.0;
	m_fireDelay = 0;
	m_reload = 0;
	m_reloadSndCh = -1;
	m_bulletTex = NULL;
}

void Weapon::setBulletImage(Texture* _tex) {
	m_bulletTex = _tex;
}

Texture *Weapon::getDroppedTex() {
	return m_droppedTex;
}

Texture *Weapon::getPlayerTex() {
	return m_playerTex;
}

void Weapon::process(int deltaTime) {
	if (m_fireDelay > 0)
		m_fireDelay -= deltaTime;

	if (m_reload > 0) {
		m_reload -= deltaTime;

		if (m_reload <= 0)
			Ammo = AmmoClipSize;
	}
}

std::vector<Bullet*> *Weapon::fire(float x, float y, float dX, float dY) {
	std::vector<Bullet*> *newBullets = new std::vector<Bullet*>();
	if (m_reload > 0 || m_fireDelay > 0 || Ammo < 1)
		return newBullets;

	for (int i = 0; i < BulletsAtOnce; i++) {
		Bullet* newBullet;
		switch (Type) {
		case BULLET_STANDARD:
			newBullet = new StandardBullet(x, y, dX, dY, false);
			break;
		case BULLET_LASER:
			newBullet = new LaserBullet(x, y, dX, dY);
			break;
		case BULLET_GRENADE:
			newBullet = new StandardBullet(x, y, dX, dY, true);
			break;
		case BULLET_FLAME:
			newBullet = new Flame(x, y, m_bulletTex);
		}
		newBullet->Damage = Damage;
		newBullet->Speed = BulletSpeed;
		newBullet->MaxRange = FireRange;

		newBullets->push_back(newBullet);
	}

	m_shotSound->playInf(2);
	Ammo--;
	m_fireDelay = FireDelayTime;

	return newBullets;
}

bool Weapon::reload(float timeMod) {
	if (m_reload <= 0) {
		m_reloadSound->play(3, 0, 0);
		m_reload = (int) (ReloadTime * timeMod);
		return true;
	} else
		return false;
}

float Weapon::getReloadState() {
	if (m_reload > 0)
		return 1.0f - (float) m_reload / ReloadTime;
	else
		return 0;
}

void Weapon::deleteResources() {
	if (m_bulletTex)
		delete m_bulletTex;
	delete m_droppedTex;
	delete m_playerTex;
	delete ShellSprite;
	delete m_shotSound;
	delete m_reloadSound;
}

Weapon::~Weapon() {
	if (m_reloadSndCh != -1 && Mix_Playing(m_reloadSndCh) != 0) {
		Mix_HaltChannel(m_reloadSndCh);
	}
}

}
