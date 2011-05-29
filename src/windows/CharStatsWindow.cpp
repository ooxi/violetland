#include <boost/format.hpp>
#include <boost/math/special_functions/round.hpp>

#include <libintl.h>
#include <locale.h>

#include "CharStatsWindow.h"

#define _(STRING)            gettext(STRING)

const char* CharStatsWindow::perkIds[] = {
	"unstoppable", 
	"poisonbullets", 
	"bigcalibre",
	"telekinesis", 
	"nightvision", 
	"looting", 
	"widesight",
};
const unsigned CharStatsWindow::perkIdsNumber = sizeof(CharStatsWindow::perkIds)/sizeof(char*);

const char* CharStatsWindow::paramIds[] = {
	"strength",
	"agility",
	"vitality",
};
const unsigned CharStatsWindow::paramIdsNumber = sizeof(CharStatsWindow::paramIds)/sizeof(char*);


CharStatsWindow::CharStatsWindow(Configuration* config,
		VideoManager* videoManager) :
	Window(0.0f, 0.0f, videoManager->getVideoMode().Width, 
			videoManager->getVideoMode().Height, 0.0f, 0.0f,
			0.0f, 0.5f), videoManager(videoManager) {
	const int r = videoManager->getVideoMode().Width * 0.6f;
	const int h = videoManager->RegularText->getHeight();

	addElement("perks", _("Perks:"), videoManager->RegularText, 
			r, 2*h, TextManager::LEFT, TextManager::MIDDLE);
			
	std::vector<Label> labels;
	labels.push_back(Label("unstoppable", _("Unstoppable")));
	labels.push_back(Label("poisonbullets", _("Poison bullets")));
	labels.push_back(Label("bigcalibre", _("Big calibre")));
	labels.push_back(Label("telekinesis", _("Telekinesis")));
	labels.push_back(Label("nightvision", _("Night vision")));
	labels.push_back(Label("looting", _("Looting")));
	labels.push_back(Label("widesight", _("Wide sight")));
	
	addElements(labels, videoManager->RegularText, 
			r+2*h, 4*h, h, TextManager::LEFT, TextManager::MIDDLE);

	addElement("explanation", _("Move mouse over text to get explanation..."),
			videoManager->SmallText, config->Screen.Width / 2, h, 
			TextManager::CENTER, TextManager::MIDDLE);
			
	perkDetails["unstoppable"] = _("Unstoppable: enemies can't block your "
			"movement anymore, but they still can hurt you.");
	perkDetails["poisonbullets"] = _("Poison bullets: after getting hit by your "
			"bullet, enemies slowly lose health until they die.");
	perkDetails["bigcalibre"] = _("Big calibre: your bullets can wound a few "
			"monsters in a row.");
	perkDetails["telekinesis"] = _("Telekinesis: useful things slowly move "
			"towards you.");
	perkDetails["nightvision"] = _("Night vision: you can see in the dark.");
	perkDetails["looting"] = _("Looting: Monsters will drop more bonuses.");
	perkDetails["widesight"] = _("Wide sight: accessible area for action "
			"is much more.");
}

void CharStatsWindow::showPerkDetails(std::string elementName) {
	int x = videoManager->getVideoMode().Width / 2;
	int y = videoManager->RegularText->getHeight();

	map<string, string>::iterator it = perkDetails.find(elementName);
	if (it != perkDetails.end())
		addElement("explanation", it->second,
				videoManager->SmallText, x, y, TextManager::CENTER,
				TextManager::MIDDLE);
}

void CharStatsWindow::refresh(violetland::Player* player) {
	const int l = (int) (videoManager->getVideoMode().Width * 0.1f);
	const int r = (int) (videoManager->getVideoMode().Width * 0.6f);
	const int h = videoManager->RegularText->getHeight();
	
	ostringstream oss;
	vector<Label> stats;

	oss << boost::format(_("Current player level: %i")) % player->Level;
	stats.push_back(Label("level", oss.str()));

	oss.str("");
	oss << boost::format(_("Available improvement points: %i"))
			% player->LevelPoints;
	stats.push_back(Label("availpoints", oss.str()));

	oss.str("");
	oss << boost::format(_("Strength: %i")) % (player->Strength * 100);
	stats.push_back(Label("strength", oss.str()));

	oss.str("");
	oss << boost::format(_("Agility: %i")) % (player->Agility * 100);
	stats.push_back(Label("agility", oss.str()));

	oss.str("");
	oss << boost::format(_("Vitality: %i")) % (player->Vitality * 100);
	stats.push_back(Label("vitality", oss.str()));

	oss.str("");
	oss << boost::format(_("HP: %i / Max HP: %i")) % boost::math::round(
			player->getHealth() * 100) % (player->MaxHealth() * 100);
	stats.push_back(Label("hp", oss.str()));

	oss.str("");
	oss << boost::format(_("Melee damage: %i")) % (player->Damage() * 100);
	stats.push_back(Label("melee", oss.str()));

	oss.str("");
	oss << boost::format(_("Chance of block: %i%%")) % (player->ChanceToEvade()
			* 100);
	stats.push_back(Label("chanceblock", oss.str()));

	oss.str("");
	oss << boost::format(_("Reloading speed modifier: %i%%"))
			% boost::math::round(player->ReloadSpeedMod() * 100);
	stats.push_back(Label("reloadingspeed", oss.str()));

	oss.str("");
	oss << boost::format(_("Accuracy deviation modifier: %i%%"))
			% (player->WeaponRetForceMod() * 100);
	stats.push_back(Label("accuracy", oss.str()));

	oss.str("");
	oss << boost::format(_("Health regeneration: %.2f/min"))
			% (player->HealthRegen() * 6000000);
	stats.push_back(Label("healthregen", oss.str()));

	addElements(stats, videoManager->RegularText, l, 4 * h, h,
			TextManager::LEFT, TextManager::MIDDLE);

	struct BoolString {
		bool f;
		const char* str;
	};

	BoolString perks[] = { { player->Unstoppable, "+unstoppable" }, {
			player->PoisonBullets, "+poisonbullets" }, { player->BigCalibre,
			"+bigcalibre" }, { player->Telekinesis, "+telekinesis" }, {
			player->NightVision, "+nightvision" }, { player->Looting,
			"+looting" }, { player->WideSight, "+widesight" }, };

	for (unsigned i = 0; i < sizeof(perks) / sizeof(BoolString); ++i)
		if (perks[i].f)
			addElement(perks[i].str, "+", videoManager->RegularText,
					r, (4 + i) * h, TextManager::CENTER, TextManager::MIDDLE);
}


CharStatsWindow::~CharStatsWindow() {
	// nothing
}
