#include <boost/format.hpp>
#include <boost/math/special_functions/round.hpp>
#include <libintl.h>
#include <locale.h>
#include "CharStatsWindow.h"
#define _(STRING)            gettext(STRING)

using namespace violetland;

const char* CharStatsWindow::perkIds[] = {
	"unstoppable",
	"poisonbullets",
	"bigcalibre",
	"telekinesis",
	"nightvision",
	"looting",
	"widesight",
	"magneto"
};

const unsigned CharStatsWindow::perkIdsNumber = sizeof(CharStatsWindow::perkIds) / sizeof(char*);

const char* CharStatsWindow::paramIds[] = {
	"strength",
	"agility",
	"vitality",
};

const unsigned CharStatsWindow::paramIdsNumber = sizeof(CharStatsWindow::paramIds) / sizeof(char*);

void CharStatsWindow::onPlayerParamClickEvent(void* sender, string paramName)
{
	CharStatsWindow* window = (CharStatsWindow*)sender;
	window->increasePlayerParam(paramName);
}

void CharStatsWindow::onPerkHoverEvent(void* sender, string perkName)
{
	CharStatsWindow* window = (CharStatsWindow*)sender;
	window->showPerkDetails(perkName);
}

void CharStatsWindow::onPerkClickEvent(void* sender, string perkName)
{
	CharStatsWindow* window = (CharStatsWindow*)sender;
	window->givePerkToPlayer(perkName);
}

CharStatsWindow::CharStatsWindow(Configuration* config,
		VideoManager* videoManager, Player* player) :
	Window(0.0f, 0.0f, videoManager->getVideoMode().Width,
			videoManager->getVideoMode().Height, 0.0f, 0.0f,
			0.0f, 0.5f), m_videoManager(videoManager), m_player(player) {
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
	labels.push_back(Label("magneto", _("Magneto")));

	addElements(labels, videoManager->RegularText,
			r+2*h, 4*h, h, TextManager::LEFT, TextManager::MIDDLE);

	addElement("explanation", _("Move mouse over text to get explanation..."),
			videoManager->SmallText, config->Screen.Width / 2, h,
			TextManager::CENTER, TextManager::MIDDLE);

	m_perkDetails["unstoppable"] = _("Unstoppable: enemies can't block your "
			"movement anymore, but they still can hurt you.");
	m_perkDetails["poisonbullets"] = _("Poison bullets: after getting hit by your "
			"bullet, enemies slowly lose health until they die.");
	m_perkDetails["bigcalibre"] = _("Big calibre: your bullets can wound a few "
			"monsters in a row.");
	m_perkDetails["telekinesis"] = _("Telekinesis: you can touch items through "
			"tour eyes.");
	m_perkDetails["nightvision"] = _("Night vision: you can see in the dark.");
	m_perkDetails["looting"] = _("Looting: Monsters will drop more bonuses.");
	m_perkDetails["widesight"] = _("Wide sight: accessible area for action "
			"is much more.");
	m_perkDetails["magneto"] = _("Magneto: useful things slowly move "
			"towards you.");

	for (unsigned i = 0; i < paramIdsNumber; ++i)
			addHandler(Window::hdl_lclick, paramIds[i], onPlayerParamClickEvent);

	for (unsigned i = 0; i < perkIdsNumber; ++i) {
		addHandler(Window::hdl_move, perkIds[i], onPerkHoverEvent);
		addHandler(Window::hdl_lclick, perkIds[i], onPerkClickEvent);
	}
}

