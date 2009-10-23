#include "./../system/DynamicObject.h"
#include "./../system/StaticObject.h"
#include "./../system/LiveObject.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_opengl.h"
#include "Weapon.h"
#include <cmath>
#include <stdlib.h>
#include <vector>

#ifndef PLAYER_H_
#define PLAYER_H_

class Player: public LiveObject {
private:
	DynamicObject *m_legs;
	StaticObject *m_arms;
	Weapon *m_weapon;
	bool m_light, m_laser;
public:
	Player(float x, float y, Sprite *legsSprite, Texture *armsTex,
			Weapon *weapon);
	~Player();
	void process(int deltaTime);
	void move(char movementX, char movementY, int deltaTime);
	void reload();
	void draw();
	void toggleLight();
	void toggleLaser();
	const float getX();
	const float getY();
	const float getMoveDirection();
	const float getArmsDirection();
	void setX(float value);
	void setY(float value);
	std::string getWeaponName();
	const int getAmmo();
	const int getMaxAmmo();
	const float getReloadState();
	const bool getLight();
	const bool getLaser();
	void setWeapon(Weapon *value);
	std::vector<Bullet*> *fire();
	float AccuracyDeviation;
	float TargetX, TargetY;
	int Level;
	int LevelPoints;
	int Xp;
	int NextLevelXp;
	int Kills;

	bool Unstoppable;
	bool PoisonBullets;
};

#endif /* PLAYER_H_ */
