#include "Weapon.h"

Weapon::Weapon(std::string bulletImage, std::string droppedImage,
		std::string shotSound, std::string reloadSound) {
	m_bulletTex = new Texture(ImageUtility::loadImage(bulletImage),
			GL_TEXTURE_2D, GL_LINEAR, true);
	m_droppedTex = new Texture(ImageUtility::loadImage(droppedImage),
			GL_TEXTURE_2D, GL_LINEAR, true);
	m_shotSound = Mix_LoadWAV(shotSound.c_str());
	m_reloadSound = Mix_LoadWAV(reloadSound.c_str());
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

std::vector<Bullet*> *Weapon::fire() {
	std::vector<Bullet*> *newBullets = new std::vector<Bullet*>();

	if (m_reload > 0 || m_fireDelay > 0 || Ammo < 1)
		return newBullets;

	for (int i = 0; i < BulletsAtOnce; i++) {
		Bullet *newBullet = new Bullet(0, 0, m_bulletTex);
		newBullet->Damage = Damage;
		newBullet->Speed = BulletSpeed;
		newBullet->MaxRange = FireRange;
		newBullet->HitR = 0.1f;

		newBullets->push_back(newBullet);
	}

	Mix_PlayChannel(-1, m_shotSound, 0);
	Ammo--;
	m_fireDelay = FireDelayTime;

	return newBullets;
}

bool Weapon::reload(float timeMod) {
	if (m_reload <= 0) {
		if (m_reloadSndCh == -1 || Mix_Playing(m_reloadSndCh) == 0) {
			m_reloadSndCh = Mix_PlayChannel(-1, m_reloadSound, 0);
		}
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
	Mix_FreeChunk(m_shotSound);
	Mix_FreeChunk(m_reloadSound);
}

Weapon::~Weapon() {
	if (m_reloadSndCh != -1 && Mix_Playing(m_reloadSndCh) != 0) {
		Mix_HaltChannel(m_reloadSndCh);
	}
}
