#include "GameState.h"

violet::GameState::GameState() {
	TimeOfDay = 1.0;
	Begun = false;
	Works = true;
	Lost = false;
	Time = 0;
	HighScore = false;
	PlayerName = "";
	Mode = GAMEMODE_SURVIVAL;
}

void violet::GameState::start(GameMode mode) {
	reset();

	Mode = mode;
	Lost = false;
	Paused = false;
	Begun = true;
	JustBegun = true;
	Time = 0;

	switch (mode) {
	case GAMEMODE_SURVIVAL:
		Hardness = 9995.0;
		GameAreaSize = 2048;
		break;
	case GAMEMODE_WAVES:
		Hardness = 9995.0;
		GameAreaSize = 1024;
		break;
	}
}

void violet::GameState::end() {
	Works = false;
}

violet::LifeForm* violet::GameState::getLifeForm(string id) {
	map<string, LifeForm*>::iterator it = lifeForms.find(id);
	if (it == lifeForms.end())
		return NULL;
	else
		return it->second;
}

vector<violet::Blood> violet::GameState::processExplosion(float x,
		float y, float damage, float range, bool affectPlayer) {
	vector<Blood> vBlood;

	if (lifeForms.empty())
		return vBlood;

	map<string, LifeForm*>::const_iterator iter;
	for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
		LifeForm* lifeForm = iter->second;
		if (lifeForm->Type == LIFEFORM_PLAYER && !affectPlayer)
			continue;

		// Damage life forms in range
		float distance = Object::calc_dist(x, y, lifeForm->X, lifeForm->Y)
				- lifeForm->HitR * lifeForm->Scale * lifeForm->getWidth();

		if (distance < range) {
			float dmg = (range - distance) / range * damage;
			lifeForm->setHealth(lifeForm->getHealth() - dmg);

			if (lifeForm->getHealth() == 0) {
				lifeForm->State = LIFEFORM_STATE_BURST;
			}

			// Blood

			Blood blood;

			blood.angle = Object::calc_angle(lifeForm->X, lifeForm->Y, x, y);
			blood.x = lifeForm->X;
			blood.y = lifeForm->Y;
			blood.scale = lifeForm->Scale;
			blood.poisoned = lifeForm->Poisoned;

			vBlood.push_back(blood);
		}
	}

	return vBlood;
}

void violet::GameState::process(int deltaTime) {
	if (!Lost) {
		Hardness -= deltaTime * 0.00012;
		Time += deltaTime;
	}
}

void violet::GameState::reset() {
	clearMap<string, LifeForm*> (&lifeForms);
	clearVector<BasePowerup*> (&powerups);
	clearVector<Bullet*> (&bullets);
}

violet::GameState::~GameState() {
	reset();
}
