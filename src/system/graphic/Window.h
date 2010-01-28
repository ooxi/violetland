#include <map>
#include <string>
#include "text/TextObject.h"
#include "../InputHandler.h"

#ifndef WINDOW_H_
#define WINDOW_H_

class Window {
protected:
	float m_left, m_top, m_right, m_bottom, m_r, m_g, m_b, m_a;
	std::map<std::string, TextObject*> m_elements;
	std::map<std::string, void(*)()> m_lcHandlers;
	std::map<std::string, void(*)()> m_rcHandlers;
	std::map<std::string, void(*)()> m_mvHandlers;
public:
	enum HandlerType {
		hdl_all = 0, hdl_click, hdl_lclick, hdl_rclick, hdl_move
	};
	Window(float x, float y, int w, int h, float r, float g, float b, float a);
	void addElement(std::string name, TextObject* element);
	void removeElement(std::string name, bool remainHandler);
	void addHandler(HandlerType hdl, std::string elementName, void(*func)());
	void removeHandler(HandlerType hdl, std::string elementName);
	void process(InputHandler* input);
	void draw();
	bool CloseFlag;
	~Window();
};

#endif /* WINDOW_H_ */
