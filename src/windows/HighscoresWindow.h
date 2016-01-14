#ifndef VIOLET_HIGHSCORESWINDOW_H
#define	VIOLET_HIGHSCORESWINDOW_H

#include <set>

#include "Window.h"

namespace violet {

class Configuration;
class Highscores;
class TextManager;

class HighscoresWindow : public Window {

private:
	Configuration const* m_config;
	TextManager const* m_text;
	
	void onBackClick();
	void onResetClick();
	
public:

	HighscoresWindow(	Configuration const* config,
				TextManager const* text,
				Highscores const& highscores
		);
	
	virtual ~HighscoresWindow();

};
}

#endif /* VIOLET_HIGHSCORESWINDOW_H */
