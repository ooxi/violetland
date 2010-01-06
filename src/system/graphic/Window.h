#include <map>
#include <string>
#include "text/TextObject.h"
#include "../InputHandler.h"

#ifndef WINDOW_H_
#define WINDOW_H_

class Window {
private:
	float m_left, m_top, m_right, m_bottom, m_r, m_g, m_b, m_a;
	std::map<std::string, TextObject*> m_elements;
	std::map<std::string, void(*)()> m_handlers;
public:
	Window(float x, float y, int w, int h, float r, float g, float b, float a);
	void addElement(std::string name, TextObject* element);
	void removeElement(std::string name, bool remainHandler);
	void addHandler(std::string elementName, void(*func)());
	void removeHandler(std::string elementName);
	void process(InputHandler* input);
	void draw();
	bool CloseFlag;
	~Window();
};

#endif /* WINDOW_H_ */
