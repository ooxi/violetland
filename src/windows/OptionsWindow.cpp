#include "OptionsWindow.h"

#include <boost/bind.hpp>
#include <libintl.h>
#include <locale.h>
#define _(STRING) gettext(STRING)
#include <sstream>
#include <vector>

#include "../system/Configuration.h"
#include "../system/graphic/text/TextManager.h"





violet::OptionsWindow::OptionsWindow(
			Configuration const* config,
			TextManager const* text
		) :	Window(0.0f, 0.0f, config->Screen.Width, config->Screen.Height, 0.0f, 0.0f, 0.0f, 0.5f),
			m_config(config),
			m_text(text)
		{
	
	const int l = m_config->Screen.Width * 0.1f;
	const int r = m_config->Screen.Width * 0.6f;
	const int h = m_text->getHeight();

	addElement("options", _("Options"), m_text, l, 2 * h,
			TextManager::LEFT, TextManager::MIDDLE);

	std::vector<Label> gameplayLabels;
	gameplayLabels.push_back(Label("autoreload", _("Weapon autoreloading")));
	gameplayLabels.push_back(Label("autopickup", _("Weapon autotaking")));
	gameplayLabels.push_back(Label("friendlyfire", _("Friendly fire")));

	addElement("sectiongame", _("Gameplay"), m_text, l,
			4 * h, TextManager::LEFT, TextManager::MIDDLE);
	addElements(gameplayLabels, m_text, l + 2 * h, 6 * h,
			h, TextManager::LEFT, TextManager::MIDDLE);

	std::vector<Label> graphicsLabels;
	graphicsLabels.push_back(Label("fullscreen", _("Fullscreen")));
	graphicsLabels.push_back(Label("resolution", _("Resolution")));

	addElement("sectiongraphics", _("Graphics"), m_text,
			r, 4 * h, TextManager::LEFT, TextManager::MIDDLE);
	addElements(graphicsLabels, m_text, r + 2 * h, 6 * h,
			h, TextManager::LEFT, TextManager::MIDDLE);

	std::vector<Label> soundLabels;
	soundLabels.push_back(Label("soundvolume", _("Sound volume")));
	soundLabels.push_back(Label("musicvolume", _("Music volume")));

	addElement("sectionsound", _("Sound"), m_text, l,
			10 * h, TextManager::LEFT, TextManager::MIDDLE);
	addElements(soundLabels, m_text, l + 3 * h, 12 * h,
			h, TextManager::LEFT, TextManager::MIDDLE);

	std::vector<Label> controlsLabels;
	controlsLabels.push_back(Label("controlsmenu", _("Edit Controls")));
	controlsLabels.push_back(Label("controlsreset", _("Reset Controls")));

	addElement("controlstitle", _("Controls"), m_text, r,
			10 * h, TextManager::LEFT, TextManager::MIDDLE);
	addElements(controlsLabels, m_text, r + 2 * h,
			12 * h, h, TextManager::LEFT, TextManager::MIDDLE);
	
	addElement("savereturn", _("Save and return"),
			m_text, l, 16 * h, TextManager::LEFT,
			TextManager::MIDDLE);
	
	
	addHandler(Window::hdl_lclick, "autoreload", boost::bind(&OptionsWindow::onAutoReloadClick, this));
	addHandler(Window::hdl_lclick, "autopickup", boost::bind(&OptionsWindow::onAutoWeaponPickupClick, this));
	addHandler(Window::hdl_lclick, "friendlyfire", boost::bind(&OptionsWindow::onFriendlyFireClick, this));
	addHandler(Window::hdl_lclick, "soundvolume", boost::bind(&OptionsWindow::switchVolumeUp, this, _1));
	addHandler(Window::hdl_rclick, "soundvolume", boost::bind(&OptionsWindow::switchVolumeDown, this, _1));
	addHandler(Window::hdl_lclick, "musicvolume", boost::bind(&OptionsWindow::switchVolumeUp, this, _1));
	addHandler(Window::hdl_rclick, "musicvolume", boost::bind(&OptionsWindow::switchVolumeDown, this, _1));
	addHandler(Window::hdl_lclick, "fullscreen", boost::bind(&OptionsWindow::onFullscreenClick, this));
	addHandler(Window::hdl_lclick, "resolution", boost::bind(&OptionsWindow::onResolutionUpClick, this));
	addHandler(Window::hdl_rclick, "resolution", boost::bind(&OptionsWindow::onResolutionDownClick, this));
	addHandler(Window::hdl_lclick, "controlsmenu", boost::bind(&OptionsWindow::onControlsMenuClick, this));
	addHandler(Window::hdl_lclick, "controlsreset", boost::bind(&OptionsWindow::onResetControlsClick, this));
	addHandler(Window::hdl_lclick, "savereturn", boost::bind(&OptionsWindow::onSaveAndReturnClick, this));
}



void violet::OptionsWindow::refresh(Configuration const* tempConfig) {
	const int l = m_config->Screen.Width * 0.1f;
	const int r = m_config->Screen.Width * 0.6f;
	const int h = m_text->getHeight();

	if (m_config->AutoReload)
		addElement("+autoreload", "+", m_text, l, 6 * h,
				TextManager::LEFT, TextManager::MIDDLE);
	else
		removeElement("+autoreload", false);

	if (m_config->AutoWeaponPickup)
		addElement("+autopickup", "+", m_text, l, 7 * h,
				TextManager::LEFT, TextManager::MIDDLE);
	else
		removeElement("+autopickup", false);

	if (m_config->FriendlyFire)
		addElement("+friendlyfire", "+", m_text, l,
				8 * h, TextManager::LEFT, TextManager::MIDDLE);
	else
		removeElement("+friendlyfire", false);

	if (m_config->Screen.Full)
		addElement("+fullscreen", "+", m_text, r, 6 * h,
				TextManager::LEFT, TextManager::MIDDLE);
	else
		removeElement("+fullscreen", false);

	std::ostringstream oss;
	oss << tempConfig->Screen.Width << 'x' << tempConfig->Screen.Height;
	addElement("+resolution", oss.str(), m_text,
			r + 8 * h, 7 * h, TextManager::LEFT, TextManager::MIDDLE);

	oss.str("");
	oss << m_config->SoundVolume * 10 << '%';
	addElement("+soundvolume", oss.str(), m_text, l,
			12 * h, TextManager::LEFT, TextManager::MIDDLE);

	oss.str("");
	oss << m_config->MusicVolume * 10 << '%';
	addElement("+musicvolume", oss.str(), m_text, l,
			13 * h, TextManager::LEFT, TextManager::MIDDLE);
}



violet::OptionsWindow::~OptionsWindow() {
	// nothing
}
