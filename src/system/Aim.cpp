#include "Aim.h"

void Aim::drawCircle(float r) {
	glBegin(GL_LINES);
	for (int i = 0; i < 360; i += 4) {
		const float radS = i * M_PI / 180;
		const float radE = (i + 4) * M_PI / 180;
		glVertex3f(r * cos(radS), r * sin(radS), 0.0f);
		glVertex3f(r * cos(radE), r * sin(radE), 0.0f);
	}
	glEnd();
}

void Aim::drawFilledCircle(float r) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 360; i += 4) {
		const float radS = i * M_PI / 180;
		const float radE = (i + 4) * M_PI / 180;
		glVertex3f(r * cos(radS), r * sin(radS), 0.0f);
		glVertex3f(r * cos(radE), r * sin(radE), 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
	}
	glEnd();
}

Aim::Aim(int colorDark, int colorLight) {
	printf("Constructing aim...\n");

	float cDarkR = ImageUtility::getColorChR(colorDark);
	float cDarkG = ImageUtility::getColorChG(colorDark);
	float cDarkB = ImageUtility::getColorChB(colorDark);

	float cLightR = ImageUtility::getColorChR(colorLight);
	float cLightG = ImageUtility::getColorChG(colorLight);
	float cLightB = ImageUtility::getColorChB(colorLight);

	m_aimDListId = glGenLists(1);

	glNewList(m_aimDListId, GL_COMPILE);
	glLineWidth(3.75f);
	glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
	drawCircle(25.0f);

	glLineWidth(1.25f);
	glColor4f(cLightR, cLightG, cLightB, 1.0f);
	drawCircle(25.0f);
	glEndList();

	m_pointDListId = glGenLists(1);

	glNewList(m_pointDListId, GL_COMPILE);
	glColor4f(cLightR, cLightG, cLightB, 1.0f);
	drawFilledCircle(20.0f);
	glLineWidth(0.3f);
	glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
	drawCircle(20.0f);
	glEndList();
}

void Aim::draw(float x, float y, float scale, float pointScale) {
	glPushMatrix();

	glTranslatef(x, y, 0.0f);

	glPushMatrix();

	glScalef(scale, scale, scale);
	glCallList(m_aimDListId);

	glPopMatrix();

	if (pointScale < 0)
		pointScale = 0;

	glScalef(pointScale, pointScale, pointScale);
	glCallList(m_pointDListId);

	glPopMatrix();
}

Aim::~Aim() {
	glDeleteLists(m_aimDListId, 1);
	glDeleteLists(m_pointDListId, 1);
}
