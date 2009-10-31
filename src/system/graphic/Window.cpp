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
}

void Window::addElement(std::string name, TextObject* element) {
	if (m_elements.count(name) > 0) {
		delete m_elements.find(name)->second;
		m_elements.erase(name);
	}
	m_elements.insert(std::map<std::string, TextObject*>::value_type(name,
			element));
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

Window::~Window() {
	std::map<std::string, TextObject*>::const_iterator iter;
	for (iter = m_elements.begin(); iter != m_elements.end(); ++iter) {
		delete iter->second;
	}
	m_elements.clear();
}
