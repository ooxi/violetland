#ifndef VIOLET_WINDOW_H_
#define VIOLET_WINDOW_H_

#include <boost/function.hpp>
#include <map>
#include <string>

#include "../system/graphic/text/TextObject.h"
#include "../system/graphic/text/TextManager.h"
#include "../system/InputHandler.h"

namespace violet {

class Window {

private:
	float m_left, m_top, m_right, m_bottom, m_r, m_g, m_b, m_a;
	std::map<std::string, TextObject*> m_elements;

	std::map<std::string, boost::function<void (std::string)> > m_lcHandlers;
	std::map<std::string, boost::function<void (std::string)> > m_rcHandlers;
	std::map<std::string, boost::function<void (std::string)> > m_mvHandlers;

protected:
	
	enum HandlerType {
		hdl_all = 0, hdl_click, hdl_lclick, hdl_rclick, hdl_move
	};
	
	Window(float x, float y, int w, int h, float r, float g, float b, float a);
	
	void addElement(std::string id, TextObject* element);
	void addElement(std::string id, std::string text, TextManager const* manager, 
			int x, int y, TextManager::TextHAlignFlag halign, 
			TextManager::TextVAlignFlag valign);
	void addElement(const Label label, TextManager const* manager, 
			int x, int y, TextManager::TextHAlignFlag halign, 
			TextManager::TextVAlignFlag valign);
	void addElements(const std::vector<Label>& labels, 
			TextManager const* manager, int x, int y, int vstep, 
			TextManager::TextHAlignFlag halign, TextManager::TextVAlignFlag valign);
	void removeElement(std::string name, bool remainHandler);
	void addHandler(HandlerType hdl, std::string elementName, boost::function<void (std::string)> handler);
	void removeHandler(HandlerType hdl, std::string elementName);
	
public:

	void process(InputHandler* input);
	void draw();
	bool CloseFlag;
	virtual ~Window();
};
}

#endif /* VIOLET_WINDOW_H_ */
