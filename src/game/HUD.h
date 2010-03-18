#ifndef HUD_H_
#define HUD_H_

#include "../system/graphic/VideoManager.h"
#include "GameState.h"
#include "Resources.h"

class HUD {
private:
	int m_bottomBasePoint;
	VideoManager* m_videoManager;
	Resources* m_resources;
	std::vector<TextObject*> m_messages;
	void drawMessages();
	void drawHealth(float health);
	void drawExperience(float experience, int levelPoints);
	void drawAmmo(int ammo);
	void drawGrenades(int grenades);
	void drawTime(GameState* gameState);
	void drawBar(int x, int y, int width, int height, float value);
public:
	HUD(VideoManager* videoManager, Resources* resources);
	void draw(GameState* gameState, float health, float experience,
			int levelPoints, int ammo, int grenades);
	void addMessage(std::string message);
	void reset();
	~HUD();
};

#endif /* HUD_H_ */
