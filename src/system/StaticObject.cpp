#include "StaticObject.h"

StaticObject::StaticObject(float x, float y, int w, int h, Texture* texture,
		bool takeCareOfTexture) :
	Object(x, y, w, h) {
	m_texture = texture;
	m_takeCareOfTexture = takeCareOfTexture;
}

GLuint StaticObject::createComplexFace(int facesCount) {
	GLuint dListId = glGenLists(1);

	glNewList(dListId, GL_COMPILE);
	float faceWidth = (float) m_width / facesCount;
	float faceHeight = (float) m_height / facesCount;
	glBegin(GL_QUADS);
	for (int i = 0; i < facesCount; i++) {
		for (int j = 0; j < facesCount; j++) {
			float faceStartX = (float) i / facesCount;
			float faceStartY = (float) j / facesCount;
			float faceEndX = (float) (i + 1) / facesCount;
			float faceEndY = (float) (j + 1) / facesCount;

			glNormal3f(0.0f, 0.0f, 1.0f);
			glColor4f(RMask, GMask, BMask, AMask);

			glTexCoord2f(faceStartX, faceStartY);
			glVertex3f(m_left + faceWidth * i, m_top + faceHeight * j, 0);
			glTexCoord2f(faceEndX, faceStartY);
			glVertex3f(m_left + faceWidth * (i + 1), m_top + faceHeight * j, 0);
			glTexCoord2f(faceEndX, faceEndY);
			glVertex3f(m_left + faceWidth * (i + 1), m_top + faceHeight * (j
					+ 1), 0);
			glTexCoord2f(faceStartX, faceEndY);
			glVertex3f(m_left + faceWidth * i, m_top + faceHeight * (j + 1), 0);
		}
	}
	glEnd();
	glEndList();

	return dListId;
}

void StaticObject::drawQuad(float texRight, float texBottom)
{
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(m_left, m_top, 0);
	glTexCoord2f(texRight, 0);
	glVertex3f(m_right, m_top, 0);
	glTexCoord2f(texRight, texBottom);
	glVertex3f(m_right, m_bottom, 0);
	glTexCoord2f(0, texBottom);
	glVertex3f(m_left, m_bottom, 0);
	glEnd();
}

void StaticObject::draw(bool outlined, float x, float y) {
	glEnable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5
	float right = m_texture->getWidth();
	float bottom = m_texture->getHeight();

	glBindTexture(m_texture->getType(), m_texture->getTextureId());

	glPushMatrix();

	glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glScalef(1.0f, 1.0f, 1.0f);

	glPushMatrix();
	glColor4f(0.0f, 0.0f, 0.0f, AMask);
	glTranslatef(x - 1.0f, y, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x - 1.0f, y - 1.0f, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y - 1.0f, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x + 1.0f, y - 1.0f, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x + 1.0f, y, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x + 1.0f, y + 1.0f, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x, y + 1.0f, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x - 1.0f, y + 1.0f, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPushMatrix();
	glColor4f(RMask, GMask, BMask, AMask);
	glTranslatef(x, y, 0.0f);
	drawQuad(right, bottom);
	glPopMatrix();

	glPopMatrix();

	glDisable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5
}

void StaticObject::draw(bool reflection, float x, float y, float angle, float scale) {
	GLfloat right = 1;
	GLfloat bottom = 1;

	if (m_texture->getType() == 0x84F5) {
		glEnable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5
		right = m_texture->getWidth();
		bottom = m_texture->getHeight();
	}

	glBindTexture(m_texture->getType(), m_texture->getTextureId());

	glPushMatrix();

	glTranslatef(x, y, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glScalef(scale, scale, scale);

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);

	glColor4f(RMask, GMask, BMask, AMask);

	glTexCoord2f(0, reflection ? bottom : 0);
	glVertex3f(m_left, m_top, 0);
	glTexCoord2f(right, reflection ? bottom : 0);
	glVertex3f(m_right, m_top, 0);
	glTexCoord2f(right, reflection ? 0 : bottom);
	glVertex3f(m_right, m_bottom, 0);
	glTexCoord2f(0, reflection ? 0 : bottom);
	glVertex3f(m_left, m_bottom, 0);
	glEnd();

	glPopMatrix();

	if (m_texture->getType() == 0x84F5)
		glDisable(0x84F5); //GL_TEXTURE_RECTANGLE_NV = GL_TEXTURE_RECTANGLE_ARB = 0x84F5
}

void StaticObject::draw(bool reflection) {
	draw(reflection, X, Y, Angle, Scale);
}

void StaticObject::draw(GLuint dListId) {
	glBindTexture(m_texture->getType(), m_texture->getTextureId());

	glPushMatrix();

	glTranslatef(X, Y, 0.0f);
	glRotatef(Angle, 0.0f, 0.0f, 1.0f);
	glScalef(Scale, Scale, Scale);

	glCallList(dListId);

	glPopMatrix();
}

void StaticObject::setTexture(Texture* texture, bool takeCareOfTexture) {
	if (m_takeCareOfTexture)
		delete m_texture;

	m_texture = texture;
	m_takeCareOfTexture = takeCareOfTexture;
}

Texture* StaticObject::getTexture() {
	return m_texture;
}

StaticObject::~StaticObject() {
	if (m_takeCareOfTexture)
		delete m_texture;
}
