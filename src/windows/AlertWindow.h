#ifndef VIOLET_ALERTWINDOW_H
#define	VIOLET_ALERTWINDOW_H

#include <set>

#include "Window.h"

namespace violet {

class Configuration;
class TextManager;

class AlertWindow : public Window {

private:
	Configuration const* m_config;
	TextManager const* m_text;

	virtual void onOKClick() {}

public:

	AlertWindow(Configuration const* config, TextManager const* text, std::string message);
	virtual ~AlertWindow();
};
}

#endif /* VIOLET_ALERTWINDOW_H */
