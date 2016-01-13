#ifndef VIOLET_OPTIONSWINDOW_H
#define	VIOLET_OPTIONSWINDOW_H

#include <string>

#include "Window.h"

namespace violet {

class Configuration;

class OptionsWindow : public Window {

private:
	Configuration const* m_config;
	TextManager const* m_text;
	
	void onAutoReloadClick();
	void onAutoWeaponPickupClick();
	void onFriendlyFireClick();
	
	void switchVolumeUp(std::string elementName);
	void switchVolumeDown(std::string elementName);
	
	void onFullscreenClick();
	void onResolutionUpClick();
	void onResolutionDownClick();
	void onControlsMenuClick();
	void onResetControlsClick();
	void onSaveAndReturnClick();

public:

	OptionsWindow(Configuration const* config, TextManager const* text);
	virtual ~OptionsWindow();

	/* @todo Should be private
	 */
	void refresh(Configuration const* tempConfig);
};
}

#endif /* VIOLET_OPTIONSWINDOW_H */
