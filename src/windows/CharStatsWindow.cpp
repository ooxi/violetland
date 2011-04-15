#include "CharStatsWindow.h"
#include <libintl.h>
#include <locale.h>
#define _(STRING)            gettext(STRING)

CharStatsWindow::CharStatsWindow(Configuration* config,
		VideoManager* videoManager) :
	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {
	const float r = config->Screen.Width * 0.6f;

	addElement("perks", videoManager->RegularText->getObject(_("Perks:"), r,
			videoManager->RegularText->getHeight() * 2.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("unstoppable", videoManager->RegularText->getObject(
			_("Unstoppable"), r + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 4.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	addElement("poisonbullets", videoManager->RegularText->getObject(
			_("Poison bullets"), r
					+ videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 5.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("bigcalibre", videoManager->RegularText->getObject(
			_("Big calibre"), r + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 6.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	addElement("telekinesis", videoManager->RegularText->getObject(
			_("Telekinesis"), r + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 7.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	addElement("nightvision", videoManager->RegularText->getObject(
			_("Night vision"), r + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 8.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	addElement("looting", videoManager->RegularText->getObject(_("Looting"), r
			+ videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 9.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("widesight", videoManager->RegularText->getObject(
			_("Wide sight"), r + videoManager->RegularText->getHeight()
					* 2.0f, videoManager->RegularText->getHeight() * 10.0f,
			TextManager::LEFT, TextManager::MIDDLE));

	addElement("explanation", videoManager->SmallText->getObject(
			_("Move mouse over text to get explanation..."),
			config->Screen.Width / 2, videoManager->RegularText->getHeight()
					* 1.0f, TextManager::CENTER, TextManager::MIDDLE));
}

CharStatsWindow::~CharStatsWindow() {
	// nothing
}
