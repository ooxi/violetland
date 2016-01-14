#include <libintl.h>
#include <locale.h>
#define _(STRING)			gettext(STRING)

#include "Powerup.h"
#include "Game.h"


violet::BasePowerup::BasePowerup(float x, float y, Texture *tex) : 
		StaticObject(x, y, 128, 128, tex, false) {
	Time = 15000;
	Dir = 1;
}


void violet::BasePowerup::process(int timeDelta) {
	Time -= timeDelta;
	AMask = Time / 15000.0;
	if (getType() != BONUS_WEAPON) {
		Angle += timeDelta * 0.05f * Dir;

		if (Angle > 45 && Dir > 0)
			Dir = -1;
		if (Angle < -45 && Dir < 0)
			Dir = 1;
	}
}


/*violet::Powerup::~Powerup() {
	if(m_type != BONUS_WEAPON)
		delete Object;
}*/


const std::string violet::MedikitPowerup::getHudInfo() const {
	return _("a medikit");
}

bool violet::MedikitPowerup::modify(Game* game, Player* player) {
	if (player->getHealth() < player->MaxHealth()) {
		game->hud->addMessage(_("You have taken a medical kit."));
		player->setHealth(player->getHealth() + m_value);
		return true;
	}
	return false;
}

const std::string violet::GrenadePowerup::getHudInfo() const {
	return _("a hand grenade");
}

bool violet::GrenadePowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You have taken a grenade."));
	++player->Grenades;
	return true;
}


const std::string violet::FreezePowerup::getHudInfo() const {
	return _("a nitrogen bomb");
}

bool violet::FreezePowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("All have been frozen around you."));

	std::map<std::string, LifeForm*>::const_iterator iter;
	for (iter = game->gameState->lifeForms.begin(); iter
			!= game->gameState->lifeForms.end(); ++iter) {
		LifeForm* lf = iter->second;

		if (lf->Type == LIFEFORM_PLAYER || lf->State
				!= LIFEFORM_STATE_ALIVE)
			continue;
		lf->Frozen = 10000;
		lf->Burning = false;
		lf->Speed = 0.0f;
	}
	player->bonusTimes[PLAYER_BONUS_FREEZE]	= 10000;
	return true;
}


const std::string violet::NukePowerup::getHudInfo() const {
	return _("nuke!");
}

void addExplosion(float x, float y, float damage, float range);
bool violet::NukePowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("Boom!"));
	addExplosion(X, Y, 12.0f, 400.0f);

	game->resources->ExplSounds[1]->play(8, 0, 0);

	Explosion * expl = new Explosion(true, X, Y,
			400.0f, game->resources->ExplTex[0], game->resources->ExplTex[1]);
	game->particleSystems.push_back(expl);

	return true;
}


const std::string violet::PenetrationBulletsPowerup::getHudInfo() const {
	return _("penetration bullets");
}

bool violet::PenetrationBulletsPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got powerful penetration bullets."));
	player->bonusTimes[PLAYER_BONUS_PENBULLETS] = 10000;

	return true;
}


const std::string violet::VitalityPowerup::getHudInfo() const {
	return _("vitality boost");
}

bool violet::VitalityPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got a vitality boost."));
	player->bonusTimes[PLAYER_BONUS_VITALITYBOOST] += 10000;

	return true;
}

const std::string violet::StrengthPowerup::getHudInfo() const {
	return _("strength boost");
}

bool violet::StrengthPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got a strength boost."));
	player->bonusTimes[PLAYER_BONUS_STRENGTHBOOST] += 10000;

	return true;
}

const std::string violet::AgilityPowerup::getHudInfo() const {
	return _("agility boost");
}

bool violet::AgilityPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got a agility boost."));
	player->bonusTimes[PLAYER_BONUS_AGILITYBOOST] += 10000;

	return true;
}


const std::string violet::TeleportPowerup::getHudInfo() const {
	return _("a teleport");
}

bool violet::TeleportPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You have taken a teleport."));
	++player->Teleports;

	return true;
}


bool violet::WeaponPowerup::modify(Game* game, Player* player) {
	if (game->input->getDownInput(InputHandler::Pickup)
						|| game->config->AutoWeaponPickup) {
		player->setWeapon(weapon);
		std::ostringstream oss;
		oss << boost::format(_("You have taken the %s.")) % weapon->Name;
		game->hud->addMessage(oss.str());
		return true;
	}
	return false;
}
