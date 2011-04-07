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
	Sprite *m_deathSprite;
	StaticObject *m_arms;
	std::vector<DynamicObject*> m_shells;
	std::vector<Sound*> m_hitSounds;
	Sound* m_dyingSound;
	Weapon *m_weapon;
	bool m_light, m_laser;
	int m_hitSndPlaying;
	void processBonus(int deltaTime);
	void processState(int deltaTime);
	void processArms(int deltaTime);
public:
	Player();
	Player(float x, float y, Sprite *legsSprite, Sprite *deathSprite,
			std::vector<Sound*> hitSounds, Sound* dyingSound);

	virtual void process(int deltaTime);
	virtual void draw();
	virtual Sound* hit(float damage, bool poison);

	virtual float getStrength();
	virtual float getAgility();
	virtual float getVitality();

	void hit();
	void reload();
	void toggleLight();
	void toggleLaser();
	const float getLegsAngle();
	const float getArmsAngle();
	void setX(float value);
	void setY(float value);
	const bool getLight();
	const bool getLaser();
	Weapon* getWeapon();
	void setWeapon(Weapon *value);
	std::vector<Bullet*> *fire();
	Bullet* throwGrenade(Sprite* grenadeSprite);
	virtual StaticObject* getCorpse();
	~Player();
	float AccuracyDeviation;
	int LevelPoints;
	int Xp;
	int NextLevelXp;
	int LastLevelXp;
	int Kills;
	int Grenades;
	int Teleports;
	bool Unstoppable;
	bool PoisonBullets;
	bool BigCalibre;
	bool Telekinesis;
	bool NightVision;
	bool Looting;
	bool WideSight;
	int bonusTimes[PLAYER_BONUS_COUNT];
	void teleport();
	void fadeColor(int deltaTime);
	bool Empty;
	PlayerActionMode ActionMode;
};

}

#endif /* PLAYER_H_ */
