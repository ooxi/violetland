#ifndef HUD_H_
#define HUD_H_

#include "../system/graphic/VideoManager.h"
#include "GameState.h"
#include "Resources.h"
#include "Player.h"

class HUD {
private:
	// References to resources and managers
	VideoManager* m_videoManager;
	Resources* m_resources;
	// Objects to store images of bonuses
	std::map<Player::bonusType, StaticObject*> m_bonusImg;
	// Console messages
	std::vector<TextObject*> m_messages;
	// HUD effects timer
	int m_bounce;
	// Apply HUD effects
	void applyEffects(float health, int levelPoints);
	// Drawing of the messages console
	void drawMessages();
	// Draw the health bar
	void drawHealth(float health);
	// Draw remaining times of bonuses
	void drawBonusStack(int* bonusTimes);
	// Draw the experience bar
	void drawExperience(float experience, int levelPoints);
	// Draw the ammo counter (not implemented)
	void drawAmmo(int ammo);
	// Draw the grenades counter (not implemented)
	void drawGrenades(int grenades);
	// Draw the timer
	void drawTime(GameState* gameState);
	// Draw a bar (rectangle)
	void drawBar(int x, int y, int width, int height, float value,
			GLfloat* bcolor, GLfloat* fcolor1, GLfloat* fcolor2);
public:
	HUD(VideoManager* videoManager, Resources* resources);

	// Draw the whole HUD
	void draw(GameState* gameState, Player* player);

	// Add a message to console
	void addMessage(std::string message);

	// Setting the effects for HUD
	void setHealthBounce(bool value);
	void setLevelUpBounce(bool value);

	// Clear the messages console
	void reset();
	~HUD();
};

#endif /* HUD_H_ */
