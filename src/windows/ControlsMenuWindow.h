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
	void onEventClick(std::string eventId);

public:

	ControlsMenuWindow(Configuration const* config, TextManager const* text);
	virtual ~ControlsMenuWindow();

	/* @todo Should be private
	 */
	void refresh();
};
}

#endif	/* VIOLET_CONTROLSMENUWINDOW_H */
