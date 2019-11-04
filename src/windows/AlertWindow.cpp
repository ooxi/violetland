#include "AlertWindow.h"

#include <boost/bind.hpp>
#include <libintl.h>
#include <set>
#include <sstream>

#include "../system/Configuration.h"
#include "../system/graphic/text/TextManager.h"

#define _(STRING) gettext(STRING)

violet::AlertWindow::AlertWindow(
			Configuration const* config,
			TextManager const* text,
			std::string message
		) :	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f),
			m_config(config),
			m_text(text)
		{

	const int l = m_config->Screen.Width * 0.1f;
	const int h = m_text->getHeight();

	addElement("message", message, m_text, l,
			   2 * h, TextManager::LEFT, TextManager::MIDDLE);
	addElement("OK", _("OK"), m_text, l,
			   16 * h, TextManager::LEFT, TextManager::MIDDLE);
	addHandler(Window::hdl_lclick, "OK", boost::bind(&AlertWindow::onOKClick, this));
}

violet::AlertWindow::~AlertWindow() {
	// nothing
}
