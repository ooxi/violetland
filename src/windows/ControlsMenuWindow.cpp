#include "ControlsMenuWindow.h"

#include <boost/bind.hpp>
#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)
#include <string>

#include "../system/Configuration.h"
#include "../system/graphic/text/TextManager.h"





violet::ControlsMenuWindow::ControlsMenuWindow(
			const Configuration* config,
			const TextManager* text
		) :	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f),
			m_config(config),
			m_text(text)
		{
	
	refresh();
}



void violet::ControlsMenuWindow::addControlElement(
			unsigned i, unsigned strN,
			unsigned lx, unsigned rx
		) {
	
	unsigned y = m_text->getHeight() * strN;
	std::string eventId = InputHandler::getEventIdentifier(i);
	std::string eventName = InputHandler::getEventName(i);
	std::string keyName = InputHandler::getKeyName(m_config->PlayerInputBinding[i]);

	addElement(eventId, eventName, m_text, lx, y,
			TextManager::LEFT, TextManager::MIDDLE);

	addElement(eventId + "key", keyName, m_text, rx, y,
			TextManager::RIGHT, TextManager::MIDDLE);

	addHandler(Window::hdl_lclick, eventId, boost::bind(&ControlsMenuWindow::onEventClick, this, _1));
}



void violet::ControlsMenuWindow::refresh() {
	const int col1_l = m_config->Screen.Width * 0.1f;
	const int col1_r = m_config->Screen.Width * 0.45f;

	addElement("controls", _("Controls"), m_text,
			col1_l, m_text->getHeight() * 2.0f,
			TextManager::LEFT, TextManager::MIDDLE
	);
	const int col2_l = m_config->Screen.Width * 0.55f;
	const int col2_r = m_config->Screen.Width * 0.9f;


	/* Change the control style
	 */
	addElement("control-style", _("Control style"),
			m_text,
			col1_l,  (m_text->getHeight() + 35) * 2.0f,
			TextManager::LEFT, TextManager::MIDDLE
	);
	addElement("control-style-value", ControlStyleToString(m_config->Control),
			m_text,
			col1_r,  (m_text->getHeight() + 35) * 2.0f,
			TextManager::RIGHT, TextManager::MIDDLE
	);
	addHandler(Window::hdl_lclick, "control-style", boost::bind(&ControlsMenuWindow::onControlStyleClick, this));


	unsigned col1_items = (InputHandler::GameInputEventsCount + 1) / 2;

	for (unsigned i = 0; i < col1_items; i++)
		addControlElement(i, i + 6, col1_l, col1_r);

	for (unsigned i = col1_items; i < InputHandler::GameInputEventsCount; i++)
		addControlElement(i, i - col1_items + 6, col2_l, col2_r);
}



violet::ControlsMenuWindow::~ControlsMenuWindow() {
	// nothing
}
