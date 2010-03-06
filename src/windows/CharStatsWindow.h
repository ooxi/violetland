#ifndef CHARSTATSWINDOW_H_
#define CHARSTATSWINDOW_H_

#include "../system/graphic/Window.h"
#include "../system/Configuration.h"
#include "../system/graphic/VideoManager.h"

class CharStatsWindow: public Window {
public:
	CharStatsWindow(Configuration* config, VideoManager* videoManager);
	~CharStatsWindow();
};

#endif /* CHARSTATSWINDOW_H_ */
