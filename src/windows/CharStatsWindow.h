#ifndef CHARSTATSWINDOW_H_
#define CHARSTATSWINDOW_H_

#include "../system/graphic/Window.h"
#include "../system/graphic/VideoManager.h"
#include "../game/lifeforms/Player.h"

namespace violetland {
class CharStatsWindow: public Window {
	VideoManager* m_videoManager;
	Player* m_player;
	map<std::string, std::string> m_perkDetails;

	void increasePlayerParam(std::string paramName);
	void showPerkDetails(std::string perkName);
	void givePerkToPlayer(std::string perkName);
public:
	static const char* perkIds[];
	static const unsigned perkIdsNumber;
	static const char* paramIds[];
	static const unsigned paramIdsNumber;

	static void onPlayerParamClickEvent(void* sender, std::string paramName);
	static void onPerkHoverEvent(void* sender, std::string perkName);
	static void onPerkClickEvent(void* sender, std::string perkName);

	CharStatsWindow(Configuration* config, VideoManager* videoManager,
			Player* player);
	void refresh();
	~CharStatsWindow();
};
}

#endif /* CHARSTATSWINDOW_H_ */
