#include "Window.h"
#include "WindowManager.h"





void violet::WindowManager::open(Window* window) {
	m_windows.push_back(window);
}



void violet::WindowManager::process(InputHandler* input) {
	for (std::vector<Window*>::const_iterator it = m_windows.begin(); it != m_windows.end(); ++it) {
		(*it)->process(input);
	}
}



void violet::WindowManager::draw() {
	
	/* Erase closed windows before drawing
	 */
	for (std::vector<Window*>::iterator it = m_windows.begin(); it != m_windows.end(); ++it) {
		if ((*it)->CloseFlag) {
			it = m_windows.erase(it);
			it--;
		}
	}
	
	/* Draw open windows
	 */
	for (std::vector<Window*>::const_iterator it = m_windows.begin(); it != m_windows.end(); ++it) {
		(*it)->draw();
	}
}
