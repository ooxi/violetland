#include "Aim.h"

void Aim::constructCircle(float r) {
	glBegin(GL_LINES);
	for (int i = 0; i < 360; i += 4) {
		const float radS = i * M_PI / 180;
		const float radE = (i + 4) * M_PI / 180;
		glVertex3f(r * cos(radS), r * sin(radS), 0.0f);
		glVertex3f(r * cos(radE), r * sin(radE), 0.0f);
	}
	glEnd();
}

void Aim::constructFilledCircle(float r) {
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

void Aim::constructAim(float cDarkR,float cDarkG,float cDarkB,float cLightR,float cLightG,float cLightB){
	printf("Constructing aim...\n");

	m_aimDListId = glGenLists(1);

	glNewList(m_aimDListId, GL_COMPILE);
	glLineWidth(3.75f);
	glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
	constructCircle(25.0f);

	glLineWidth(1.25f);
	glColor4f(cLightR, cLightG, cLightB, 1.0f);
	constructCircle(25.0f);
	glEndList();

	m_pointDListId = glGenLists(1);

	glNewList(m_pointDListId, GL_COMPILE);
	glColor4f(cLightR, cLightG, cLightB, 1.0f);
	constructFilledCircle(20.0f);
	glLineWidth(0.3f);
	glColor4f(cDarkR, cDarkG, cDarkB, 1.0f);
	constructCircle(20.0f);
	glEndList();
}

Aim::Aim(float cDarkR,float cDarkG,float cDarkB,float cLightR,float cLightG,float cLightB)
{
	constructAim(cDarkR,cDarkG,cDarkB,cLightR,cLightG,cLightB);
}

Aim::Aim(Configuration* config){
	float cDarkR = ImageUtility::getColorChR(config->AimColorA);
	float cDarkG = ImageUtility::getColorChG(config->AimColorA);
	float cDarkB = ImageUtility::getColorChB(config->AimColorA);

	float cLightR = ImageUtility::getColorChR(config->AimColorB);
	float cLightG = ImageUtility::getColorChG(config->AimColorB);
	float cLightB = ImageUtility::getColorChB(config->AimColorB);

	constructAim(cDarkR,cDarkG,cDarkB,cLightR,cLightG,cLightB);
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
