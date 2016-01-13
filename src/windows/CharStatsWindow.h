#ifndef VIOLET_CHARSTATSWINDOW_H_
#define VIOLET_CHARSTATSWINDOW_H_

#include "Window.h"
#include "../system/graphic/VideoManager.h"
#include "../game/lifeforms/Player.h"

namespace violet {

class CharStatsWindow: public Window {
	VideoManager* m_videoManager;
	Player* m_player;
	map<std::string, std::string> m_perkDetails;

	void increasePlayerParam(std::string paramName);
	void showPerkDetails(std::string perkName);
	void givePerkToPlayer(std::string perkName);
	
	void onPlayerParamClickEvent(std::string paramName);
	void onPerkHoverEvent(std::string perkName);
	void onPerkClickEvent(std::string perkName);
	
public:
	static const char* perkIds[];
	static const unsigned perkIdsNumber;
	static const char* paramIds[];
	static const unsigned paramIdsNumber;

	CharStatsWindow(Configuration* config, VideoManager* videoManager,
			Player* player);
	void refresh();
	virtual ~CharStatsWindow();
};
}

#endif /* VIOLET_CHARSTATSWINDOW_H_ */
