#include "CharStatsWindow.h"

CharStatsWindow::CharStatsWindow(Configuration* config,
		VideoManager* videoManager) :
	Window(0.0f, 0.0f, config->VMode.Width, config->VMode.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {
	const float r = config->VMode.Width * 0.6f;

	addElement("perks", videoManager->RegularText->getObject("Perks:", r,
			videoManager->RegularText->getHeight() * 2.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("unstoppable", videoManager->RegularText->getObject(
			"Unstoppable", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 4.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("poisonbullets", videoManager->RegularText->getObject(
			"Poison bullets",
			r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 5.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("bigcalibre", videoManager->RegularText->getObject(
			"Big calibre", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 6.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("telekinesis", videoManager->RegularText->getObject(
			"Telekinesis", r + videoManager->RegularText->getHeight() * 2.0f,
			videoManager->RegularText->getHeight() * 7.0f, TextManager::LEFT,
			TextManager::MIDDLE));

	addElement("explantation", videoManager->SmallText->getObject(
			"Move mouse over text to get explantation...", config->VMode.Width
					/ 2, videoManager->RegularText->getHeight() * 1.0f,
			TextManager::CENTER, TextManager::MIDDLE));
}

CharStatsWindow::~CharStatsWindow() {
	// nothing
}
