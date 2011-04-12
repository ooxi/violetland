#include "Resources.h"

violetland::Resources::Resources(FileUtility* fileUtility,
		SoundManager* sndManager) {
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

	loadPowerupTex(BONUS_MEDIKIT, "medikit.png");
	loadPowerupTex(BONUS_GRENADES, "grenade.png");
	loadPowerupTex(BONUS_FREEZE, "freeze.png");
	loadPowerupTex(BONUS_PENBULLETS, "penbullets.png");
	loadPowerupTex(BONUS_NUKE, "bon_nuke.png");
	loadPowerupTex(BONUS_AGILITYROIDS, "pill.png");
	loadPowerupTex(BONUS_VITALITYROIDS, "pill.png");
	loadPowerupTex(BONUS_STRENGTHROIDS, "pill.png");
	loadPowerupTex(BONUS_TELEPORTS, "teleport.png");
}

Sprite* violetland::Resources::loadSprite(unsigned int frames,
		std::string pattern) {
	std::vector<SDL_Surface*> animSurfaces;
	for (unsigned i = 0; i < frames; i++) {
		std::ostringstream oss;
		oss << format(pattern) % i;
		SDL_Surface *surface = ImageUtility::loadImage(m_fileUtil->getFullPath(
				FileUtility::anima, oss.str()));
		animSurfaces.push_back(surface);
	}
	return new Sprite(animSurfaces);
}

Sound* violetland::Resources::loadSnd(std::string fileName) {
	return m_sndManager->create(m_fileUtil->getFullPath(FileUtility::sound,
			fileName));
}

Texture* violetland::Resources::loadTex(std::string fileName) {
	return new Texture(ImageUtility::loadImage(m_fileUtil->getFullPath(
			FileUtility::image, fileName)), GL_TEXTURE_2D, GL_LINEAR, true);
}

void violetland::Resources::loadPowerupTex(BonusType type,
		std::string fileName) {
	PowerupTex.insert(std::map<BonusType, Texture*>::value_type(type,
			new Texture(ImageUtility::loadImage(m_fileUtil->getFullPath(
					FileUtility::image, fileName)), GL_TEXTURE_2D, GL_LINEAR,
					true)));
}

violetland::Resources::~Resources() {
	delete HealthIndicator;
	delete LevelUpIndicator;
	delete Crystal;
	delete PlayerDeathSound;
	delete PlayerWalkSprite;
	delete GrenadeSprite;

	clearVector<Texture*> (&ExplTex);
	clearVector<Texture*> (&BloodTex);
	clearVector<Sound*> (&ExplSounds);
	clearVector<Sound*> (&PlayerHitSounds);
	clearVector<Sprite*> (&PlayerDeathSprites);
	clearMap<BonusType, Texture*> (&PowerupTex);
}
