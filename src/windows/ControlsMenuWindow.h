#ifndef VIOLET_CONTROLSMENUWINDOW_H
#define	VIOLET_CONTROLSMENUWINDOW_H

#include "Window.h"

namespace violet {

class Configuration;
class TextManager;

class ControlsMenuWindow : public Window {

private:
	Configuration const* m_config;
	TextManager const* m_text;
	
	void addControlElement(unsigned i, unsigned strN, unsigned lx, unsigned rx);
	
	void onControlStyleClick();
	void onControlPresetClick();
	void onEventClick(std::string eventId);
	
	void refresh();

public:

	ControlsMenuWindow(Configuration const* config, TextManager const* text);
	virtual ~ControlsMenuWindow();

};
}

#endif	/* VIOLET_CONTROLSMENUWINDOW_H */
