#ifndef VIOLET_WINDOWMANAGER_H
#define	VIOLET_WINDOWMANAGER_H

#include <vector>



namespace violet {

class InputHandler;
class Window;

class WindowManager {
	
private:
	std::vector<Window*> m_windows;
	
	
public:
	
	/**
	 * Requests to overlay the current window stack with a new window
	 * 
         * @param window Will be managed (and deleted) by the window manager
	 *     from now on
         */
	void open(Window* window);
	
	/**
	 * Will invoke `process' on all managed windows
	 * 
         * @param input New input to process
         */
	void process(InputHandler* input);
	
	/**
	 * Will paint all windows in the order they were created (newest window
	 * will be drawn on top)
         */
	void draw();

};
}

#endif /* VIOLET_WINDOWMANAGER_H */
