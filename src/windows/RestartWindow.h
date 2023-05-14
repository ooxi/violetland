#ifndef VIOLET_RESTARTWINDOW_H
#define	VIOLET_RESTARTWINDOW_H

#include <set>

#include "AlertWindow.h"

#define _(STRING) gettext(STRING)

namespace violet {
	class RestartWindow : public AlertWindow {
	private:
		void onOKClick();
	public:
		RestartWindow(Configuration const* config, TextManager const* text) :
			AlertWindow(config, text, _("Restart the game to apply changes")) {/* Do nothing */}
		~RestartWindow() {/* Do noting */}
	};
}

#endif /* VIOLET_RESTARTWINDOW_H */
