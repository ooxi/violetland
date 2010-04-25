#include "Window.h"

Window::Window(float x, float y, int w, int h, float r, float g, float b,
		float a) {
	m_left = x;
	m_top = y;
	m_right = x + w;
	m_bottom = y + h;
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
	CloseFlag = false;
}

void Window::addElement(std::string name, TextObject* element) {
	removeElement(name, true);
	m_elements[name] = element;
}

void Window::removeElement(std::string name, bool remainHandler) {
	if (m_elements.count(name) > 0) {
		delete m_elements.find(name)->second;
		m_elements.erase(name);
	}
	if (!remainHandler)
		removeHandler(hdl_click, name);
}

void Window::addHandler(HandlerType hdl, std::string elementName, void(*func)()) {
	removeHandler(hdl, elementName);
	if (hdl == hdl_click || hdl == hdl_lclick)
		m_lcHandlers[elementName] = func;
	if (hdl == hdl_click || hdl == hdl_rclick)
		m_rcHandlers[elementName] = func;
	if (hdl == hdl_move)
		m_mvHandlers[elementName] = func;
}

void Window::removeHandler(HandlerType hdl, std::string elementName) {
	if ((hdl == hdl_all || hdl == hdl_click || hdl == hdl_lclick)
			&& m_lcHandlers.count(elementName) > 0) {
		m_lcHandlers.erase(elementName);
	}
	if ((hdl == hdl_all || hdl == hdl_click || hdl == hdl_rclick)
			&& m_rcHandlers.count(elementName) > 0) {
		m_rcHandlers.erase(elementName);
	}
	if ((hdl == hdl_all || hdl == hdl_move) && m_mvHandlers.count(elementName)
			> 0) {
		m_mvHandlers.erase(elementName);
	}
}

void Window::draw() {
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glLoadIdentity();

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);

	glColor4f(m_r, m_g, m_b, m_a);

	glVertex3f(m_left, m_top, 0.0f);
	glVertex3f(m_right, m_top, 0.0f);
	glVertex3f(m_right, m_bottom, 0.0f);
	glVertex3f(m_left, m_bottom, 0.0f);

	glEnd();

	glPopMatrix();

	glEnable(GL_TEXTURE_2D);

	std::map<std::string, TextObject*>::const_iterator iter;
	for (iter = m_elements.begin(); iter != m_elements.end(); ++iter) {
		TextObject* o = iter->second;
		o->draw(true, o->X, o->Y);
	}
}

void Window::process(InputHandler* input) {
	int gmx = input->mouseX;
	int gmy = input->mouseY;

	std::map<std::string, void(*)()>::const_iterator iter;
	for (iter = m_mvHandlers.begin(); iter != m_mvHandlers.end(); ++iter) {
		if (m_elements.count(iter->first) > 0) {
			TextObject* o = m_elements.find(iter->first)->second;
			if (gmx > o->getLeft() && gmx < o->getRight() && gmy > o->getTop()
					&& gmy < o->getBottom()) {
				iter->second();
			}
		}
	}

	//TODO: refactor next two copypasted cycles
	for (iter = m_lcHandlers.begin(); iter != m_lcHandlers.end(); ++iter) {
		if (m_elements.count(iter->first) > 0) {
			TextObject* o = m_elements.find(iter->first)->second;
			if (gmx > o->getLeft() && gmx < o->getRight() && gmy > o->getTop()
					&& gmy < o->getBottom()) {
				o->GMask = 0.3f;
				if (input->getPressInput(InputHandler::MenuClickA)) {
					iter->second();
				}
			} else {
				o->GMask = 1.0f;
			}
		}
	}

	for (iter = m_rcHandlers.begin(); iter != m_rcHandlers.end(); ++iter) {
		if (m_elements.count(iter->first) > 0) {
			TextObject* o = m_elements.find(iter->first)->second;
			if (gmx > o->getLeft() && gmx < o->getRight() && gmy > o->getTop()
					&& gmy < o->getBottom()) {
				o->RMask = 0.3f;
				if (input->getPressInput(InputHandler::MenuClickB)) {
					iter->second();
				}
			} else {
				o->RMask = 1.0f;
			}
		}
	}
}

Window::~Window() {
	std::map<std::string, TextObject*>::const_iterator iter;
	for (iter = m_elements.begin(); iter != m_elements.end(); ++iter) {
		delete iter->second;
	}
	m_elements.clear();
	m_lcHandlers.clear();
	m_rcHandlers.clear();
}
