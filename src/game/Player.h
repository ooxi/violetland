#include <cmath>
#include <stdlib.h>
#include <vector>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "../system/graphic/DynamicObject.h"
#include "./../system/graphic/StaticObject.h"
#include "LifeForm.h"
#include "Weapon.h"
#include "bullets/GrenadeBullet.h"

#ifndef PLAYER_H_
#define PLAYER_H_

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
public:
	Player();
	Player(float x, float y, Sprite *legsSprite, Sprite *deathSprite,
			std::vector<Sound*> hitSounds, Sound* dyingSound);
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
	int Kills;
	int Grenades;
	std::string HudInfo;

	bool Unstoppable;
	bool PoisonBullets;
	bool BigCalibre;
	bool Telekinesis;
	int PenBullets;

	bool Empty;
};

#endif /* PLAYER_H_ */
