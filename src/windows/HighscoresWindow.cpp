#include "HighscoresWindow.h"

#include <boost/bind.hpp>
#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)
#include <set>
#include <sstream>

#include "../game/Highscores.h"
#include "../system/Configuration.h"
#include "../system/graphic/text/TextManager.h"





violet::HighscoresWindow::HighscoresWindow(
			Configuration const* config,
			TextManager const* text,
			Highscores const& highscores
		) :	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f),
			m_config(config),
			m_text(text)
		{

	const int l = m_config->Screen.Width * 0.1f;
	const int r2 = l * 2.0f;
	const int r3 = l * 5.0f;
	const int r4 = l * 7.0f;
	const int h = m_text->getHeight();

	addElement("highscores", _("Highscores"), m_text, l,
			2 * h, TextManager::LEFT, TextManager::MIDDLE);

	addElement("headerXp", _("XP"), m_text, l, 4 * h,
			TextManager::LEFT, TextManager::MIDDLE);
	addElement("headerParams", _("Str/Agil/Vital"),
			m_text, r2, 4 * h, TextManager::LEFT,
			TextManager::MIDDLE);
	addElement("headerTime", _("Time"), m_text, r3,
			4 * h, TextManager::LEFT, TextManager::MIDDLE);
	addElement("headerName", _("Name"), m_text, r4,
			4 * h, TextManager::LEFT, TextManager::MIDDLE);

	std::set<HighscoresEntry> entries = highscores.getData();
	std::set<HighscoresEntry>::reverse_iterator it = entries.rbegin();
	for (unsigned i = 0; it != entries.rend(); ++it, ++i) {
		std::ostringstream oss1, oss2;

		oss1 << "xp" << i;
		oss2 << it->Xp;
		addElement(oss1.str(), oss2.str(), m_text, l,
				(5 + i) * h, TextManager::LEFT, TextManager::MIDDLE);

		oss1.str("");
		oss2.str("");
		oss1 << "params" << i;
		oss2 << (int) (it->Strength * 100) << '/' << (int) (it->Agility * 100)
				<< '/' << (int) (it->Vitality * 100);
		addElement(oss1.str(), oss2.str(), m_text, r2,
				(5 + i) * h, TextManager::LEFT, TextManager::MIDDLE);

		const int minutes = it->Time / 60000;
		const int seconds = (it->Time - minutes * 60000) / 1000;

		oss1.str("");
		oss2.str("");
		oss1 << "time" << i;
		oss2 << minutes << "m " << seconds << 's';
		addElement(oss1.str(), oss2.str(), m_text, r3,
				(5 + i) * h, TextManager::LEFT, TextManager::MIDDLE);

		oss1.str("");
		oss1 << "name" << i;
		addElement(oss1.str(), it->Name, m_text, r4,
				(5 + i) * h, TextManager::LEFT, TextManager::MIDDLE);
	}

	addElement("back", _("Back to main menu"), m_text, l,
			16 * h, TextManager::LEFT, TextManager::MIDDLE);

	addElement("reset", _("Reset list"), m_text, r3,
			16 * h, TextManager::LEFT, TextManager::MIDDLE);

	addHandler(Window::hdl_lclick, "back", boost::bind(&HighscoresWindow::onBackClick, this));
	addHandler(Window::hdl_lclick, "reset", boost::bind(&HighscoresWindow::onResetClick, this));
}



violet::HighscoresWindow::~HighscoresWindow() {
	// nothing
}
