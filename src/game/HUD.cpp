#include "HUD.h"
#include <libintl.h>
#include <locale.h>
#define _(STRING)            gettext(STRING)

violetland::HUD::HUD(VideoManager* videoManager, Resources* resources) {
	m_videoManager = videoManager;

	m_resources = resources;
	m_bonusImg.insert(std::map<PlayerBonusType, StaticObject*>::value_type(
			PLAYER_BONUS_PENBULLETS, new StaticObject(0, 0, 128, 128,
					m_resources->PowerupTex[BONUS_PENBULLETS], false)));
	m_bonusImg.insert(std::map<PlayerBonusType, StaticObject*>::value_type(
			PLAYER_BONUS_AGILITYBOOST, new StaticObject(0, 0, 128, 128,
					m_resources->PowerupTex[BONUS_AGILITYROIDS], false)));
	m_bonusImg[PLAYER_BONUS_AGILITYBOOST]->RMask
			= m_bonusImg[PLAYER_BONUS_AGILITYBOOST]->GMask = 0.2f;
	m_bonusImg.insert(std::map<PlayerBonusType, StaticObject*>::value_type(
			PLAYER_BONUS_VITALITYBOOST, new StaticObject(0, 0, 128, 128,
					m_resources->PowerupTex[BONUS_VITALITYROIDS], false)));
	m_bonusImg[PLAYER_BONUS_VITALITYBOOST]->RMask
			= m_bonusImg[PLAYER_BONUS_VITALITYBOOST]->BMask = 0.2f;
	m_bonusImg.insert(std::map<PlayerBonusType, StaticObject*>::value_type(
			PLAYER_BONUS_STRENGTHBOOST, new StaticObject(0, 0, 128, 128,
					m_resources->PowerupTex[BONUS_STRENGTHROIDS], false)));
	m_bonusImg[PLAYER_BONUS_STRENGTHBOOST]->GMask
			= m_bonusImg[PLAYER_BONUS_STRENGTHBOOST]->BMask = 0.2f;
	m_bonusImg.insert(std::map<PlayerBonusType, StaticObject*>::value_type(
			PLAYER_BONUS_FREEZE, new StaticObject(0, 0, 128, 128,
					m_resources->PowerupTex[BONUS_FREEZE], false)));

	m_inventoryImg.push_back(new StaticObject(0, 0, 128, 128, m_resources->PowerupTex[BONUS_GRENADES], false));
	m_inventoryImg.push_back(new StaticObject(0, 0, 128, 128, m_resources->PowerupTex[BONUS_TELEPORTS], false));

	m_bounce = 0;
	reset();
}

void violetland::HUD::applyEffects(float health, int levelPoints) {
	m_bounce += m_videoManager->getFrameDeltaTime();

	int lMultiplier = std::min<int>(levelPoints, 4);
	double hMultiplier = std::min<float>(health, 0.5);

	//To avoid zero
	lMultiplier = std::max<int>(lMultiplier, 1);
	hMultiplier = std::max<double>(hMultiplier, 0.1);

	float factorLevel = (float) fabs(cos(m_bounce / (800.0 / lMultiplier))
			/ 2.0) + 0.5;
	float factorHealth = (float) fabs(cos(m_bounce / (800.0 * hMultiplier))
			/ 2.0) + 0.5;

	float baseScale = m_videoManager->Scale * 0.15;
	float bigScaleLevel = m_videoManager->Scale * (0.2 + (0.025 * lMultiplier));
	float bigScaleHealth = m_videoManager->Scale * (0.25 - (0.1 * hMultiplier));

	if (health < 0.75) {
		m_resources->HealthIndicator->setMask(1.0, 1.0 - factorHealth, 1.0
				- factorHealth, 1.0);
		m_resources->HealthIndicator->Scale = bigScaleHealth;
	} else {
		m_resources->HealthIndicator->setMask(1.0, 1.0, 1.0, 0.5);
		m_resources->HealthIndicator->Scale = baseScale;
	}

	if (levelPoints > 0) {
		m_resources->LevelUpIndicator->setMask(1.0 - factorLevel, 1.0
				- factorLevel, 1.0, 1.0);
		m_resources->LevelUpIndicator->Scale = bigScaleLevel;
	} else {
		m_resources->LevelUpIndicator->setMask(1.0, 1.0, 1.0, 0.5);
		m_resources->LevelUpIndicator->Scale = baseScale;
	}
}

void violetland::HUD::drawMessages() {
	if (!m_messages.empty()) {
		int m_bottomBasePoint = m_videoManager->getVideoMode().Height
				- m_videoManager->RegularText->getIndent();

		int s = m_messages.size();
		for (int i = s - 1; i >= 0; i--) {
			float x = m_messages[i]->X + m_videoManager->SmallText->getIndent();
			float y = m_bottomBasePoint
					- m_videoManager->RegularText->getHeight() - s
					* m_videoManager->SmallText->getHeight() + i
					* m_videoManager->SmallText->getHeight();

			if (m_messages[i]->Scale > 0) {
				x = x + m_messages[i]->Scale;
				m_messages[i]->Scale -= 0.4f
						* m_videoManager->getFrameDeltaTime();
			} else {
				m_messages[i]->AMask -= 0.0001f
						* m_videoManager->getFrameDeltaTime();
			}

			m_messages[i]->draw(true, x, y);

			if (m_messages[i]->AMask <= 0) {
				delete m_messages[i];
				m_messages.erase(m_messages.begin() + i);
			}
		}
	}
}