void CharStatsWindow::refresh() {
	const int l = (int) (m_videoManager->getVideoMode().Width * 0.1f);
	const int r = (int) (m_videoManager->getVideoMode().Width * 0.6f);
	const int h = m_videoManager->RegularText->getHeight();

	ostringstream oss;
	vector<Label> stats;

	oss << boost::format(_("Current player level: %i")) % m_player->Level;
	stats.push_back(Label("level", oss.str()));

	oss.str("");
	oss << boost::format(_("Available improvement points: %i"))
			% m_player->LevelPoints;
	stats.push_back(Label("availpoints", oss.str()));

	oss.str("");
	oss << boost::format(_("Strength: %i")) % (m_player->Strength * 100);
	stats.push_back(Label("strength", oss.str()));

	oss.str("");
	oss << boost::format(_("Agility: %i")) % (m_player->Agility * 100);
	stats.push_back(Label("agility", oss.str()));

	oss.str("");
	oss << boost::format(_("Vitality: %i")) % (m_player->Vitality * 100);
	stats.push_back(Label("vitality", oss.str()));

	oss.str("");
	oss << boost::format(_("HP: %i / Max HP: %i")) % boost::math::round(
			m_player->getHealth() * 100) % (m_player->MaxHealth() * 100);
	stats.push_back(Label("hp", oss.str()));

	oss.str("");
	oss << boost::format(_("Melee damage: %i")) % (m_player->Damage() * 100);
	stats.push_back(Label("melee", oss.str()));

	oss.str("");
	oss << boost::format(_("Chance of block: %i%%")) % (m_player->ChanceToEvade()
			* 100);
	stats.push_back(Label("chanceblock", oss.str()));

	oss.str("");
	oss << boost::format(_("Reloading speed modifier: %i%%"))
			% boost::math::round(m_player->ReloadSpeedMod() * 100);
	stats.push_back(Label("reloadingspeed", oss.str()));

	oss.str("");
	oss << boost::format(_("Accuracy deviation modifier: %i%%"))
			% (m_player->WeaponRetForceMod() * 100);
	stats.push_back(Label("accuracy", oss.str()));

	oss.str("");
	oss << boost::format(_("Health regeneration: %.2f/min"))
			% (m_player->HealthRegen() * 6000000);
	stats.push_back(Label("healthregen", oss.str()));

	addElements(stats, m_videoManager->RegularText, l, 4 * h, h,
			TextManager::LEFT, TextManager::MIDDLE);

	struct BoolString {
		bool f;
		const char* str;
	};

	BoolString perks[] = { { m_player->Unstoppable, "+unstoppable" }, {
			m_player->PoisonBullets, "+poisonbullets" }, { m_player->BigCalibre,
			"+bigcalibre" }, { m_player->Telekinesis, "+telekinesis" }, {
			m_player->NightVision, "+nightvision" }, { m_player->Looting,
			"+looting" }, { m_player->WideSight, "+widesight" }, { m_player->Magneto, "+magneto" } };

	for (unsigned i = 0; i < sizeof(perks) / sizeof(BoolString); ++i)
		if (perks[i].f)
			addElement(perks[i].str, "+", m_videoManager->RegularText,
					r, (4 + i) * h, TextManager::CENTER, TextManager::MIDDLE);
}

void CharStatsWindow::increasePlayerParam(std::string elementName) {
	if (m_player->LevelPoints > 0) {
		if (elementName == "strength")
			m_player->Strength += 0.1;
		else if (elementName == "agility")
			m_player->Agility += 0.1;
		else if (elementName == "vitality") {
			float h = m_player->getHealth() / m_player->MaxHealth();
			m_player->Vitality += 0.1;
			m_player->setHealth(h * m_player->MaxHealth());
		}

		m_player->LevelPoints--;

		refresh();
	}
}

void CharStatsWindow::showPerkDetails(string perkName) {
	int x = m_videoManager->getVideoMode().Width / 2;
	int y = m_videoManager->RegularText->getHeight();

	map<string, string>::iterator it = m_perkDetails.find(perkName);
	if (it != m_perkDetails.end())
		addElement("explanation", it->second,
				m_videoManager->SmallText, x, y, TextManager::CENTER,
				TextManager::MIDDLE);
}

void CharStatsWindow::givePerkToPlayer(string perkName) {
	if (m_player->LevelPoints > 0) {
		map<string, bool*> m;
		m["unstoppable"] = &m_player->Unstoppable;
		m["poisonbullets"] = &m_player->PoisonBullets;
		m["bigcalibre"] = &m_player->BigCalibre;
		m["telekinesis"] = &m_player->Telekinesis;
		m["nightvision"] = &m_player->NightVision;
		m["looting"] = &m_player->Looting;
		m["widesight"] = &m_player->WideSight;
		m["magneto"] = &m_player->Magneto;

		map<string, bool*>::iterator it = m.find(perkName);

		if (it != m.end()) {
			if (!*it->second) {
				*it->second = true;
				--m_player->LevelPoints;
				refresh();
			}
		}
	}
}

CharStatsWindow::~CharStatsWindow() {
	// nothing
}
