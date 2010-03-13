#include "HUD.h"

HUD::HUD(VideoManager* videoManager) {
	m_videoManager = videoManager;
	m_bottomBasePoint = m_videoManager->getVideoMode().Height
			- m_videoManager->RegularText->getIndent();
	reset();
}

void HUD::drawMessages() {
	if (!m_messages.empty()) {
		int s = m_messages.size();
		for (int i = s - 1; i >= 0; i--) {
			m_messages[i]->draw(true, m_messages[i]->X
					+ m_videoManager->SmallText->getIndent(), m_bottomBasePoint
					- m_videoManager->RegularText->getHeight() - s
					* m_videoManager->SmallText->getHeight() + i
					* m_videoManager->SmallText->getHeight());
			m_messages[i]->AMask -= 0.0001f
					* m_videoManager->getFrameDeltaTime();

			if (m_messages[i]->AMask <= 0) {
				delete m_messages[i];
				m_messages.erase(m_messages.begin() + i);
			}
		}
	}
}

void HUD::drawExperience(float experience, int levelPoints) {
	VideoMode screen = m_videoManager->getVideoMode();
	const int barLen = screen.Width / 4;
	const int barLeft = 2 * screen.Width / 3;
	const int barHeight = m_videoManager->RegularText->getHeight() / 4;

	glPushMatrix();

	glTranslatef(barLeft, m_bottomBasePoint, 0.0f);

	glBegin( GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);

	glColor4f(0.7f, 0.7f, 0.7f, 0.5f);

	glVertex3f(0.0f, -barHeight, 0);
	glVertex3f(barLen, -barHeight, 0);
	glVertex3f(barLen, barHeight, 0);
	glVertex3f(0.0f, barHeight, 0);

	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, -barHeight, 0);
	glColor4f(0.5f, 0.5f, 1.0f, 1.0f);
	glVertex3f(barLen * experience, -barHeight, 0);
	glVertex3f(barLen * experience, barHeight, 0);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, barHeight, 0);

	glEnd();

	glPopMatrix();
}

void HUD::drawHealth(float health) {
	VideoMode screen = m_videoManager->getVideoMode();
	const int barLeft = screen.Width / 12;
	const int barLen = screen.Width / 4;
	const int barHeight = m_videoManager->RegularText->getHeight() / 4;

	glPushMatrix();

	glTranslatef(barLeft, m_bottomBasePoint, 0.0f);

	glBegin( GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);

	glColor4f(0.7f, 0.7f, 0.7f, 0.5f);

	glVertex3f(0.0f, -barHeight, 0);
	glVertex3f(barLen, -barHeight, 0);
	glVertex3f(barLen, barHeight, 0);
	glVertex3f(0.0f, barHeight, 0);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, -barHeight, 0);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(barLen * health, -barHeight, 0);
	glVertex3f(barLen * health, barHeight, 0);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, barHeight, 0);

	glEnd();

	glPopMatrix();
}

void HUD::drawTime(GameState* gameState) {
	const int minutes = gameState->Time / 60000;
	const int seconds = (gameState->Time - minutes * 60000) / 1000;

	char *buf;
	sprintf(buf = new char[20], "%02d:%02d", minutes, seconds);
	m_videoManager->RegularText->draw(buf, m_videoManager->getVideoMode().Width
			/ 2, m_bottomBasePoint, TextManager::CENTER, TextManager::MIDDLE);
	delete[] buf;
}

void HUD::draw(GameState* gameState, float health, float experience,
		int levelPoints, int ammo, int grenades) {
	VideoMode screen = m_videoManager->getVideoMode();

	drawMessages();
	drawTime(gameState);

	glDisable( GL_TEXTURE_2D);
	drawHealth(health);
	drawExperience(experience, levelPoints);
	glEnable(GL_TEXTURE_2D);

	if (gameState->Lost && !gameState->Paused)
		m_videoManager->RegularText->draw("They have overcome...", screen.Width
				/ 2, screen.Height / 3, TextManager::CENTER,
				TextManager::MIDDLE);

	if (gameState->Paused)
		m_videoManager->RegularText->draw("PAUSE", screen.Width / 2,
				screen.Height / 2, TextManager::CENTER, TextManager::MIDDLE);
}

void HUD::addMessage(std::string message) {
	m_messages.push_back(m_videoManager->SmallText->getObject(message.c_str(),
			0, 0, TextManager::LEFT, TextManager::MIDDLE));
}

void HUD::reset() {
	clearVector<TextObject*> (&m_messages);
}

HUD::~HUD() {
	reset();
}
