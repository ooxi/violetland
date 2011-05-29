#ifndef CHARSTATSWINDOW_H_
#define CHARSTATSWINDOW_H_

#include "../system/graphic/Window.h"
#include "../system/graphic/VideoManager.h"
#include "../game/lifeforms/Player.h"

using namespace violetland;
using namespace std;

class CharStatsWindow: public Window {
	VideoManager* videoManager;
	Player* player;
	map<string, string> perkDetails;

	void increasePlayerParam(string paramName);
	void showPerkDetails(string perkName);
	void givePerkToPlayer(string perkName);
public:
	static const char* perkIds[];
	static const unsigned perkIdsNumber;
	static const char* paramIds[];
	static const unsigned paramIdsNumber;

	static void onPlayerParamClickEvent(void* sender, string paramName);
	static void onPerkHoverEvent(void* sender, string perkName);
	static void onPerkClickEvent(void* sender, string perkName);

	CharStatsWindow(Configuration* config, VideoManager* videoManager,
			Player* player);
	void refresh();
	~CharStatsWindow();
};

#endif /* CHARSTATSWINDOW_H_ */
