#include <libintl.h>
#include <locale.h>
#define _(STRING)			gettext(STRING)

#include "Powerup.h"
#include "Game.h"


violetland::BasePowerup::BasePowerup(float x, float y, Texture *tex) : 
		StaticObject(x, y, 128, 128, tex, false) {
	Time = 15000;
	Dir = 1;
}


void violetland::BasePowerup::process(int timeDelta) {
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


/*violetland::Powerup::~Powerup() {
	if(m_type != BONUS_WEAPON)
		delete Object;
}*/


const std::string violetland::MedikitPowerup::getHudInfo() const {
	return _("a medikit");
}

bool violetland::MedikitPowerup::modify(Game* game, Player* player) {
	if (player->getHealth() < player->MaxHealth()) {
		game->hud->addMessage(_("You have taken a medical kit."));
		player->setHealth(player->getHealth() + m_value);
		return true;
	}
	return false;
}

const std::string violetland::GrenadePowerup::getHudInfo() const {
	return _("a hand grenade");
}

bool violetland::GrenadePowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You have taken a grenade."));
	++player->Grenades;
	return true;
}


const std::string violetland::FreezePowerup::getHudInfo() const {
	return _("a nitrogen bomb");
}

bool violetland::FreezePowerup::modify(Game* game, Player* player) {
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


const std::string violetland::NukePowerup::getHudInfo() const {
	return _("nuke!");
}

void addExplosion(float x, float y, float damage, float range);
bool violetland::NukePowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("Boom!"));
	addExplosion(player->X, player->Y, 12.0f, 400.0f);

	game->resources->ExplSounds[1]->play(8, 0, 0);

	Explosion * expl = new Explosion(true, player->X, player->Y,
			400.0f, game->resources->ExplTex[0], game->resources->ExplTex[1]);
	game->particleSystems.push_back(expl);

	return true;
}


const std::string violetland::PenetrationBulletsPowerup::getHudInfo() const {
	return _("penetration bullets");
}

bool violetland::PenetrationBulletsPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got powerful penetration bullets."));
	player->bonusTimes[PLAYER_BONUS_PENBULLETS] = 10000;

	return true;
}


const std::string violetland::VitalityPowerup::getHudInfo() const {
	return _("vitality boost");
}

bool violetland::VitalityPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got a vitality boost."));
	player->bonusTimes[PLAYER_BONUS_VITALITYBOOST] += 10000;

	return true;
}

const std::string violetland::StrengthPowerup::getHudInfo() const {
	return _("strength boost");
}

bool violetland::StrengthPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got a vitality boost."));
	player->bonusTimes[PLAYER_BONUS_STRENGTHBOOST] += 10000;

	return true;
}

const std::string violetland::AgilityPowerup::getHudInfo() const {
	return _("agility boost");
}

bool violetland::AgilityPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You got a vitality boost."));
	player->bonusTimes[PLAYER_BONUS_AGILITYBOOST] += 10000;

	return true;
}


const std::string violetland::TeleportPowerup::getHudInfo() const {
	return _("a teleport");
}

bool violetland::TeleportPowerup::modify(Game* game, Player* player) {
	game->hud->addMessage(_("You have taken a teleport."));
	++player->Teleports;

	return true;
}


bool violetland::WeaponPowerup::modify(Game* game, Player* player) {
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
