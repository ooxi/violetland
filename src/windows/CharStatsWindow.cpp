#include "CharStatsWindow.h"
#include <libintl.h>
#include <locale.h>
#define _(STRING)            gettext(STRING)

CharStatsWindow::CharStatsWindow(Configuration* config,
		VideoManager* videoManager) :
	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f,
			0.0f, 0.5f) {
	const int r = config->Screen.Width * 0.6f;
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
}

CharStatsWindow::~CharStatsWindow() {
	// nothing
}
