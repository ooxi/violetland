#ifndef CHARSTATSWINDOW_H_
#define CHARSTATSWINDOW_H_

#include "../system/graphic/Window.h"
#include "../system/graphic/VideoManager.h"
#include "../game/lifeforms/Player.h"

class CharStatsWindow: public Window {
	VideoManager* videoManager;
	map<string, string> perkDetails;
public:
	void showPerkDetails(std::string elementName);
	static const char* perkIds[];
	static const unsigned perkIdsNumber;
	static const char* paramIds[];
	static const unsigned paramIdsNumber;

	CharStatsWindow(Configuration* config, VideoManager* videoManager);
	void refresh(violetland::Player* player);
	~CharStatsWindow();
};

#endif /* CHARSTATSWINDOW_H_ */
