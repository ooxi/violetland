#ifndef PLAYER_H_
#define PLAYER_H_

#include "LifeForm.h"
#include "Weapon.h"

class Player: public LifeForm {
private:
	Sprite *m_deathSprite;
	DynamicObject *m_body;
	StaticObject *m_arms;
	std::vector<DynamicObject*> m_shells;
	std::vector<Sound*> m_hitSounds;
	Sound* m_dyingSound;
	Weapon *m_weapon;
	bool m_light, m_laser;
	int m_hitSndPlaying;
	void processBonus(int deltaTime);
public:
	Player();
	Player(float x, float y, Sprite *legsSprite, Sprite *deathSprite,
			std::vector<Sound*> hitSounds, Sound* dyingSound);
    enum bonusType {
        FIRSTBONUS = 0, PENBULLETS = FIRSTBONUS, VITALITYROIDS,AGILITYROIDS,STRENGTHROIDS,BONUSCOUNT
	};
	virtual void process(int deltaTime);
	virtual void draw();

	void hit();
	void move(char movementX, char movementY, int deltaTime);
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
	std::string HudInfo;
	bool Unstoppable;
	bool PoisonBullets;
	bool BigCalibre;
	bool Telekinesis;
	bool NightVision;
	int bonusTimes[BONUSCOUNT];
	float getStrength();
	float getAgility();
	float getVitality();
	void teleport();
	void fadeColor(int deltaTime);
	bool Empty;
	int ActionMode;
};

#endif /* PLAYER_H_ */
