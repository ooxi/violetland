#include "Resources.h"

Resources::Resources(FileUtility* fileUtility, SoundManager* sndManager) {
	m_fileUtil = fileUtility;
	m_sndManager = sndManager;

	ExplSounds.push_back(loadSnd("explode-0.ogg"));
	ExplSounds.push_back(loadSnd("explode-1.ogg"));
	PlayerDeathSound = loadSnd("player_killed.ogg");

	PlayerHitSounds.push_back(loadSnd("player_hit_0.ogg"));
	PlayerHitSounds.push_back(loadSnd("player_hit_1.ogg"));
	PlayerHitSounds.push_back(loadSnd("player_hit_2.ogg"));

	PlayerWalkSprite = loadSprite(25, "player/walk/%i.png");
	PlayerDeathSprites.push_back(loadSprite(56, "player/death/0/%i.png"));
	PlayerDeathSprites.push_back(loadSprite(42, "player/death/1/%i.png"));

	GrenadeSprite = loadSprite(12, "grenade/%i.png");

	ExplTex.push_back(loadTex("expl_0.png"));
	ExplTex.push_back(loadTex("expl_1.png"));

	BloodTex.push_back(loadTex("blood_0.png"));
	BloodTex.push_back(loadTex("blood_1.png"));
	BloodTex.push_back(loadTex("blood_2.png"));

	Crystal = new StaticObject(0, 0, 128, 128, loadTex("crystal.png"), true);

	HealthIndicator = new StaticObject(0, 0, 128, 128, loadTex("health.png"),
			true);
	LevelUpIndicator = new StaticObject(0, 0, 128, 128, loadTex("levelup.png"),
			true);

	loadPowerupTex(Powerup::medikit, "medikit.png");
	loadPowerupTex(Powerup::grenades, "grenade.png");
	loadPowerupTex(Powerup::freeze, "freeze.png");
	loadPowerupTex(Powerup::penBullets, "penbullets.png");
	loadPowerupTex(Powerup::nuke, "bon_nuke.png");
}

Sprite* Resources::loadSprite(unsigned int frames, std::string pattern) {
	std::vector<SDL_Surface*> animSurfaces;
	for (unsigned i = 0; i < frames; i++) {
		char *buf;
		sprintf(buf = new char[150], pattern.c_str(), i);
		SDL_Surface *surface = ImageUtility::loadImage(m_fileUtil->getFullPath(
				FileUtility::anima, buf));
		animSurfaces.push_back(surface);
		delete[] buf;
	}
	return new Sprite(animSurfaces);
}

Sound* Resources::loadSnd(std::string fileName) {
	return m_sndManager->create(m_fileUtil->getFullPath(FileUtility::sound,
			fileName));
}

Texture* Resources::loadTex(std::string fileName) {
	return new Texture(ImageUtility::loadImage(m_fileUtil->getFullPath(
			FileUtility::image, fileName)), GL_TEXTURE_2D, GL_LINEAR, true);
}

void Resources::loadPowerupTex(Powerup::PowerupType type, std::string fileName) {
	PowerupTex.insert(std::map<Powerup::PowerupType, Texture*>::value_type(
			type, new Texture(ImageUtility::loadImage(m_fileUtil->getFullPath(
					FileUtility::image, fileName)), GL_TEXTURE_2D, GL_LINEAR,
					true)));
}

Resources::~Resources() {
	delete HealthIndicator;
	delete LevelUpIndicator;
	delete Crystal;
	delete PlayerDeathSound;
	delete PlayerWalkSprite;
	delete GrenadeSprite;
	for (unsigned int i = 0; i < BloodTex.size(); i++) {
		delete BloodTex[i];
	}
	BloodTex.clear();
	std::map<Powerup::PowerupType, Texture*>::const_iterator iter;
	for (iter = PowerupTex.begin(); iter != PowerupTex.end(); ++iter) {
		delete iter->second;
	}
	PowerupTex.clear();
	for (unsigned int i = 0; i < PlayerHitSounds.size(); i++) {
		delete PlayerHitSounds[i];
	}
	PlayerHitSounds.clear();
	for (unsigned int i = 0; i < PlayerDeathSprites.size(); i++) {
		delete PlayerDeathSprites[i];
	}
	PlayerDeathSprites.clear();
}
