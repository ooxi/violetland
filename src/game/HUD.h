#ifndef HUD_H_
#define HUD_H_

#include "../system/graphic/VideoManager.h"
#include "GameState.h"
#include "Resources.h"

/*TODO: add to HUD a tool that will be able to show all
 * remaining time of time-based bonuses (something like in crimsonland)
 */

class HUD {
private:
	VideoManager* m_videoManager;
	Resources* m_resources;
	std::vector<TextObject*> m_messages;
	void drawMessages();
	void drawHealth(float health);
	void drawExperience(float experience, int levelPoints);
	void drawAmmo(int ammo);
	void drawGrenades(int grenades);
	void drawTime(GameState* gameState);
	void drawBar(int x, int y, int width, int height, float value,
			GLfloat* bcolor, GLfloat* fcolor1, GLfloat* fcolor2);
public:
	HUD(VideoManager* videoManager, Resources* resources);
	void draw(GameState* gameState, float health, float experience,
			int levelPoints, int ammo, int grenades);
	void addMessage(std::string message);
	void reset();
	~HUD();
};

#endif /* HUD_H_ */
