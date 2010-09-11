#include "GameState.h"

violetland::GameState::GameState() {
	TimeOfDay = 1.0;
	Begun = false;
	Works = true;
	Lost = false;
	Time = 0;
	HighScore = false;
	PlayerName = "";
}

void violetland::GameState::start(GameMode mode) {
	clearMap<string, LifeForm*> (&lifeForms);
	clearVector<Powerup*> (&powerups);
	clearVector<StaticObject*> (&bloodStains);
	clearVector<Bullet*> (&bullets);
	clearVector<Explosion*> (&explosions);

	switch (mode) {
	case GAMEMODE_SURVIVAL:
		Hardness = 9995.0;
		Lost = false;
		Paused = false;
		Begun = true;
		Time = 0;
		break;
	case GAMEMODE_SCENARIO:
		break;
	}
}

void violetland::GameState::end() {
	Works = false;
}

violetland::LifeForm* violetland::GameState::getLifeForm(string id) {
	return lifeForms.at(id);
}

void violetland::GameState::process(int deltaTime) {
	if (!Lost) {
		Hardness -= deltaTime * 0.00012;
		Time += deltaTime;
	}

	terrain->beginDrawOn();
	{
		for (unsigned int i = 0; i < bloodStains.size(); i++) {
			terrain->drawOn(bloodStains[i]);
		}

		clearVector<StaticObject*> (&bloodStains);

		map<string, LifeForm*>::const_iterator iter;
		for (iter = lifeForms.begin(); iter != lifeForms.end(); ++iter) {
			LifeForm* lf = iter->second;

			if (lf->State == LIFEFORM_STATE_DIED)
				terrain ->drawOn(lf->getCorpse());

			if (lf->State == LIFEFORM_STATE_DIED || lf->State
					== LIFEFORM_STATE_BURST) {

				if (lf->Type == LIFEFORM_MONSTER) {
					delete lf;
					lifeForms.erase(iter->first);
				}
			}
		}
	}

	terrain->endDrawOn();
}

violetland::GameState::~GameState() {
	delete terrain;

	clearVector<Powerup*> (&powerups);
	clearVector<StaticObject*> (&bloodStains);
	clearVector<Bullet*> (&bullets);
	clearVector<Explosion*> (&explosions);
	clearMap<std::string, LifeForm*> (&lifeForms);
}
