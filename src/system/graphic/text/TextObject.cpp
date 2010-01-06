#include "TextObject.h"

TextObject::TextObject(float x, float y, int w, int h, Texture *texture) :
	Object(x, y, w, h) {
	m_texture = texture;
}

void TextObject::drawQuad() {
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(m_left, m_top, 0.0f);
	glTexCoord2f(m_texture->getWidth(), 0.0f);
	glVertex3f(m_right, m_top, 0.0f);
	glTexCoord2f(m_texture->getWidth(), m_texture->getHeight());
	glVertex3f(m_right, m_bottom, 0.0f);
	glTexCoord2f(0.0f, m_texture->getHeight());
	glVertex3f(m_left, m_bottom, 0.0f);
	glEnd();
}

void TextObject::draw(bool outlined, float x, float y) {
	glEnable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5

	glBindTexture(m_texture->getType(), m_texture->getTextureId());

	glPushMatrix();

	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	glColor4f(0.0f, 0.0f, 0.0f, pow(AMask, 2));

	glPushMatrix();
	glTranslatef(x - 1.0f, y, 0.0f);
	drawQuad();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x - 1.0f, y - 1.0f, 0.0f);
	drawQuad();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y - 1.0f, 0.0f);
	drawQuad();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x + 1.0f, y - 1.0f, 0.0f);
	drawQuad();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x + 1.0f, y, 0.0f);
	drawQuad();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x + 1.0f, y + 1.0f, 0.0f);
	drawQuad();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 1.0f, 0.0f);
	drawQuad();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x - 1.0f, y + 1.0f, 0.0f);
	drawQuad();
	glPopMatrix();

	glColor4f(RMask, GMask, BMask, AMask);

	glPushMatrix();
	glTranslatef(x, y, 0.0f);
	drawQuad();
	glPopMatrix();

	glPopMatrix();

	glDisable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5
}

TextObject::~TextObject() {
	delete m_texture;
}
