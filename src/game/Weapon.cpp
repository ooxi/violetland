#include "Weapon.h"

Weapon::Weapon(Bullet::BulletType type, std::string droppedImage,
		Sound* shotSound, Sound* reloadSound) {
	m_droppedTex = new Texture(ImageUtility::loadImage(droppedImage),
			GL_TEXTURE_2D, GL_LINEAR, true);
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
	Type = type;
}

void Weapon::setBulletImage(std::string image) {
	m_bulletTex = new Texture(ImageUtility::loadImage(image), GL_TEXTURE_2D,
			GL_LINEAR, true);
}

Texture *Weapon::getDroppedTex() {
	return m_droppedTex;
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

std::vector<Bullet*> *Weapon::fire(float x, float y) {
	std::vector<Bullet*> *newBullets = new std::vector<Bullet*>();

	if (m_reload > 0 || m_fireDelay > 0 || Ammo < 1)
		return newBullets;

	for (int i = 0; i < BulletsAtOnce; i++) {
		Bullet* newBullet;
		switch (Type) {
		case Bullet::standard:
			newBullet = new StandardBullet(x, y);
			break;
		case Bullet::laser:
			newBullet = new LaserBullet(x, y);
			break;
		}
		newBullet->Damage = Damage;
		newBullet->Speed = BulletSpeed;
		newBullet->MaxRange = FireRange;

		newBullets->push_back(newBullet);
	}

	m_shotSound->playInf();
	Ammo--;
	m_fireDelay = FireDelayTime;

	return newBullets;
}

bool Weapon::reload(float timeMod) {
	if (m_reload <= 0) {
		m_reloadSound->play();
		m_reload = ReloadTime * timeMod;
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
	delete m_bulletTex;
	delete m_droppedTex;
	delete m_shotSound;
	delete m_reloadSound;
}

Weapon::~Weapon() {
	if (m_reloadSndCh != -1 && Mix_Playing(m_reloadSndCh) != 0) {
		Mix_HaltChannel(m_reloadSndCh);
	}
}
