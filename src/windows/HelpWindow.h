#ifndef HELPWINDOW_H_
#define HELPWINDOW_H_

#include <sstream>

#include "../system/graphic/Window.h"
#include "../system/Configuration.h"
#include "../system/graphic/text/TextManager.h"

class HelpWindow: public Window {
public:
	HelpWindow(Configuration* config, TextManager* text);
	~HelpWindow();
};

#endif /* HELPWINDOW_H_ */
