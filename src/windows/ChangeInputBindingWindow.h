#ifndef VIOLET_CHANGEINPUTBINDINGWINDOW_H
#define	VIOLET_CHANGEINPUTBINDINGWINDOW_H

#include "Window.h"

namespace violet {

class Configuration;
class TextManager;

class ChangeInputBindingWindow : public Window {
	
public:

	ChangeInputBindingWindow(Configuration const* config, TextManager const* text);
	virtual ~ChangeInputBindingWindow();
};
}

#endif /* VIOLET_CHANGEINPUTBINDINGWINDOW_H */