void violetland::HUD::drawExperience(float experience, int levelPoints,
		int bottomBasePoint) {
	VideoMode screen = m_videoManager->getVideoMode();
	const int barLen = screen.Width / 4;
	const int barLeft = 2 * screen.Width / 3;
	const int barHeight = m_videoManager->RegularText->getHeight() / 4;

	const int indX = screen.Width - screen.Width / 24;
	m_resources->LevelUpIndicator->X = indX;
	m_resources->LevelUpIndicator->Y = bottomBasePoint;

	m_resources->LevelUpIndicator->draw(false, false);

	GLfloat bcolor[] = { 1.0f, 1.0f, 1.0f, 0.3f };
	GLfloat fcolor1[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat fcolor2[] = { 0.0f, 1.0f, 1.0f, 1.0f };

	drawBar(barLeft, bottomBasePoint, barLen, barHeight, experience, bcolor,
			fcolor1, fcolor2);
}

void violetland::HUD::drawBar(int x, int y, int width, int height, float value,
		GLfloat* bcolor, GLfloat* fcolor1, GLfloat* fcolor2) {
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();

	glTranslatef(x, y, 0.0f);

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);

	glColor4fv(bcolor);

	glVertex3f(0.0f, -height, 0);
	glVertex3f(width, -height, 0);
	glVertex3f(width, height, 0);
	glVertex3f(0.0f, height, 0);

	glColor4fv(fcolor1);
	glVertex3f(0.0f, -height, 0);
	glColor4fv(fcolor2);
	glVertex3f(width * value, -height, 0);
	glVertex3f(width * value, height, 0);
	glColor4fv(fcolor1);
	glVertex3f(0.0f, height, 0);

	glEnd();

	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

void violetland::HUD::drawInventory(Player* player) {
	int topBasePoint = m_videoManager->RegularText->getIndent()
			+ (m_videoManager->RegularText->getHeight()) * 4;

	VideoMode screen = m_videoManager->getVideoMode();
	const int barLeft = screen.Width / 25;
	const int barLen = screen.Width / 15;
	const int barHeight = m_videoManager->RegularText->getHeight() / 10;

	GLfloat bcolor[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat fcolor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat fcolor2[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	const float baseScale = m_videoManager->Scale * 0.2;

	const int dataLeft = barLeft + 25 * m_videoManager->Scale;

	// TODO: cache weapon image inside this class
	StaticObject* wpnImg = new StaticObject(barLeft,
		m_videoManager->RegularText->getIndent() + m_videoManager->RegularText->getHeight(),
		128, 128,
		player->getWeapon()->getDroppedTex(), false);
	wpnImg->Scale = baseScale;

	glDisable(GL_TEXTURE_2D);
	wpnImg->RMask = wpnImg->GMask = wpnImg->BMask = 1.0f;
	wpnImg->draw(false, false);
	glEnable(GL_TEXTURE_2D);
	wpnImg->RMask = wpnImg->GMask = wpnImg->BMask = 0;
	wpnImg->draw(false, false);

	delete wpnImg;

	ostringstream oss;
	oss << player->getWeapon()->Ammo << '/' << player->getWeapon()->AmmoClipSize;
	m_videoManager->RegularText->draw(oss.str(), dataLeft,
		m_videoManager->RegularText->getIndent() + m_videoManager->RegularText->getHeight(),
		TextManager::LEFT, TextManager::MIDDLE);

	m_inventoryImg[0]->draw(false, false, barLeft,
		m_videoManager->RegularText->getIndent() + m_videoManager->RegularText->getHeight() * 2,
		0.0f, baseScale);

	oss.str("");
	oss << player->Grenades;
	m_videoManager->RegularText->draw(oss.str(), dataLeft,
		m_videoManager->RegularText->getIndent() + m_videoManager->RegularText->getHeight() * 2,
		TextManager::LEFT, TextManager::MIDDLE);

	m_inventoryImg[1]->draw(false, false, barLeft,
		m_videoManager->RegularText->getIndent() + m_videoManager->RegularText->getHeight() * 3,
		0.0f, baseScale);

	oss.str("");
	oss << player->Teleports;
	m_videoManager->RegularText->draw(oss.str(), dataLeft,
		m_videoManager->RegularText->getIndent() + m_videoManager->RegularText->getHeight() * 3,
		TextManager::LEFT, TextManager::MIDDLE);

	int bonusN = 0;
	for (int i = PLAYER_BONUS_FIRST; i < PLAYER_BONUS_COUNT; i++) {
		if (player->bonusTimes[i] > 0) {
			const int y = topBasePoint + bonusN * 132 * baseScale;
			m_bonusImg[(PlayerBonusType) i]->draw(false, false, barLeft, y, 0,
					baseScale);
			drawBar(barLeft + 25 * m_videoManager->Scale, y, barLen, barHeight,
					player->bonusTimes[i] / 15000.0f, bcolor, fcolor1, fcolor2);
			bonusN++;
		}
	}
}

void violetland::HUD::drawHealth(float health, int bottomBasePoint) {
	VideoMode screen = m_videoManager->getVideoMode();
	const int barLeft = screen.Width / 12;
	const int barLen = screen.Width / 4;
	const int barHeight = m_videoManager->RegularText->getHeight() / 4;

	const int indX = screen.Width / 24;
	m_resources->HealthIndicator->X = indX;
	m_resources->HealthIndicator->Y = bottomBasePoint;

	m_resources->HealthIndicator->draw(false, false);

	GLfloat bcolor[] = { 1.0f, 1.0f, 1.0f, 0.3f };
	GLfloat fcolor1[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat fcolor2[] = { 1.0f, 1.0f, 0.0f, 1.0f };

	drawBar(barLeft, bottomBasePoint, barLen, barHeight, health, bcolor,
			fcolor1, fcolor2);
}

void violetland::HUD::drawTime(GameState* gameState) {
	int bottomBasePoint = m_videoManager->getVideoMode().Height
			- m_videoManager->RegularText->getIndent();

	const int minutes = gameState->Time / 60000;
	const int seconds = (gameState->Time - minutes * 60000) / 1000;

	ostringstream oss;
	oss << setfill('0') << setw(2) << minutes << ':' << setfill('0') << setw(2)
			<< seconds;
	m_videoManager->RegularText->draw(oss.str(),
			m_videoManager->getVideoMode().Width / 2, bottomBasePoint,
			TextManager::CENTER, TextManager::MIDDLE);
}

void violetland::HUD::drawEndGameScreen(GameState* gameState, int xp) {
	VideoMode screen = m_videoManager->getVideoMode();

	int y = screen.Height / 4;
	m_videoManager->RegularText->draw(_("They have overcome..."), screen.Width
			/ 2, y, TextManager::CENTER, TextManager::MIDDLE);

	ostringstream oss;
	oss << boost::format(_("You have earned %i points.")) % xp;
	m_videoManager->RegularText->draw(oss.str(), screen.Width / 2, y
			+ m_videoManager->RegularText->getHeight(), TextManager::CENTER,
			TextManager::MIDDLE);

	if (gameState->HighScore) {
		m_videoManager->RegularText->draw(_("Enter your name:"), screen.Width
				/ 2, y + m_videoManager->RegularText->getHeight() * 2,
				TextManager::CENTER, TextManager::MIDDLE);
		m_videoManager ->RegularText->draw(
				std::string(gameState->PlayerName).append("_").c_str(),
				screen.Width / 2, y + m_videoManager->RegularText->getHeight()
						* 3, TextManager::CENTER, TextManager::MIDDLE);
	}
}

void violetland::HUD::draw(GameState* gameState, Player* player) {
	float health = player->getHealth() / player->MaxHealth();
	float experience = (float) (player->Xp - player->LastLevelXp)
			/ (player->NextLevelXp - player->LastLevelXp);

	VideoMode screen = m_videoManager->getVideoMode();

	drawMessages();
	drawTime(gameState);

	applyEffects(health, player->LevelPoints);

	int bottomBasePoint = m_videoManager->getVideoMode().Height
			- m_videoManager->RegularText->getIndent() * 2;

	drawHealth(health, bottomBasePoint);
	drawExperience(experience, player->LevelPoints, bottomBasePoint);

	drawInventory(player);

	if (gameState->Lost && !gameState->Paused)
		drawEndGameScreen(gameState, player->Xp);

	if (!gameState->Lost)
		if (Info != "")
			m_videoManager->RegularText->draw(Info.c_str(),
					m_videoManager->getVideoMode().Width / 2,
					m_videoManager->RegularText->getIndent(),
					TextManager::CENTER, TextManager::TOP);

	if (gameState->Paused)
		m_videoManager->RegularText->draw(_("PAUSE"), screen.Width / 2,
				screen.Height / 2, TextManager::CENTER, TextManager::MIDDLE);
}

void violetland::HUD::addMessage(std::string message) {
	std::cout << message << std::endl;

	TextObject* msg = m_videoManager->SmallText->getObject(message, 0, 0,
			TextManager::LEFT, TextManager::MIDDLE);

	// Here scale is the variable for control over animation sequence
	msg->Scale = m_videoManager->getVideoMode().Width + msg->getWidth();

	m_messages.push_back(msg);
}

void violetland::HUD::reset() {
	Info = "";
	clearVector<TextObject*> (&m_messages);
}

violetland::HUD::~HUD() {
	reset();
	clearMap<PlayerBonusType, StaticObject*> (&m_bonusImg);
	clearVector(&m_inventoryImg);
}
