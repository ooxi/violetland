#ifndef PLAYER_H_
#define PLAYER_H_

#include "LifeForm.h"
#include "../Weapon.h"

namespace violetland {

enum PlayerActionMode {
	PLAYER_ACT_MODE_FIRE = 0, PLAYER_ACT_MODE_TELEPORT
};

enum PlayerBonusType {
	PLAYER_BONUS_FIRST = 0,
	PLAYER_BONUS_PENBULLETS = PLAYER_BONUS_FIRST,
	PLAYER_BONUS_VITALITYBOOST,
	PLAYER_BONUS_AGILITYBOOST,
	PLAYER_BONUS_STRENGTHBOOST,
	PLAYER_BONUS_FREEZE,
	PLAYER_BONUS_COUNT
};

class Player: public LifeForm {
private:
	static const unsigned TELEKINESIS_DELAY = 2000;

	Sprite *m_deathSprite;
	StaticObject *m_arms;
	std::vector<DynamicObject*> m_shells;
	std::vector<Sound*> m_hitSounds;
	Sound* m_dyingSound;
	Weapon *m_weapon;
	bool m_light, m_laser;
	int m_hitSndPlaying;
	int m_telekinesisDelay;

	void processBonus(int deltaTime);
	void processState(int deltaTime);
	void processArms(int deltaTime);
public:
	float AccuracyDeviation;
	int LevelPoints;
	int Xp;
	int NextLevelXp;
	int LastLevelXp;
	int Kills;
	int Grenades;
	int Teleports;

	// Perks

	bool Unstoppable;
	bool PoisonBullets;
	bool BigCalibre;
	bool Telekinesis;
	bool Magneto;
	bool NightVision;
	bool Looting;
	bool WideSight;

	// Fire with gun or use teleport
	PlayerActionMode ActionMode;

	int bonusTimes[PLAYER_BONUS_COUNT];

	Player(float x, float y,
			Sprite *legsSprite,
			Sprite *deathSprite,
			std::vector<Sound*> hitSounds,
			Sound* dyingSound);
	virtual ~Player();

	virtual void process(int deltaTime);
	virtual void draw();
	virtual Sound* hit(float damage, bool poison);

	virtual float getStrength() const;
	virtual float getAgility() const;
	virtual float getVitality() const;

	void hit();
	void setX(float value);
	void setY(float value);
	const float getLegsAngle() const { return m_body->Angle; }
	const float getArmsAngle() const { return m_arms->Angle; }
	void setWeapon(Weapon *value);
	const Weapon* getWeapon() const { return m_weapon; }
	std::vector<Bullet*> *fire();
	void reload();
	void toggleLight() { m_light = !m_light; }
	const bool getLight() const { return m_light; }
	void toggleLaser() { m_laser = !m_laser; }
	const bool getLaser() const { return m_laser; }
	Bullet* throwGrenade(Sprite* grenadeSprite);
	virtual StaticObject* getCorpse();
	void teleport();
	void fadeColor(int deltaTime);
	unsigned processTelekinesis(int deltaTime, bool reset);
};

}

#endif /* PLAYER_H_ */
