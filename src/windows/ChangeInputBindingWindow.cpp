#include "ChangeInputBindingWindow.h"

#include <libintl.h>
#define _(STRING) gettext(STRING)

#include "../system/Configuration.h"
#include "../system/graphic/text/TextManager.h"





violet::ChangeInputBindingWindow::ChangeInputBindingWindow(
			Configuration const* config,
			TextManager const* text
		) :	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f)
		{
	
	addElement("pressakey", _("Press a key, please..."),
			text, config->Screen.Width / 2,
			config->Screen.Height / 2, TextManager::CENTER, TextManager::MIDDLE);
}



violet::ChangeInputBindingWindow::~ChangeInputBindingWindow() {
	// nothing
}
